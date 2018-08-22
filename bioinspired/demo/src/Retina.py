import cv2
import numpy as np

cap = cv2.VideoCapture("../../../statics/video1.avi")

ret, frame = cap.read()
if not ret:
    exit(0)

size = list(frame.shape[0:2])
[size[0], size[1]] = [size[1], size[0]]
size = tuple(size)

retina = cv2.bioinspired.Retina_create(size)

retina.setupOPLandIPLParvoChannel(True, True, 0.0, 0.0, 0.9, 0.0, 0.0, 0.0, 0.0)
retina.setupIPLMagnoChannel(True, 1.0, 0.0, 0.0, 1.0, 0.9, 100.0, 100.0)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    retina.setup()
    retina.run(frame)
    parvoImg = retina.getParvo()
    magnoImg = retina.getMagno()

    cv2.imshow("parvo", parvoImg)
    cv2.imshow("magno", magnoImg)

    key = cv2.waitKey(10)
    if key == ord('q'):
        break;