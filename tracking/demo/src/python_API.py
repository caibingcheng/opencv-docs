import cv2
import numpy as np

cap = cv2.VideoCapture("../../../statics/video1.avi")

# tracker = cv2.TrackerBoosting_create()
# tracker = cv2.TrackerCSRT_create()
# tracker = cv2.TrackerKCF_create()
# tracker = cv2.TrackerMedianFlow_create()
# tracker = cv2.TrackerMIL_create()
# tracker = cv2.TrackerMOSSE_create()
tracker = cv2.TrackerTLD_create()

init = True

while True:
    ret, frame = cap.read()
    if not ret:
        break

    if init:
        # bounding_box = cv2.selectROI(frame)
        # cv2.destroyAllWindows() 
        bounding_box = (470, 40, 60, 50)
        tracker.init(frame, bounding_box)
        init = False
        continue

    ret, bounding_box = tracker.update(frame)
    bounding_box = tuple(int(ele) for ele in bounding_box)

    cv2.rectangle(frame, bounding_box[0:2], tuple(np.array(bounding_box[0:2]) + np.array(bounding_box[2:4])), (0, 255, 0), 4)
    cv2.imshow("tracker", frame)

    key = cv2.waitKey(10)
    if key == ord('q'):
        break;