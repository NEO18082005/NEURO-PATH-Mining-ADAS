"""
PROJECT:     NEURO-PATH - Edge-AI Vision Node
PLATFORM:    Poco F1 / Snapdragon 845-class edge device
DESCRIPTION: Runs YOLOv8 Nano on the native camera while receiving fused
             Radar/Sonar distance telemetry from the ESP32 over UART.
VERSION:     SIH 2026 Zero-Cloud Architecture
"""

from __future__ import annotations

import threading
from typing import Optional

import cv2
import serial
from serial import SerialException
from ultralytics import YOLO


# Configure these values for the Poco F1 / Termux or a desktop test host.
SERIAL_PORT = "COM4"  # Use /dev/ttyUSB0 or /dev/ttyACM0 on Linux/Termux.
BAUD_RATE = 115200
CAMERA_INDEX = 0  # Native Poco F1 camera index.
FRAME_WIDTH = 640
FRAME_HEIGHT = 480
CONFIDENCE_THRESHOLD = 0.40
DANGER_DISTANCE_CM = 150

# COCO classes requested for the NEURO-PATH vision node:
# 0 = person, 2 = car, 5 = bus, 7 = truck.
DETECTION_CLASSES = [0, 2, 5, 7]


class Esp32TelemetryReader:
    """Read fused ESP32 distance telemetry without blocking camera inference."""

    def __init__(self, port: str, baud_rate: int) -> None:
        self.port = port
        self.baud_rate = baud_rate
        self._distance_cm: Optional[int] = None
        self._lock = threading.Lock()
        self._stop_event = threading.Event()
        self._thread: Optional[threading.Thread] = None

    @property
    def distance_cm(self) -> Optional[int]:
        with self._lock:
            return self._distance_cm

    def start(self) -> None:
        self._thread = threading.Thread(
            target=self._read_loop,
            name="ESP32-UART-Telemetry",
            daemon=True,
        )
        self._thread.start()

    def stop(self) -> None:
        self._stop_event.set()
        if self._thread is not None:
            self._thread.join(timeout=1.0)

    def _read_loop(self) -> None:
        try:
            with serial.Serial(self.port, self.baud_rate, timeout=0.10) as connection:
                print(f"[SYSTEM OK] ESP32 telemetry link established on {self.port}")
                while not self._stop_event.is_set():
                    raw_line = connection.readline()
                    if not raw_line:
                        continue

                    line = raw_line.decode("utf-8", errors="ignore").strip()
                    if not line.startswith("DIST:"):
                        continue

                    value = line.split(":", 1)[1].strip()
                    try:
                        distance = int(value)
                    except ValueError:
                        continue

                    with self._lock:
                        self._distance_cm = distance
        except SerialException as error:
            print(f"[WARNING] ESP32 telemetry link offline: {error}")
        finally:
            print("[SYSTEM] ESP32 telemetry reader stopped")


def class_name(model: YOLO, class_id: int) -> str:
    names = model.names
    if isinstance(names, dict):
        return str(names.get(class_id, class_id)).upper()
    return str(names[class_id]).upper()


def draw_hud(frame, distance_cm: Optional[int], telemetry_online: bool) -> None:
    """Draw the operator-facing radar/vision status overlay."""
    panel_color = (0, 0, 0)
    cv2.rectangle(frame, (10, 10), (460, 88), panel_color, -1)

    if distance_cm is None:
        distance_text = "SCANNING..."
        distance_color = (0, 255, 255)
    else:
        distance_text = f"{distance_cm} cm"
        distance_color = (0, 0, 255) if distance_cm < DANGER_DISTANCE_CM else (0, 255, 0)

    link_text = "UART: ONLINE" if telemetry_online else "UART: OFFLINE"
    cv2.putText(
        frame,
        f"RADAR THREAT: {distance_text}",
        (20, 42),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.70,
        distance_color,
        2,
        cv2.LINE_AA,
    )
    cv2.putText(
        frame,
        link_text,
        (20, 72),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.55,
        (255, 255, 255),
        1,
        cv2.LINE_AA,
    )


def main() -> int:
    print("[SYSTEM] Loading YOLOv8 Nano...")
    model = YOLO("yolov8n.pt")

    telemetry = Esp32TelemetryReader(SERIAL_PORT, BAUD_RATE)
    telemetry.start()

    # Native camera feed: no ESP32-CAM JPEG decoding or binary reassembly.
    cap = cv2.VideoCapture(CAMERA_INDEX)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, FRAME_WIDTH)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT)

    if not cap.isOpened():
        print(f"[ERROR] Could not open native camera index {CAMERA_INDEX}.")
        telemetry.stop()
        return 1

    print("[SYSTEM OK] NEURO-PATH Vision Node active. Press Q to quit.")

    try:
        while True:
            success, frame = cap.read()
            if not success:
                print("[ERROR] Native camera feed lost.")
                break

            # Detect only Person, Car, Bus, and Truck from the YOLOv8 COCO model.
            results = model(
                frame,
                stream=True,
                classes=DETECTION_CLASSES,
                conf=CONFIDENCE_THRESHOLD,
                verbose=False,
            )

            for result in results:
                for box in result.boxes:
                    x1, y1, x2, y2 = map(int, box.xyxy[0].tolist())
                    confidence = float(box.conf[0])
                    detected_id = int(box.cls[0])
                    label = class_name(model, detected_id)

                    cv2.rectangle(frame, (x1, y1), (0, 255, 255), 2)
                    cv2.putText(
                        frame,
                        f"VISION: {label} | {confidence:.2f}",
                        (x1, max(y1 - 10, 20)),
                        cv2.FONT_HERSHEY_SIMPLEX,
                        0.50,
                        (0, 255, 255),
                        2,
                        cv2.LINE_AA,
                    )

            draw_hud(
                frame,
                telemetry.distance_cm,
                telemetry._thread is not None and telemetry._thread.is_alive(),
            )
            cv2.imshow("NEURO-PATH HUD - Poco F1 Edge AI", frame)

            if cv2.waitKey(1) & 0xFF == ord("q"):
                break
    finally:
        cap.release()
        telemetry.stop()
        cv2.destroyAllWindows()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
