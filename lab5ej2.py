import RPi.GPIO as GPIO
import time
import serial

try:
    ser = serial.Serial('/dev/serial0', 9600, timeout=1)
except:
    print("Error: No se pudo abrir el puerto serial. Revisa raspi-config.")

TRIG = 18
ECHO = 24

GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

def get_distance():
    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)
    
    start = time.time()
    while GPIO.input(ECHO) == 0:
        start = time.time()
    while GPIO.input(ECHO) == 1:
        end = time.time()
    
    dist = (end - start) * 17150
    return int(dist) 

try:
    print("init Transmition")
    while True:
        distancia = get_distance()
        
        mensaje = f"{distancia}\n"
        ser.write(mensaje.encode('utf-8'))
        
        print(f">> enviado: {distancia} cm")
        time.sleep(0.5)

except KeyboardInterrupt:
    GPIO.cleanup()
    print("\ndetenido")