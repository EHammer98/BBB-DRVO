#include <Wire.h>

const int triggerPin = 2;
const int echoPin = 3;
long duration, distance;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Wire.begin(8);  // Join I2C bus with address #8
  Wire.onRequest(requestEvent);  // Register event
}

void loop() {
  // Clears the triggerPin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Sets the triggerPin on HIGH state for 10 micro seconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  delay(100);
}

void requestEvent() {
  Wire.write((int)(distance >> 8));    // sends high byte
  Wire.write((int)(distance & 0xFF));  // sends low byte
}
