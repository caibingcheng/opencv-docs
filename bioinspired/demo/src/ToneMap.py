import cv2
import numpy as np

cap = cv2.VideoCapture("../../../statics/video1.avi")

ret, frame = cap.read()
if not ret:
    exit(0)

toner = cv2.bioinspired.RetinaFastToneMapping_create(frame.shape[0:2])

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    toner.setup()
    toneImg = toner.applyFastToneMapping(frame)

    cv2.imshow("tracker", frame)

    key = cv2.waitKey(10)
    if key == ord('q'):
        break;