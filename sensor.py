import RPi.GPIO as GPIO
from gpiozero import MotionSensor, LED
from signal import pause
import time

ledPIN = 17

GPIO.setmode(GPIO.BCM)
PIR_PIN = 27
GPIO.setup(PIR_PIN, GPIO.IN)

led = LED(ledPIN)

def MOTION(PIR_PIN):
               print ("Motion Detected!")

print ("PIR Module Test (CTRL+C to exit)")
time.sleep(2)
print ("Ready")

try:
     GPIO.add_event_detect(PIR_PIN, GPIO.RISING, callback=MOTION)
     while 1:
          print (GPIO.input(27))
          if GPIO.input(27):
               led.on()
          else:
               led.off()
               time.sleep(0.25)
except KeyboardInterrupt:
               print (" Quit")
               GPIO.cleanup()
