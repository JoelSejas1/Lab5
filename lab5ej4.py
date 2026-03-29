
import RPi.GPIO as GPIO
import time


PWM_PIN = 12 
IN1 = 23      
IN2 = 24      

GPIO.setmode(GPIO.BCM)
GPIO.setup(PWM_PIN, GPIO.OUT)
GPIO.setup(IN1, GPIO.OUT)
GPIO.setup(IN2, GPIO.OUT)

GPIO.output(IN1, GPIO.HIGH)
GPIO.output(IN2, GPIO.LOW)

pwm_motor = GPIO.PWM(PWM_PIN, 1000)
pwm_motor.start(0) 

try:
    while True:
        for duty in range(0, 101): 
            pwm_motor.ChangeDutyCycle(duty)
            print(f"Duty Cycle: {duty}%")
            
            time.sleep(0.5)
            
        print("loop")

except KeyboardInterrupt:
    print("\ndetener motor")
finally:
    pwm_motor.stop()
    GPIO.cleanup()