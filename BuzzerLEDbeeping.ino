#include <Arduino_LSM6DS3.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define LED 2
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUZZER_PIN 21 // Define the buzzer pin
#define TEMP_THRESHOLD 30.0 // Define the temperature threshold

void beepBuzzerAndBlinkLED(int duration, int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED, LOW);
    delay(duration);
  }
}

void setup() {
  Serial.begin(9600);

  // Initialize the buzzer and LED pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially
  digitalWrite(LED, LOW);       // Ensure LED is off initially

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Confirm address
    Serial.println("SSD1306 allocation failed");
    while (1); // Halt execution if OLED initialization fails
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Ensure text is visible
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Initializing...");
  display.display();
  delay(2000);

  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("IMU Init Failed!");
    display.display();
    while (1); // Halt execution if IMU initialization fails
  }

  Serial.print("Temperature sensor sample rate = ");
  Serial.print(IMU.temperatureSampleRate());
  Serial.println(" Hz");
}

void loop() {
  float t;

  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(t);

    // Display temperature on OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Temperature:");
    display.setTextSize(2);
    display.setCursor(0, 20); // Adjust position for larger text
    display.print(t);
    display.print((char)247); // Degree symbol
    display.print("C");
    display.display();

    // Debug print to Serial Monitor
    Serial.print("Temperature: ");
    Serial.println(t);

    // Check if temperature exceeds threshold
    if (t > TEMP_THRESHOLD) {
      beepBuzzerAndBlinkLED(100, 3); // Beep "Ti Ti Ti" and blink LED
    } else {
      digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off
      digitalWrite(LED, LOW);       // Ensure LED is off
    }
  }

  delay(500);
}
