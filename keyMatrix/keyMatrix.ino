
#include <SoftwareSerial.h>

// Defining digital pins to use for a software serial connection to calc arduino
const byte rxPin = 15;
const byte txPin = 14;
SoftwareSerial mySerial (rxPin, txPin);

// Define the pins that connect to each column and row (physical pins)
byte rows[] = {10,16,9,4,5,3};
byte cols[] = {2,6,7,8};
const int rowCount = sizeof(rows)/sizeof(rows[0]);
const int colCount = sizeof(cols)/sizeof(cols[0]);

// Keys to keep track of current input, lastInput to compare with keys to check for repeats
byte keys[colCount][rowCount];
byte lastInput[colCount][rowCount];

long previousKeyboardMillis = 0;        // will store last time keyboard was checked
int keyboardInterval = 1;           // interval at which to check keyboard (Milliseconds)

void setup() {
  // Defining digital pins to use for a software serial connection to keyboard
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // Set the baud rate for software serial and regular serial
  mySerial.begin(9600);
  // Serial.begin(9600);
}

void readMatrix() {
  // Iterate through each row
  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
    byte curRow = rows[rowIndex]; // Keep track of current row
    pinMode(curRow, OUTPUT); // Set the curent row to output mode
    digitalWrite(curRow, LOW); // Pull the curren row's voltage low

    // Iterate through each column attached to each row
    for (int colIndex = 0; colIndex<colCount; colIndex++) {
      byte colRow = cols[colIndex]; // Keep track of current column
      pinMode(colRow, INPUT_PULLUP); // Set column high but resist more voltage coming through
      keys[colIndex][rowIndex] = digitalRead(colRow); // Check if column goes low, effectively, does the drop in the row cause there to be a voltage drop in a column?
      pinMode(colRow, INPUT); // Set column back to regular voltage input mode
    }
    pinMode(curRow, INPUT); // Set row back to regular voltage input mode
  }
}

unsigned long latestMillis = 0;

void sendMatrix(unsigned long currentMillis) {
  unsigned long elapsedMillis = currentMillis - latestMillis;
  // Iterate through each row and columns
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 4; j++) {
      // If any of the bytes in the keys array is zero (pulled low) ...
      if (keys[j][i] == 0) {
        // Check to see if that last input is the same as current input, if it is, don't send
        if (memcmp(keys, lastInput, sizeof(lastInput)) != 0) {
          // Serial.println("Diff");
          int totalBytes = sizeof(keys); // Total size of keys array
          // Send the bytes over 
          mySerial.write((uint8_t*)keys, totalBytes);
          memcpy(lastInput, keys, sizeof(keys));
          latestMillis = currentMillis;
          break;
        }
        else if (0 == memcmp(keys, lastInput, sizeof(lastInput)) && elapsedMillis >= 200) {
          // Serial.println("Same");
          int totalBytes = sizeof(keys); // Total size of keys array
          // Send the bytes over 
          mySerial.write((uint8_t*)keys, totalBytes);
          memcpy(lastInput, keys, sizeof(keys));
          latestMillis = currentMillis;
          break;
        }
        else {
          break;          
        }
      }
    }
  }  
}

void loop() {
	readMatrix();
  unsigned long currentMillis = millis();
  unsigned long elapsed = currentMillis - previousKeyboardMillis;
  if(elapsed > keyboardInterval) { 
    previousKeyboardMillis = currentMillis;   

    sendMatrix(currentMillis);
  }
}

