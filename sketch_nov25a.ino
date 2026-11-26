#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

#define BUTTON_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* messages[] = {
  "THANK YOU",
  "I NEED HELP",
  "GOOD MORNING",
  "SORRY",
  "HOW ARE YOU"
};

const int totalMessages = 5;

int messageIndex = 0;

int buttonState = HIGH;
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void showReady() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(8, 15);
  display.println("SHOW");

  display.setCursor(8, 38);
  display.println("GESTURE");

  display.display();
}

void showStartup() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(5, 10);
  display.println("KUMAS");

  display.setTextSize(1);
  display.setCursor(5, 38);
  display.println("COMMUNICATION");

  display.display();

  delay(2500);
  showReady();
}

void showMessage(const char* msg) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(15, 5);
  display.println("GESTURE DETECTED");

  display.drawLine(0, 20, 127, 20, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 32);
  display.println(msg);

  display.display();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED NOT FOUND!");
    while (true);
  }

  showStartup();

  Serial.println("KUMAS COMMUNICATION READY");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      // Button pressed
      if (buttonState == LOW) {
        showMessage(messages[messageIndex]);

        messageIndex++;

        if (messageIndex >= totalMessages) {
          messageIndex = 0;
        }
      }
    }
  }

  lastButtonState = reading;
}