import cv2
import mediapipe as mp
import numpy as np
import serial 
import time

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands
mp_holistic = mp.solutions.holistic
#intilaiing serial port
arduino = serial.Serial(port='COM15', baudrate=115200, timeout = 0.1)

def write(x,y):
    x = str(int(x))
    y = str(int(y))
    arduino.write((x+','+y).encode("utf-8"))
    time.sleep(0.05)
    data=arduino.readline()
    print(data) 
x = 0
y = 0


# Start capturing video from the default camera
cap = cv2.VideoCapture(0)
#intianting holistic model
with mp_holistic.Holistic(min_detection_confidence=0.5, min_tracking_confidence=0.5) as holistic:
    
    while cap.isOpened():
        success, image = cap.read()
        #recoloring the feed
        image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
        #making detections
        results = holistic.process(image)
        image.flags.writeable = True

        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR) #recoloring the image back to BGR for rendering
          
        #drawing left hand landmarks
        mp_drawing.draw_landmarks(image, results.left_hand_landmarks, mp_holistic.HAND_CONNECTIONS)
        #drawing right hand landmarks
        mp_drawing.draw_landmarks(image, results.right_hand_landmarks, mp_holistic.HAND_CONNECTIONS)
                # Extract landmark coordinates for thumb tip and index tip
        if results.right_hand_landmarks:
            #Right hand
            thumb_tip_r = results.right_hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_tip_r = results.right_hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            # Calculate Euclidean distance between thumb tip and index tip
            distance_r = np.sqrt((thumb_tip_r.x - index_tip_r.x) ** 2 + (thumb_tip_r.y - index_tip_r.y) ** 2)
            # if distance_r is None:
            #     distance_r = 0
            
            x = 100*distance_r

            # Draw distance text on image
            cv2.putText(image, f'Distance_R: {distance_r:.2f} px', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
            # Draw line between thumb tip and index tip
            thumb_tip_r_x, thumb_tip_r_y = int(thumb_tip_r.x * image.shape[1]), int(thumb_tip_r.y * image.shape[0])
            index_tip_r_x, index_tip_r_y = int(index_tip_r.x * image.shape[1]), int(index_tip_r.y * image.shape[0])
            #showing that on the image to be displayed
            cv2.line(image, (thumb_tip_r_x, thumb_tip_r_y), (index_tip_r_x, index_tip_r_y), (0, 255, 0), 3)

        if results.left_hand_landmarks:
            #Left hand
            thumb_tip_l = results.left_hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_tip_l = results.left_hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            # Calculate Euclidean distance between thumb tip and index tip
            distance_l = np.sqrt((thumb_tip_l.x - index_tip_l.x) ** 2 + (thumb_tip_l.y - index_tip_l.y) ** 2)


            

            y = 100*distance_l

            # Draw distance text on image
            cv2.putText(image, f'Distance_L: {distance_l:.2f} px', (25, 25), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
            # Draw line between thumb tip and index tip
            thumb_tip_l_x, thumb_tip_l_y = int(thumb_tip_l.x * image.shape[1]), int(thumb_tip_l.y * image.shape[0])
            index_tip_l_x, index_tip_l_y = int(index_tip_l.x * image.shape[1]), int(index_tip_l.y * image.shape[0])
            #showing that on the image to be displayed

            cv2.line(image, (thumb_tip_l_x, thumb_tip_l_y), (index_tip_l_x, index_tip_l_y), (0, 255, 0), 3)
        cv2.imshow('camera wowo', image)
        write(x,y)
        if cv2.waitKey(5) & 0xFF == 27:
            break
cap.release()
cv2.destroyAllWindows()