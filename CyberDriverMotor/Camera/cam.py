import cv
from time import sleep

capture = cv.CaptureFromCAM(-1)
config = {
    cv.CV_CAP_PROP_BRIGHTNESS: 50, 
    cv.CV_CAP_PROP_CONTRAST: 50,
    cv.CV_CAP_PROP_SATURATION: 50,
}
for param,value in config.iteritems():
    cv.SetCaptureProperty(capture,param, value)
cv.NamedWindow("capture", cv.CV_WINDOW_AUTOSIZE)
try:
    while True:
        frame = cv.QueryFrame(capture)
        cv.ShowImage("capture", frame)
        cv.WaitKey(10)
except Exception:
    pass
