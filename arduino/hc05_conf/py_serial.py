
import serial
import time

# Replace 'COM6' with your actual Bluetooth COM port
PORT = 'COM6'
BAUD_RATE = 38400  # This must match your HC-05 and Arduino sketch baud rate

try:
    bt_serial = serial.Serial(PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Give the connection some time to stabilize
    print(f"[INFO] Connected to {PORT} at {BAUD_RATE} baud.")
except serial.SerialException as e:
    print(f"[ERROR] Could not open serial port {PORT}: {e}")
    exit(1)

try:
    while True:
        user_input = input("Enter message to send (or 'exit' to quit): ")

        if user_input.lower() == "exit":
            print("[INFO] Exiting...")
            break

        bt_serial.write(user_input.encode('utf-8'))
        bt_serial.write(b'\n')  # Send newline if needed by Arduino

        # Optional: Read and display response from Arduino
        time.sleep(0.5)
        while bt_serial.in_waiting:
            response = bt_serial.readline().decode('utf-8', errors='ignore').strip()
            print(f"[RECEIVED] {response}")

except KeyboardInterrupt:
    print("\n[INFO] Interrupted by user. Exiting...")

finally:
    bt_serial.close()
    print("[INFO] Serial port closed.")