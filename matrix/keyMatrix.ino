
#include <SoftwareSerial.h>

const byte rxPin = 15;
const byte txPin = 14;
SoftwareSerial mySerial (rxPin, txPin);

byte rows[] = {10,16,9,4,5,3};


byte cols[] = {2,6,7,8};
const int rowCount = sizeof(rows)/sizeof(rows[0]);
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];

void setup() {
  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
  Serial.begin(9600);
  delay(500);
}

void readMatrix() {

  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
    byte curRow = rows[rowIndex];
    pinMode(curRow, OUTPUT);
    digitalWrite(curRow, LOW);

    for (int colIndex = 0; colIndex<colCount; colIndex++) {
      byte colRow = cols[colIndex];
      pinMode(colRow, INPUT_PULLUP);
      keys[colIndex][rowIndex] = digitalRead(colRow);
      pinMode(colRow, INPUT);
    }
    pinMode(curRow, INPUT);
  }  
}

void loop() {
	readMatrix();
	if (Serial.read()=='!') {
		// Calculate the total number of bytes to send
    int totalBytes = sizeof(keys);
    // Send the bytes over TX
    mySerial.write((uint8_t*)keys, totalBytes);
  }
}

