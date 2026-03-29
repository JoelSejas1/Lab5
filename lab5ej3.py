import RPi.GPIO as GPIO
import time

PWM_PIN = 12  
IN1 = 23      
IN2 = 24      

GPIO.setmode(GPIO.BCM)
GPIO.setup(PWM_PIN, GPIO.OUT)
GPIO.setup(IN1, GPIO.OUT)
GPIO.setup(IN2, GPIO.OUT)

pwm_motor = GPIO.PWM(PWM_PIN, 1000)
pwm_motor.start(0)

def set_motor_speed(duty_cycle):
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    pwm_motor.ChangeDutyCycle(duty_cycle)
    print(f"Probando motor con Duty Cycle al {duty_cycle}%")

try:
    ciclos = [75, 25, 45, 50]
    for dc in ciclos:
        set_motor_speed(dc)
        time.sleep(5)  
        
    pwm_motor.stop()

except KeyboardInterrupt:
    pass
finally:
    GPIO.cleanup()