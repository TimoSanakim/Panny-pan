#include <SD.h>
#include <TMRpcm.h>

#define LED1 5
#define LED2 6
#define LED3 7

const int buttonPin = 3; // Button connected to digital pin 3
const int resetButtonPin = 4; // Second button connected to digital pin 4
int x = 0; // Initialize x to 0
int lastButtonState = LOW; // Previous state of the button
int currentButtonState; // Current state of the button
int lastResetButtonState = LOW; // Previous state of the reset button
int currentResetButtonState; // Current state of the reset button

unsigned long lastClickTime = 0; // The last time the button was clicked
unsigned long doubleClickDelay = 500; // The maximum time between clicks for a double-click
unsigned long buttonPressStartTime = 0; // The time when the button was first pressed

TMRpcm tmrpcm; // Create an object for TMRpcm

void setup() {
  // Initialize the LED pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // Initialize the serial communication:
  Serial.begin(9600);
  tmrpcm.setVolume(7);

  // Initialize the pushbutton pins as inputs:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);

  // Initialize the SD card:
  if (!SD.begin(10)) { // CS pin for the SD card module
    Serial.println("SD card initialization failed!");
    return;
  }

  tmrpcm.speakerPin = 9; // Set the speaker pin
  tmrpcm.setVolume(5); // Set the volume level (0 to 7)
}

void loop() {
  // Read the state of the pushbutton:
  currentButtonState = digitalRead(buttonPin);
  currentResetButtonState = digitalRead(resetButtonPin);

  // Check if the button is pressed (LOW) and was not pressed before:
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    buttonPressStartTime = millis(); // Record the time when the button was first pressed
  }

  // Check if the button is held down for 2 seconds:
  if (currentButtonState == LOW && (millis() - buttonPressStartTime >= 2000)) {
    x = -1; // Reset x to 0
    lightOff(); // Turn off all LEDs
  }

  // Check if the button is released (HIGH) and was pressed before:
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    if ((millis() - lastClickTime) < doubleClickDelay) {
      x -= 2; // Decrement x on double-click
      playAudio(x); // Call the function to play the audio
    } else {
      x++; // Increment x on single click
      playAudio(x); // Call the function to play the audio
    }
    lastClickTime = millis();
  }

  // Check if the reset button is pressed (HIGH) and was not pressed before:
  if (currentResetButtonState == HIGH && lastResetButtonState == LOW) {
    x = 0; // Reset x to 0
  }

  // Save the current states as the last states for the next loop iteration:
  lastButtonState = currentButtonState;
  lastResetButtonState = currentResetButtonState;
}

// Function to play specific audio files based on the value of x
void playAudio(int x) {
  Serial.println(x);
  if (x == 1) {
    lightOff();
    tmrpcm.play("a1.wav");
    Serial.println("step1");
    lightUpLEDs(); // Light up LEDs in sequence
  } else if (x == 2) {
    lightOff();
    tmrpcm.play("a2.wav");
    lightUpLEDs(); // Light up LEDs in sequence
  } else if (x == 3) {
    lightOff();
    tmrpcm.play("a3.wav");
    lightUpLEDs(); // Light up LEDs in sequence
  }
}

// Function to light up LEDs in sequence with a delay
void lightUpLEDs() {
  // Turn on LED
  delay(2000);
  digitalWrite(LED1, HIGH);
  delay(2000);
  digitalWrite(LED2, HIGH);
  delay(2000);
  digitalWrite(LED3, HIGH);
}

void lightOff() {
  // Turn off LED
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}
