#include <Servo.h>

Servo radarServo;  // sweeps ultrasonic sensor
Servo gunServo;    // controls toy gun

const int trigPin = 9;
const int echoPin = 10;
const int ledPin  = 6;

long duration;
int distance;
int targetDistance = 25; // cm threshold for firing

void setup() {
  Serial.begin(9600);
  radarServo.attach(5);
  gunServo.attach(3);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Sweep left to right
  for (int angle = 0; angle <= 180; angle++) {
    radarServo.write(angle);
    delay(8);  // scanning speed
    distance = getDistance();
    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);

    // High sensitivity trigger
    if (distance > 0 && distance < targetDistance) {
      fireGun(angle);
    }
  }

  // Sweep right to left
  for (int angle = 180; angle >= 0; angle--) {
    radarServo.write(angle);
    delay(8);
    distance = getDistance();
    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);

    if (distance > 0 && distance < targetDistance) {
      fireGun(angle);
    }
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return 0;
  return duration * 0.034 / 2;
}

void fireGun(int aimAngle) {
  gunServo.write(aimAngle);   // aim at detected object
  digitalWrite(ledPin, HIGH); // trigger light ON
  delay(150);                 // simulate shot
  digitalWrite(ledPin, LOW);  // light OFF
}
