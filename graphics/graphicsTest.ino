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

#define WIDTH 480
#define HEIGHT 320

const byte rxPin = 10;
const byte txPin = 11;
SoftwareSerial mySerial (rxPin, txPin);

char input;

void setup(void)
{
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(3);
    tft.fillScreen(WHITE);
    Serial.begin(9600);
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(9600);
    delay(100);
}

void circle() {
  // tft.drawLine(random(0, 480), random(0, 320), random(0, 480), random(0, 320), BLACK);
  // tft.drawTriangle(random(0, 480), random(0, 320), random(0, 480), random(0, 320), random(0, 480), random(0, 320), BLACK);
  int rad = random(0, 200);
  int x = random(0, 480);
  int y = random(0, 320);
  tft.drawCircle(x, y, rad, BLACK);
  for (int i = rad; i > 1; i-=4) {
    tft.drawCircle(x, y, i, BLACK);
  }
}

void uncircle() {
  int rad = random(0, 200);
  int x = random(0, 480);
  int y = random(0, 320);
  tft.drawCircle(x, y, rad, WHITE);
  for (int i = rad; i > 1; i-=4) {
    tft.drawCircle(x, y, i, WHITE);
  }
}

byte empty[4][6] = {{1, 1, 1, 1, 1, 1},{1, 1, 1, 1, 1, 1},{1, 1, 1, 1, 1, 1},{1, 1, 1, 1, 1, 0}};
void loop(void)
{
  byte keys[4][6];
  
  if(Serial.available() > 0){
      input = Serial.read();
      if (input == 'o') {
        for (int i = 0; i < 10; i--) {
          circle();
          uncircle();
        }
      }
      else if (input == 'c') {
        tft.fillScreen(WHITE);
      }
      else {
        Serial.println("Bad input");
      }
  }
  else if (mySerial.available() > 0) {
    mySerial.readBytes((uint8_t*)keys, sizeof(keys));
    if (0 == memcmp(keys, empty, sizeof(empty))) {
      circle();
    }
    // Print the data to the Serial monitor
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 4; j++) {
        Serial.print(keys[j][i]);
        Serial.print(" ");
      }
      Serial.println();
    }
    Serial.println();
  }
  
}
