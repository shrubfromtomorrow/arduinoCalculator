// #include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <SoftwareSerial.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410

// Screen width and height
#define WIDTH 480
#define HEIGHT 320

const int cursorTick = 750; // Tickrate for cursor flash, once every 750 milliseconds
long previousCursorMillis = 0;        // will store last time keyboard was checked

// Defining digital pins to use for a software serial connection to keyboard
const byte rxPin = 10;
const byte txPin = 11;
SoftwareSerial mySerial (rxPin, txPin);

byte keys[4][6]; // Keymap array, conatins the digital voltage values of each key

const byte empty[4][6] = {{1, 1, 1, 1, 1, 1},{1, 1, 1, 1, 1, 1},{1, 1, 1, 1, 1, 1},{1, 1, 1, 1, 1, 0}};

// Draw the interface for the regular calculator
void drawCalcInterface(void) {
  tft.drawFastHLine(0, HEIGHT * (7.0 / 8.0), 480, BLACK);
}

int cursorReps = 0; // Keep track of how many times the cursor has flashed

// If cursor reps is even, draw the cursor, if it is odd, draw over the cursor
void drawCursor(void) {
  if (cursorReps % 2 == 0) {
    tft.drawFastVLine(20, HEIGHT * (7.0 / 8.0) + 8, HEIGHT * (1.0 / 8.0) - 16, BLACK);
  }
  else {
    tft.drawFastVLine(20, HEIGHT * (7.0 / 8.0) + 8, HEIGHT * (1.0 / 8.0) - 16, WHITE);    
  }
  cursorReps++;
}

// Refresh the screen
void refresh(void) {
  tft.fillScreen(BLACK);
  tft.fillScreen(WHITE);
}


void setup(void)
{
    uint16_t ID = tft.readID(); // Get ID of the display for MCUfriend
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(3); // Set rotation to some value, this is one of the two landscape modes
    tft.fillScreen(WHITE);
    Serial.begin(9600); // Begin regular serial connection to laptop with TX and RX, pins 0 and 1

    // Define pin modes for TX and RX for software serial connections
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    mySerial.begin(9600); // Set the baud rate for the SoftwareSerial object
    drawCalcInterface(); // Draw the main calc interface
}

void loop(void){
  unsigned long currentMillis = millis(); // How many milliseconds calc has been running

  // Check if the number of milliseconds elapsed is more than a cursor tick, draw cursor if it is
  if(currentMillis - previousCursorMillis >= cursorTick) {
    drawCursor();
    previousCursorMillis = currentMillis;
  }
  
  // Check if there is any incoming serial data from software serial to keyboard
  if (mySerial.available() > 0) {
    mySerial.readBytes((uint8_t*)keys, sizeof(keys)); // Readbytes of serial into keys array
    if (0 == memcmp(keys, empty, sizeof(empty))) { // Check if keys array matches some designated key array
      refresh();
      drawCalcInterface();
    }
  }
  
}

// Print the data to the Serial monitor
    // for (int i = 0; i < 6; i++) {
    //   for (int j = 0; j < 4; j++) {
    //     Serial.print(keys[j][i]);
    //     Serial.print(" ");
    //   }
    //   Serial.println();
    // }
    // Serial.println();
