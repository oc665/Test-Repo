// ===== PW3 mouse refined control code =====

// Analog inputs
const int leftSensorPin  = A1;   // left whisker signal
const int rightSensorPin = A3;   // right whisker signal

// Motor outputs
const int leftMotorPin   = 3;    // D3
const int rightMotorPin  = 6;    // D6

// Tunable values
int baseSpeed = 150;      // normal forward speed (0-255)
float Kp = 0.5;           // BUG FIX: Increased gain for better responsiveness
int deadband = 15;        // ignores tiny differences between sensors
int minSpeed = 0;         
int maxSpeed = 255;       

void setup() {
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  // Note: analogRead pins don't strictly need pinMode(INPUT), 
  // but it doesn't hurt.
  Serial.begin(115200);
}

void loop() {
  // 1. Read both sensor voltages
  int leftValue = analogRead(leftSensorPin);
  int rightValue = analogRead(rightSensorPin);

  // 2. Compute error (Right - Left)
  // LOGIC FIX: Flipping this to (right - left) ensures that if the right 
  // sensor sees more light/wall, the 'correction' helps turn away from it correctly.
  int error = rightValue - leftValue;

  // 3. Ignore tiny differences to stop twitching
  if (abs(error) < deadband) {
    error = 0;
  }

  // 4. Convert error into steering correction
  float correction = Kp * error;

  // 5. Adjust motor speeds
  // LOGIC FIX: To turn LEFT, you slow down the Left motor and speed up the Right.
  int leftSpeed  = baseSpeed - (int)correction;
  int rightSpeed = baseSpeed + (int)correction;

  // 6. Clamp speeds using the built-in constrain() function
  // REFINEMENT: This is cleaner and prevents "variable creep" bugs.
  leftSpeed  = constrain(leftSpeed, minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  // 7. Output PWM to motors
  analogWrite(leftMotorPin, leftSpeed);
  analogWrite(rightMotorPin, rightSpeed);

  // 8. Print values for debugging (formatted for Serial Plotter)
  Serial.print("L_Val:"); Serial.print(leftValue);
  Serial.print(","); Serial.print("R_Val:"); Serial.print(rightValue);
  Serial.print(","); Serial.print("L_Spd:"); Serial.print(leftSpeed);
  Serial.print(","); Serial.print("R_Spd:"); Serial.println(rightSpeed);

  delay(20); // Slightly longer delay for stability
}