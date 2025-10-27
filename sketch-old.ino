#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>

Adafruit_MPU6050 mpu;
Servo servo1, servo2, servo3, servo4, servo5;

int pirPin = 25;
bool motionDetected;

void setup() {
Serial.begin(115200);
Wire.begin();
pinMode(pirPin, INPUT);
Wire.begin(21, 22);
servo1.attach(18);
servo2.attach(17);
servo3.attach(16);
servo4.attach(15);
servo5.attach(14);

setAllServo(90);

if (!mpu.begin()) {
Serial.println("MPU6050 not detected.");
while (1);
}
Serial.println("MPU6050 is ready.");
}

void loop() {
motionDetected = digitalRead(pirPin);

if (motionDetected) {
Serial.println("PIR detected a motion.");
setAllServo(135);
delay(500);
setAllServo(90);
}
else {
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);


float roll = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
float yaw = g.gyro.z;

Serial.print("Roll: "); Serial.print(roll);
Serial.print(" | Pitch: "); Serial.print(pitch);
Serial.print(" | Yaw: "); Serial.println(yaw);

// Roll Pos/Neg S1 & S2 Berlawanan arah bidang.
if (roll > 5) {
  servo1.write(60);
  servo2.write(120);
} 
else if (roll < -5) {
  servo1.write(120);
  servo2.write(60);
} 
else {
  servo1.write(90);
  servo2.write(90);
}
// Pitch Pos/Neg S3 & S4 Searah arah bidang.
if (pitch > 5) {
  servo3.write(120);
  servo4.write(120);
} 
else if (pitch < -5) {
  servo3.write(60);
  servo4.write(60);
} 
else {
  servo3.write(90);
  servo4.write(90);
}
// Yaw Pos/Neg S5 mengikuti arah bidang, lalu kembali.
if (yaw > 1) {
  servo5.write(120);
  delay(1000);
  servo5.write(90);
} 
else if (yaw < -1) {
  servo5.write(60);
  delay(1000);
  servo5.write(90);
}
}
delay(100);
}

void setAllServo(int pos) {
servo1.write(pos);
servo2.write(pos);
servo3.write(pos);
servo4.write(pos);
servo5.write(pos);
}
