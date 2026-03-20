// ===== PW3 mouse basic control code =====

// Analog inputs
const int leftSensorPin  = A1;   // left whisker signal
const int rightSensorPin = A3;   // right whisker signal

// Motor outputs
const int leftMotorPin   = 3;    // D3
const int rightMotorPin  = 6;    // D6

// Tunable values
int baseSpeed = 150;      // normal forward speed (0-255)
float Kp = 0.25;          // proportional steering gain
int deadband = 15;        // ignores tiny differences between sensors
int minSpeed = 0;         // don't let PWM go below this
int maxSpeed = 255;       // don't let PWM go above this

void setup() {
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);

  Serial.begin(115200);
}

void loop() {
  // 1. Read both sensor voltages
  int leftValue = analogRead(leftSensorPin);
  int rightValue = analogRead(rightSensorPin);

  // 2. Compute error
  int error = leftValue - rightValue;

  // 3. Ignore tiny differences to stop twitching
  if (abs(error) < deadband) {
    error = 0;
  }

  // 4. Convert error into steering correction
  int correction = (int)(Kp * error);

  // 5. Adjust motor speeds
  int leftSpeed  = baseSpeed - correction;
  int rightSpeed = baseSpeed + correction;

  // 6. Clamp speeds to safe PWM range
  if (leftSpeed < minSpeed) leftSpeed = minSpeed;
  if (leftSpeed > maxSpeed) leftSpeed = maxSpeed;

  if (rightSpeed < minSpeed) rightSpeed = minSpeed;
  if (rightSpeed > maxSpeed) rightSpeed = maxSpeed;

  // 7. Output PWM to motors
  analogWrite(leftMotorPin, leftSpeed);
  analogWrite(rightMotorPin, rightSpeed);

  // 8. Print values for debugging
  Serial.print("L: ");
  Serial.print(leftValue);
  Serial.print("   R: ");
  Serial.print(rightValue);
  Serial.print("   Err: ");
  Serial.print(error);
  Serial.print("   LS: ");
  Serial.print(leftSpeed);
  Serial.print("   RS: ");
  Serial.println(rightSpeed);

  delay(10);
}