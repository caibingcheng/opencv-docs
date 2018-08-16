import cv2
import numpy as np

cap = cv2.VideoCapture("../../../statics/video1.avi")

# saliency = cv2.saliency.StaticSaliencySpectralResidual_create()
# saliency = cv2.saliency.StaticSaliencyFineGrained_create()
# saliency = cv2.saliency.ObjectnessBING_create(); saliency.setTrainingPath("../../../statics/bing_models")
saliency = cv2.saliency.MotionSaliencyBinWangApr2014_create(); init = True

while True:
    ret, frame = cap.read()
    if not ret:
        break;

    if init:
        saliency.setImageHeight(frame.shape[0])
        saliency.setImageWidth(frame.shape[1])
        saliency.init()
        print(saliency.getImageWidth())
        init = False

    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    ret, saliency_map = saliency.computeSaliency(frame)

    cv2.imshow("frame", saliency_map * 255)

    key = cv2.waitKey(10)
    if key == ord('q'):
        break