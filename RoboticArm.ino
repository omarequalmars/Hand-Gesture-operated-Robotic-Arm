
#include <Servo.h>
#include <string.h>
#include "filters.h"

Servo manipulator; // from 140 (open) to 180 (close)
Servo x_axis_joint; // 0 to 140 (75 is upright)
Servo z_axis_joint; // 0 to 180
String readString;
String data;
String v;
String i;
String w;
String h;
int ind1; // , locations
int ind2;
int ind3;
int ind4;
String dist;
int x_filtered = 0;
int y_filtered = 0;
const char delimiter = ',';
int dist_num = 0;
int dist_num_filtered = 0;
int angle1 = 0;
int angle2 = 0;
void setup() {
  manipulator.attach(8);
  x_axis_joint.attach(7);
  manipulator.write(75);
  x_axis_joint.write(0);
  Serial.begin(115200);
  Serial.setTimeout(100);

}

void loop() {
if (Serial.available() > 0) 
{//Wait for user input
      String data=Serial.readStringUntil('\n');
      data.trim();
      int commaIndex = data.indexOf(',');
      if (commaIndex >= 0) { // if the comma separator is found
      String xString = data.substring(0, commaIndex); // extract the X string
      String yString = data.substring(commaIndex + 1); // extract the Y string
      
      int x = xString.toInt(); // cast the X string to an int
      int y = yString.toInt(); // cast the Y string to an int
      
      
      angle1 = map(x, 0, 40, 0, 180);
      angle2 = map(y, 10, 40, 220, 140);
      angle1 = (angle1 < 0)? 0:angle1 ;
      angle2 = (angle2 < 10)? 0:angle2 ;
      x_filtered = ComplementaryFilter(angle1, x_filtered, 0.2);
      y_filtered = ComplementaryFilter(angle2, y_filtered, 1);
      x_axis_joint.write(x_filtered);
      manipulator.write(y_filtered);
      Serial.print(x);
      Serial.print(',');
      Serial.println(y);
      delay(50);
      }
      
        

}

}
