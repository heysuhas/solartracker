#include <Servo.h>

#define NUM_LDRS 4

const int LDR_pins[NUM_LDRS] = {A0, A1, A2, A3};
const int error_threshold = 10;

int servo_position = 90;
Servo solar_panel_servo;

void setup() {
  solar_panel_servo.attach(11);
  solar_panel_servo.write(servo_position);
  delay(1000);
}

void loop() {
  int ldr_readings[NUM_LDRS];

  // Read values from all LDRs
  for (int i = 0; i < NUM_LDRS; ++i) {
    ldr_readings[i] = analogRead(LDR_pins[i]);
  }

  // Check if all LDR readings are within the error threshold
  if (areReadingsWithinThreshold(ldr_readings)) {
    // Do nothing if the difference is within the error threshold for all LDRs
  } else {
    int average = calculateAverage(ldr_readings);

    // Adjust the servo position based on the average LDR reading
    adjustServoPosition(average);
  }

  // Move the servo to the new position
  solar_panel_servo.write(servo_position);
  delay(80);
}

bool areReadingsWithinThreshold(int readings[]) {
  for (int i = 0; i < NUM_LDRS - 1; ++i) {
    for (int j = i + 1; j < NUM_LDRS; ++j) {
      if (abs(readings[i] - readings[j]) > error_threshold) {
        return false;
      }
    }
  }
  return true;
}

int calculateAverage(int readings[]) {
  int sum = 0;
  for (int i = 0; i < NUM_LDRS; ++i) {
    sum += readings[i];
  }
  return sum / NUM_LDRS;
}

void adjustServoPosition(int target_position) {
  const int servo_speed = 1;

  if (target_position > servo_position) {
    servo_position = min(180, servo_position + servo_speed);
  } else if (target_position < servo_position) {
    servo_position = max(0, servo_position - servo_speed);
  }
}
