#include <NewPing.h>
#include "DHT.h"

#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 400
#define DHTPIN 2
#define DHTTYPE DHT11

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
DHT dht(DHTPIN, DHTTYPE);

float speedOfSound, distance, duration;
float lastValidDistance = 0;  // Store the last valid distance

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  delay(100);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Calculate speed of sound in m/s
  speedOfSound = 331.4 + (0.6 * t) + (0.0124 * h);

  // Get median ping duration in microseconds
  duration = sonar.ping_median(10);
  duration = duration / 1000000.0;  // Convert microseconds to seconds

  // Calculate distance in cm
  distance = (speedOfSound * duration) / 2;
  distance = distance * 100;  // meters to centimeters

  // Only update lastValidDistance if the distance is not 0
  if (distance > 0.0) {
    lastValidDistance = distance;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm  | Last valid: ");
  Serial.print(lastValidDistance);
  Serial.println(" cm");

  // LED logic: on if distance is valid and < 50 cm
  if (distance > 0.0 && distance < 50.0) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
