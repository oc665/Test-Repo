// ===== PW3 mouse advanced refined control code =====

const int leftSensorPin  = A1;
const int rightSensorPin = A3;
const int leftMotorPin   = 3;
const int rightMotorPin  = 6;

// Tunable values
int baseSpeed = 150;
float Kp = 0.6;           // Slightly higher gain for tighter response
int deadband = 10;        // Reduced deadband for more precision
int minSpeed = 0;
int maxSpeed = 255;

// BUG FIX: Variable to store ambient light offset
int leftOffset = 0;
int rightOffset = 0;

void setup() {
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
  Serial.begin(115200);

  // FIX: SENSOR CALIBRATION
  // Sensors are rarely identical. We take a few readings at startup 
  // to find the "zero" point while the robot is stationary.
  delay(500); // Wait for the user to let go
  for(int i = 0; i < 10; i++) {
    leftOffset += analogRead(leftSensorPin);
    rightOffset += analogRead(rightSensorPin);
    delay(20);
  }
  leftOffset /= 10;
  rightOffset /= 10;
}

void loop() {
  // 1. Read sensors and subtract offsets to normalize them
  // FIX: Normalization ensures the robot drives straight when sensors see the same light
  int leftValue = analogRead(leftSensorPin) - leftOffset;
  int rightValue = analogRead(rightSensorPin) - rightOffset;

  // 2. Compute error
  int error = rightValue - leftValue;

  // 3. Apply deadband
  if (abs(error) < deadband) {
    error = 0;
  }

  // 4. Calculate correction
  float correction = Kp * error;

  // 5. Adjust motor speeds
  // BUG FIX: Pivot Logic. By allowing subtraction to go negative, 
  // the robot can spin in place for sharp corners.
  int leftSpeed  = baseSpeed - (int)correction;
  int rightSpeed = baseSpeed + (int)correction;

  // 6. Constrain speeds
  // REFINEMENT: Ensure we don't exceed the 0-255 limit for analogWrite
  leftSpeed  = constrain(leftSpeed, minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  // 7. Output PWM
  analogWrite(leftMotorPin, leftSpeed);
  analogWrite(rightMotorPin, rightSpeed);

  // 8. Debugging
  Serial.print("Err:"); Serial.print(error);
  Serial.print(",L_Spd:"); Serial.print(leftSpeed);
  Serial.print(",R_Spd:"); Serial.println(rightSpeed);

  delay(15); 
}