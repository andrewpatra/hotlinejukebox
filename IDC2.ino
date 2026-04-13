#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "Adafruit_Keypad.h"

#define KEYPAD_PID1824
#define R1    2
#define R2    3
#define R3    4
#define R4    5
#define C1    8
#define C2    9
#define C3    10
#define C4    11

#include "keypad_config.h"
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))  // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/6, /*tx =*/7);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
const int ledPin = 13;
int currentTrack = -1;

long dialNum = 0;      // Stores the current built number
int digitCount = 0;    // Tracks how many digits have been pressed

void setup() {
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
#else
  FPSerial.begin(9600);
#endif

  Serial.begin(115200);

  Serial.println();
  Serial.println(F("IDC1 Tuner starting..."));

  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println(F("...Signal Recieving..."));

  myDFPlayer.setTimeOut(500);  //Set serial communictaion time out 500ms

  myDFPlayer.volume(15);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  //----Read imformation----
  Serial.println(myDFPlayer.readState());                //read mp3 state
  Serial.println(myDFPlayer.readVolume());               //read current volume
  Serial.println(myDFPlayer.readEQ());                   //read EQ setting
  Serial.println(myDFPlayer.readFileCounts());           //read all file counts in SD card
  Serial.println(myDFPlayer.readCurrentFileNumber());    //read current play file number
  Serial.println(myDFPlayer.readFileCountsInFolder(3));  //read file counts in folder SD:/03

  pinMode(ledPin, OUTPUT);

  customKeypad.begin();

}

void loop() {

  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    
    // Only process when the key is first pressed
    if(e.bit.EVENT == KEY_JUST_PRESSED) {
      char key = (char)e.bit.KEY;
      myDFPlayer.playMp3Folder(1);
      Serial.print(key);
      Serial.println(" pressed");

      // Check if the key is a number (0-9)
      if (key >= '0' && key <= '9') {
        int val = key - '0'; // Convert char to actual integer
        
        // Build the number: shift existing digits left and add new one
        dialNum = (dialNum * 10) + val;
        digitCount++;

        Serial.print("Current dialNum: ");
        Serial.println(dialNum);
      }

      if (digitCount >= 6) {
        myDFPlayer.playMp3Folder(2);
        dialNum = 0;
        digitCount = 0; 
      }

      if (key == '*') {
        dialNum = 0;
        digitCount = 0;
      }
    }
  }

  if (digitCount == 5 && dialNum == 95378) {
    if (currentTrack != 3) { 
      myDFPlayer.playMp3Folder(3);
      currentTrack = 3;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 24512) {
    if (currentTrack != 4) { 
      myDFPlayer.playMp3Folder(4);
      currentTrack = 4;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 54343) {
    if (currentTrack != 5) { 
      myDFPlayer.playMp3Folder(5);
      currentTrack = 5;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 48368) {
    if (currentTrack != 6) { 
      myDFPlayer.playMp3Folder(6);
      currentTrack = 6;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 87465) {
    if (currentTrack != 7) { 
      myDFPlayer.playMp3Folder(7);
      currentTrack = 7;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 22947) {
    if (currentTrack != 8) { 
      myDFPlayer.playMp3Folder(8);
      currentTrack = 8;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 41287) {
    if (currentTrack != 9) { 
      myDFPlayer.playMp3Folder(9);
      currentTrack = 9;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 10554) {
    if (currentTrack != 10) { 
      myDFPlayer.playMp3Folder(10);
      currentTrack = 10;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 39568) {
    if (currentTrack != 11) { 
      myDFPlayer.playMp3Folder(11);
      currentTrack = 11;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 32485) {
    if (currentTrack != 12) { 
      myDFPlayer.playMp3Folder(12);
      currentTrack = 12;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (digitCount == 5 && dialNum == 00067) {
    if (currentTrack != 13) { 
      myDFPlayer.playMp3Folder(13);
      currentTrack = 13;
    }
    dialNum = 0;
    digitCount = 0; 
  }

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
    digitalWrite(ledPin, HIGH);
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}