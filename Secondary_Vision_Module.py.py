"""
 * PROJECT:     AG~3 NEURO-PATH - Industrial Vision Edge
 * DESCRIPTION: High-speed binary serial reassembly and YOLOv8 neural network inference engine
 * for heavy industrial ADAS and worker safety tracking (PS 1).
 * VERSION:     3.0 (ET Hackathon Edition)
"""

import cv2
import numpy as np
import serial
from ultralytics import YOLO

# Initialize YOLOv8 Nano
model = YOLO("yolov8n.pt")

SERIAL_PORT = "COM4"
BAUD_RATE = 115200

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.01)
    ser.set_buffer_size(rx_size=128000, tx_size=128000)
    ser.reset_input_buffer()
    print(f"[SYSTEM OK] Locked onto Industrial Hardware Core on {SERIAL_PORT}. Building UI...")
except Exception as e:
    print(f"[FATAL ERROR]: Could not open {SERIAL_PORT}. Ensure your Arduino IDE Serial Monitor is CLOSED!")
    exit()

raw_bytes = bytearray()

while True:
    if ser.in_waiting > 0:
        raw_bytes.extend(ser.read(ser.in_waiting))
    
    # Search for JPEG Start/End markers
    start = raw_bytes.find(b'\xff\xd8')
    end = raw_bytes.find(b'\xff\xd9')
    
    if start != -1 and end != -1 and end > start:
        jpg_segment = raw_bytes[start:end+2]
        del raw_bytes[:end+2]
        
        # Decompress frame
        frame = cv2.imdecode(np.frombuffer(jpg_segment, dtype=np.uint8), cv2.IMREAD_COLOR)
        
        if frame is not None:
            # HACKATHON FIX: Added Class 0 (Person/Worker) and 7 (Truck/Machinery)
            results = model(frame, stream=True, classes=[0, 2, 5, 7], conf=0.35) 
            closest_target_distance = 999

            for result in results:
                boxes = result.boxes
                for box in boxes:
                    x1, y1, x2, y2 = map(int, box.xyxy[0])
                    confidence = float(box.conf[0])
                    label = model.names[int(box.cls[0])]

                    # Spatial Distance Math
                    box_height = y2 - y1 if (y2 - y1) > 0 else 1
                    K_FACTOR = 6000  
                    estimated_distance = K_FACTOR / box_height
                    
                    if estimated_distance < closest_target_distance:
                        closest_target_distance = int(estimated_distance)

                    # Dynamic UI Coloration (Red for critical strike zone)
                    color = (0, 0, 255) if estimated_distance < 1.5 else (0, 255, 255)
                    cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
                    cv2.putText(frame, f"HAZARD: {label} {confidence:.2f} | Dist: {estimated_distance:.1f}m", 
                                (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.4, color, 1)

            # Fire serial trigger back to ESP32 Hardware Core
            if closest_target_distance != 999:
                ser.write(f"DIST:{closest_target_distance}\n".encode('utf-8'))

            cv2.imshow("NEURO-PATH Industrial HUD (ACTIVE)", frame)

    # Memory Leak Prevention
    elif len(raw_bytes) > 50000:
        raw_bytes.clear()
        ser.reset_input_buffer()

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

ser.close()
cv2.destroyAllWindows()