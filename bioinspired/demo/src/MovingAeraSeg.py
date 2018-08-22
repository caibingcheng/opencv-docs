import cv2
import numpy as np

cap = cv2.VideoCapture("../../../statics/video1.avi")

ret, frame = cap.read()
if not ret:
    exit(0)

size = list(frame.shape[0:2])
[size[0], size[1]] = [size[1], size[0]]
size = tuple(size)

retina = cv2.bioinspired.TransientAreasSegmentationModule_create(size)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    retina.setup()
    retina.run(frame)
    movingImg = retina.getSegmentationPicture()

    cv2.imshow("movingImg", movingImg * 255)

    key = cv2.waitKey(10)
    if key == ord('q'):
        break;