#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

int16_t gx, gy, gz;
float angleX = 0, angleY = 0;

unsigned long lastTime;

const int threshold = 800;
unsigned long highLightStartTime = 0;
bool isTiming = false;

int sensorPin = A0;
int sensorValue = 0;

int buzzerPin = 7;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  pinMode(buzzerPin, OUTPUT);
  lastTime = millis();

  delay(1000);
  Serial.println("Ready. Initial angle = 0. Tilt from here.");
}

void checkLight() {
  sensorValue = analogRead(sensorPin);

  if (sensorValue > threshold) {
    if (!isTiming) {
      isTiming = true;
      highLightStartTime = millis();
    }
    else if (millis() - highLightStartTime >= 5000) {
      Serial.println("Light to high");
      tone(buzzerPin, 1000);
      delay(1000);
      noTone(buzzerPin);
    }
  }
  else {
    isTiming = false;
    Serial.println("Not watching phone!");
  }

  delay(100);
}

void loop() {
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  mpu.getRotation(&gx, &gy, &gz);

  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;

  angleX += gyroX * dt;
  angleY += gyroY * dt;

  Serial.print("angleX: ");
  Serial.print(angleX);
  Serial.print(" | angleY: ");
  Serial.println(angleY);

  if (abs(angleX) > 20 || abs(angleY) > 20) {
    tone(buzzerPin, 1000);
    Serial.println("TILTED TOO MUCH");
  } else {
    noTone(buzzerPin);
  }

  delay(10);
}
