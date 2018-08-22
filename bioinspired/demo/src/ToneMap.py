import cv2
import numpy as np

cap = cv2.VideoCapture("../../../statics/video1.avi")

ret, frame = cap.read()
if not ret:
    exit(0)

size = list(frame.shape[0:2])
[size[0], size[1]] = [size[1], size[0]]
size = tuple(size)

toner = cv2.bioinspired.RetinaFastToneMapping_create(size)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    toner.setup()
    toneImg = toner.applyFastToneMapping(frame)

    cv2.imshow("tracker", toneImg)

    key = cv2.waitKey(10)
    if key == ord('q'):
        break;