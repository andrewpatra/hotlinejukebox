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
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
const int ledPin = 13;
int currentTrack = -1;

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
    Serial.print((char)e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }
  
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(50);  // delay in between reads for stability

  if (sensorValue < 20) {
    myDFPlayer.pause();
  }

  else if (sensorValue > 50 && sensorValue < 90) {
    if (currentTrack != 1) { 
      myDFPlayer.playMp3Folder(1);
      currentTrack = 1;
    } 
  }

  else if (sensorValue > 120 && sensorValue < 160) {
    if (currentTrack != 6) { 
      myDFPlayer.playMp3Folder(6);
      currentTrack = 6;
    }
  }

  else if (sensorValue > 200 && sensorValue < 240) {
     if (currentTrack != 2) { 
      myDFPlayer.playMp3Folder(2);
      currentTrack = 2;
    }
  }

  else if (sensorValue > 280 && sensorValue < 320) {
     if (currentTrack != 3) { 
      myDFPlayer.playMp3Folder(3);
      currentTrack = 3;
    }
  }

   else if (sensorValue > 360 && sensorValue < 400) {
     if (currentTrack != 4) { 
      myDFPlayer.playMp3Folder(4);
      currentTrack = 4;
    }
  }

   else if (sensorValue > 440 && sensorValue < 480) {
     if (currentTrack != 5) { 
      myDFPlayer.playMp3Folder(5);
      currentTrack = 5;
    }
  }

   else if (sensorValue > 520 && sensorValue < 560) {
     if (currentTrack != 7) { 
      myDFPlayer.playMp3Folder(7);
      currentTrack = 7;
    }
  }

   else if (sensorValue > 600 && sensorValue < 640) {
     if (currentTrack != 8) { 
      myDFPlayer.playMp3Folder(8);
      currentTrack = 8;
    }
  }

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());  //Print the detail message from DFPlayer to handle different errors and states.
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