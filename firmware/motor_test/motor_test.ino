#include <Arduino.h>

struct MotorPins {
  uint32_t pwm;
  uint32_t forward;
  uint32_t reverse;
};

const MotorPins motors[] = {
  {PC6, PB12, PA10},
  {PB14, PB15, PB13},
  {PB4, PB3, PB5},
  {PA7, PC4, PB0}
};

HardwareSerial console(PB11, PB10);
constexpr uint32_t pulseMs = 500;
constexpr uint8_t testDuty = 40;
uint32_t startedAt = 0;
bool running = false;
bool pwmReady = true;

void stopMotors() {
  for (const auto &motor : motors) {
    digitalWrite(motor.forward, LOW);
    digitalWrite(motor.reverse, LOW);
    analogWrite(motor.pwm, 0);
  }
  running = false;
}

void setup() {
  for (const auto &motor : motors) {
    digitalWrite(motor.forward, LOW);
    digitalWrite(motor.reverse, LOW);
    pinMode(motor.forward, OUTPUT);
    pinMode(motor.reverse, OUTPUT);
    digitalWrite(motor.pwm, LOW);
    pinMode(motor.pwm, OUTPUT);
    pwmReady = pwmReady && digitalPinHasPWM(motor.pwm);
  }
  analogWriteResolution(8);
  console.begin(115200);
  console.println("Motor test: a/b/c/d forward, A/B/C/D reverse, s stop");
  if (!pwmReady) {
    console.println("PWM pin mapping unavailable: select the generic STM32G431CBU board");
  }
}

void loop() {
  if (running && millis() - startedAt >= pulseMs) {
    stopMotors();
    console.println("Stopped");
  }
  if (!console.available()) {
    return;
  }
  const char command = console.read();
  if (command == '\r' || command == '\n') {
    return;
  }
  if (command == 's' || command == 'S') {
    stopMotors();
    return;
  }
  const bool forward = command >= 'a' && command <= 'd';
  const bool reverse = command >= 'A' && command <= 'D';
  if ((!forward && !reverse) || !pwmReady) {
    stopMotors();
    console.println("Invalid command or unavailable PWM");
    return;
  }
  if (running) {
    console.println("Wait for stop before another test");
    return;
  }
  const uint8_t index = forward ? command - 'a' : command - 'A';
  stopMotors();
  delay(100);
  digitalWrite(forward ? motors[index].forward : motors[index].reverse, HIGH);
  analogWrite(motors[index].pwm, testDuty);
  startedAt = millis();
  running = true;
  console.println("Testing for 500 ms");
}
