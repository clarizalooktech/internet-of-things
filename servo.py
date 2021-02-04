import RPi.GPIO as GPIO
from gpiozero import MotionSensor, LED
from signal import pause
import time

servoPIN = 22

GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPIN, GPIO.OUT)  # Set servo pin as output
 
p = GPIO.PWM(servoPIN, 50)      # PWM with 50Hz
p.start(12.5)                   # Initialize at resting position
try:
    p.ChangeDutyCycle(2.5)      # Command servo to move to dispensing position
    time.sleep(1.0)             # Wait for motion to complete
    p.ChangeDutyCycle(12.5)     # Command servo to move to resting position
    time.sleep(1.0)             # Wait for motion to complete
    p.stop()                    # Stop servo control
    GPIO.cleanup()
except KeyboardInterrupt:
  p.stop()
  GPIO.cleanup()
