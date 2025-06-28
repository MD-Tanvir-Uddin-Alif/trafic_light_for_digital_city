
// libraries
#include <Arduino.h>
#include <TM1637Display.h>

// Digital Pins
#define CLK 2
#define DIO 3

// button
int buttonpin = 4;
bool buttonstate = true;

// delay defined
#define TEST_DELAY 3000
#define count_delay 1000

// Define segment mappings for characters 'S', 'T', 'O', 'P', 'G'
const uint8_t SEG_S[] = {
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
};
const uint8_t SEG_T[] = {
  SEG_A | SEG_E | SEG_F,
};

const uint8_t SEG_O[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
};

const uint8_t SEG_P[] = {
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,
};

const uint8_t SEG_G0[] = {
  SEG_A | SEG_D | SEG_C | SEG_E | SEG_F,
};

// countdown segments
const uint8_t SEGMENTS[10] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,          // 0
  SEG_B | SEG_C,                                          // 1
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,                  // 2
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,                  // 3
  SEG_B | SEG_C | SEG_F | SEG_G,                          // 4
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                  // 5
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,          // 6
  SEG_A | SEG_B | SEG_C,                                  // 7
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,  // 8
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G           // 9

};

// Create an object of TM1637Display class
TM1637Display display(CLK, DIO);

//11 - red
//12- yellow
//13- green

bool buttonPressed = false;  // State flag to track button press

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  display.setBrightness(0x0f);
  pinMode(buttonpin, INPUT);
}

// Display the word on the 4-digit display
void displayWord(uint8_t word[4]) {
  display.setSegments(word);
}

// countdown function
void displayNumber(int num) {
  uint8_t digits[4] = { 0, 0, 0, 0 };

  if (num < 100) {
    digits[2] = SEGMENTS[num - num];
    digits[3] = SEGMENTS[num];
  }

  display.setSegments(digits);
}

// Button Loop (Activated when button is pressed)
void buttonActivatedLoop() {
// Set all segments to off
  uint8_t off[] = { 0x00, 0x00, 0x00, 0x00 };

//set yellow high
  digitalWrite(12, HIGH);

  // countdown
  for (int i = 8; i >= 0; i--) {
    displayNumber(i);
    delay(count_delay);
  }

  // LED Blinking effect
  for (int i = 0; i < 3; i++) {
    digitalWrite(12, LOW);
    delay(500);
    digitalWrite(12, HIGH);
    delay(500);
  }

  // Display "GO"
  uint8_t go[] = { 0, SEG_G0[0], SEG_O[0], 0 };  // 'G', 'O', blank, blank
  displayWord(go);

  digitalWrite(13, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  delay(5000);

  //light blinking effect for green
  for (int i = 0; i < 3; i++) {
    digitalWrite(11, LOW);
    delay(500);
    digitalWrite(11, HIGH);
    delay(500);
  }

  display.setSegments(off);

  //set yellow hight
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  delay(2000);

  //light binlikng effect for yellow
  for (int i = 0; i < 3; i++) {
    digitalWrite(12, LOW);
    delay(500);
    digitalWrite(12, HIGH);
    delay(500);
  }
  // Button loop is done, set the flag back to false to return to automatic loop
  buttonPressed = false;
}

// Main Automatic Loop
void automaticLoop() {

  uint8_t off[] = { 0x00, 0x00, 0x00, 0x00 };  // Set all segments to off

  // Display "STOP"
  uint8_t stop[] = { SEG_S[0], SEG_T[0], SEG_O[0], SEG_P[0] };  // 'S', 'T', 'O', 'P'
  displayWord(stop);

  // Turn on the red LED
  digitalWrite(13, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  delay(5000);

  // LED Blinking effect for stop phase
  for (int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }

  // Check the button state to transition to the button loop during automaticLoop
  buttonstate = digitalRead(buttonpin);        // Read the button state
  if (buttonstate == LOW && !buttonPressed) {  // Button pressed, not in button loop
    buttonPressed = true;                      // Set the flag
    buttonActivatedLoop();                     // Enter button loop
  }

  display.setSegments(off);

  // set yellow high
  digitalWrite(13, LOW);   // Red LED OFF
  digitalWrite(11, LOW);   // Green LED OFF
  digitalWrite(12, HIGH);  // Yellow LED ON
  delay(2000);

  // Countdown again
  for (int i = 4; i >= 0; i--) {
    displayNumber(i);
    delay(count_delay);
  }

  // LED Blinking effect for waiting phase
  for (int i = 0; i < 3; i++) {
    digitalWrite(12, LOW);
    delay(500);
    digitalWrite(12, HIGH);
    delay(500);
  }

  // Check the button state again to transition to the button loop during automaticLoop
  buttonstate = digitalRead(buttonpin);        // Read the button state
  if (buttonstate == LOW && !buttonPressed) {  // Button pressed, not in button loop
    buttonPressed = true;                      // Set the flag
    buttonActivatedLoop();                     // Enter button loop
  }

  // Display "GO"
  uint8_t go[] = { 0, SEG_G0[0], SEG_O[0], 0 };  // 'G', 'O', blank, blank
  displayWord(go);

  digitalWrite(13, LOW);   // Red LED OFF
  digitalWrite(11, HIGH);  // Green LED ON
  digitalWrite(12, LOW);   // Yellow LED OFF
  delay(5000);

  // // Countdown again
  // for (int i = 4; i >= 0; i--) {
  //   displayNumber(i);
  //   delay(count_delay);
  // }

  // LED Blinking effect for GO phase
  for (int i = 0; i < 3; i++) {
    digitalWrite(11, LOW);
    delay(500);
    digitalWrite(11, HIGH);
    delay(500);
  }

  display.setSegments(off);
  
  //set yellow high
  digitalWrite(13, LOW);   // Red LED OFF
  digitalWrite(11, LOW);   // Green LED OFF
  digitalWrite(12, HIGH);  // Yellow LED ON
  delay(2000);

  // // Countdown
  // for (int i = 4; i >= 0; i--) {
  //   displayNumber(i);
  //   delay(count_delay);
  // }

  // LED Blinking effect for waiting phase
  for (int i = 0; i < 3; i++) {
    digitalWrite(12, LOW);
    delay(500);
    digitalWrite(12, HIGH);
    delay(500);
  }

  // Check the button state again to transition to the button loop during automaticLoop
  buttonstate = digitalRead(buttonpin);        // Read the button state
  if (buttonstate == LOW && !buttonPressed) {  // Button pressed, not in button loop
    buttonPressed = true;                      // Set the flag
    buttonActivatedLoop();                     // Enter button loop
  }
}

void loop() {
  // Perform automatic functionality
  automaticLoop();  // Keep the loop running
}
