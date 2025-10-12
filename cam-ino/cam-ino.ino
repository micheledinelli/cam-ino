#include <NewPing.h>
#include "DHT.h"
#include "Config.h"

NewPing sonar(HCSR04_TRIG_PIN, HCSR04_ECHO_PIN, MAX_DISTANCE);
DHT dht(DHT_PIN, DHTTYPE);

float speedOfSound, distance, duration;
float lastValidDistance = 0;

void setup() {
  Serial.begin(9600);

  dht.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CAM_TRIG_PIN, OUTPUT);
}

void loop() {
  delay(500);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Calculate speed of sound in m/s
  speedOfSound = 331.4 + (0.6 * t) + (0.0124 * h);

  duration = sonar.ping_median(10);
  duration = duration / 1000000.0;  // us to s

  distance = (speedOfSound * duration) / 2;
  distance = distance * 100;  // m to cm

  if (distance > 0.0) {
    lastValidDistance = distance;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm  | Last valid: ");
  Serial.print(lastValidDistance);
  Serial.println(" cm");

  if (distance > 0.0 && distance < TRIGGER_DISTANCE_CM) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(CAM_TRIG_PIN, HIGH);
    Serial.println("Sent HIGH");
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(CAM_TRIG_PIN, LOW);
  }
}
