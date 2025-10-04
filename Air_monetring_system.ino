#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -----------------------------
// Fire & Gas Leakage Detection System using Arduino Uno
// Components: MQ2 Gas Sensor, Flame Sensor, Buzzer, 16x2 LCD (I2C)
// -----------------------------

// LCD Configuration
// I2C address (commonly 0x27 or 0x3F), 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
const int gasSensorPin = A0;        // Analog pin for MQ2 Gas Sensor
const int flameSensorPin = 2;       // Digital pin for Flame Sensor (DOUT)
const int buzzerPin = 5;            // Digital pin for Buzzer

// Gas detection threshold (0 - 1023). Tune based on testing.
const int gasThreshold = 400;

void setup() {
  // Configure pin modes
  pinMode(gasSensorPin, INPUT);
  pinMode(flameSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("SMOKE Alarm System Starting...");

  // Initialize LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Initializing");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read analog gas value from MQ2 (0-1023)
  int gasValue = analogRead(gasSensorPin);

  // Read digital flame status (LOW = fire detected)
  int airStatus = digitalRead(flameSensorPin);

  // Debugging output to Serial Monitor
  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.print(" | Air💨: ");
  Serial.println(airStatus == LOW ? "DETECTED" : "Safe✅");

  // Check for danger conditions: GAS leak or FIRE
  if (gasValue > gasThreshold || airStatus == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);

    // Gas leak warning
    if (gasValue > gasThreshold) {
      lcd.print("!! SMOKE LEAK !!  ");
      Serial.println("SMOKE LEAK DETECTED");
    } 
    // Fire alert
    else {
      lcd.print("** SMOKE DETECTED **");
      Serial.println("SMOKE DETECTED");
    }

    lcd.setCursor(0, 1);
    lcd.print("Evacuate Now!   ");

    // Beep buzzer repeatedly for 10 seconds
    for (int i = 0; i < 10; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(500);
      digitalWrite(buzzerPin, LOW);
      delay(500);
    }

    // Post-alert message
    lcd.setCursor(0, 0);
    lcd.print("Rechecking...   ");
    lcd.setCursor(0, 1);
    lcd.print("Please Wait...  ");
    delay(2000);
    lcd.clear();
  } 
  else {
    // Normal status - display gas level
    digitalWrite(buzzerPin, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Smoke Level: ");
    lcd.print(gasValue);
    lcd.print("   ");  // Padding for clear output

    lcd.setCursor(0, 1);
    lcd.print("System Normal   ");
    delay(1000);
  }
}
