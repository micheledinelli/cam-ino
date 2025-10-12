#ifndef CONFIG_H
#define CONFIG_H

const int HCSR04_TRIG_PIN = 11;  // Trigger pin for ultrasonic sensor HCSR04
const int HCSR04_ECHO_PIN = 12;  // Echo pin for ultrasonic sensor HCSR04
const int MAX_DISTANCE = 400;    // Maximum distance reachable from ultrasonic burst

const int TRIGGER_DISTANCE_CM = 100; // Distance threshold to trigger the camera

const int DHT_PIN = 2;  // DHT temperature sensor pin
#define DHTTYPE DHT11   // DHT sensor type

const int CAM_TRIG_PIN = 7;  // Trigger pin for ESP32 camera

#endif
