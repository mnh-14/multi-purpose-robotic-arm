import serial
import time

PORT = 'COM3'       # Replace with your Arduino's COM port
BAUD_RATE = 9600

try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=0.1)
    time.sleep(2)  # Arduino auto-reset delay

    print("[INFO] Serial bridge started. Type 'exit' to quit.")

    while True:

        # Send user input to Arduino (which forwards to ATmega32)
        user_input = input("\n[YOU] > ")
        if user_input.lower() == 'exit':
            break
        if user_input:
            ser.write(user_input[0].encode())

        # Read from Arduino (data coming from ATmega32)
        while ser.in_waiting:
            data = ser.read(ser.in_waiting).decode(errors='ignore')
            print(f"[ATMEGA32] {data}", end='')

except serial.SerialException as e:
    print(f"[ERROR] {e}")

finally:
    ser.close()
    print("[INFO] Serial bridge stopped.")