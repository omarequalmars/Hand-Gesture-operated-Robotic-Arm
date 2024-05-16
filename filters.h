

float dotproduct(float array1[], float array2[],int array_size);
float ComplementaryFilter(float readings, float filtered, float alpha);
float ComplementaryFusionFilter(float readings_1, float delta_readings_2,float updated, float alpha);
float FIR_filter(float x_0, int window_size, float weights[]);
float IIR_filter(float x_0,int window_size, float weights_input[], float weights_feedback[]);

float dotproduct(float array1[], float array2[],int array_size){
        static float sum;
        sum = 0;
        static int i = 0;
        for(i=0;i<array_size;i++){
            sum += array1[i]*array2[i];
        }
        return sum;
    }

    float ComplementaryFilter(float readings, float filtered, float alpha){ // Infinite Impulse Response Filter
        filtered = readings*alpha + filtered*(1-alpha);
        return filtered;
        /*

        This filter is a super basic IIR filter
        It takes an exponentially weighed moving average over the readings
        to the effect of smoothing out any ripples.
        ================================
        alpha is the MA Weight:
        alpha = 1 means the filter output will essentially be the same as the input.
        As alpha is decreased, the effective moving average window is increased
        to the effect of higher order smoothing, however note that this comes with a larger lag effect
        where the output is phase-shifted

        0 <= alpha <= 1

        */
    }

    float ComplementaryFusionFilter(float readings_1, float delta_readings_2,float updated, float alpha){
      updated = readings_1*alpha + (updated + delta_readings_2)*(1-alpha);
      return updated;
      /*
       This is a sensor fusion filter based on the complementary filter concept
       It updates the state (readings_1) from sensor 1
       and it's derivative from sensor 2 and recursively updates the fused reading.
       It is recommended to trace the function for the best understanding.
       =======================================================
       alpha is the weight coefficient for sensor 1 (proportional to how much it is 'trusted')
       0 <= alpha <= 1
       in the extreme cases, alpha = 1 means the filter will converge to sensor 1
       and alpha = 0 means it will converge to sensor 2
       readings_1 is the reading from sensor 1
       readings_2_dot is the reading from sensor 2
       updated is the filter output
       */
    }

    float FIR_filter(float x_0, int window_size, float weights[]){
        // defining input array
        static float output;
        static int i;
        output = 0;
        static float X[10] = {0,0,0,0,0,0,0,0,0,0};
           // saving the most recent reading
      X[0]=x_0;
      output = dotproduct(X,weights,window_size);
      for(i=window_size;i<0;i--){
        X[i] = X[i-1];
      }
      return output;
    }

    float IIR_filter(float x_0,int window_size, float weights_input[], float weights_feedback[]){
        // defining input and output arrays
        static float output;
    static float X[5] = {0,0,0,0,0};
    static float Y[4] = {0,0,0,0};
    static int i;
    if(window_size>5){
        window_size = 5;
         // max window size is 5 for speed in calculation
    }
    X[0]=x_0;
    output = dotproduct(X,weights_input,window_size) - dotproduct(Y,weights_feedback,window_size-1);
    for(int i = window_size;i > 0;i--){
       // delaying each received input
       if (i == (window_size - 1)){
        Y[i]=Y[i-1];
       }else{
        X[i]=X[i-1];
        Y[i]=Y[i-1];
       }
     }
      Y[0]=output;
      return output;
    }

    float DirectFormII(float x_0, float feedback_weights[], float input_weights[]){
    static float v[3] = {0,0,0};
    static float y = 0;
    v[0] = x_0 - feedback_weights[0]*v[1] - feedback_weights[1]*v[2];
    y = v[0]*input_weights[0] + v[1]*input_weights[1] + v[2]*input_weights[2];
    for (int i=2;i<0;i--){
        v[i] = v[i-1];
    }
    return y;
    }
