#include <Wire.h>

const int triggerPin = 2;
const int echoPin = 3;
long duration, distance;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600); // Start serial communication for debugging
  Wire.begin(8);  // Join I2C bus with address #8
  Wire.onRequest(requestEvent);  // Register event
}

void loop() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}

void requestEvent() {
  Wire.write((int)(distance >> 8));    // sends high byte
  Wire.write((int)(distance & 0xFF));  // sends low byte
}