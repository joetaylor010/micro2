// Micro 2 Lab 3
// Fall 2021
// Joseph Taylor, Elizabeth Brown, Cody Bellec
// November 2, 2021
// the following resource was used to help get the ds1307 working:
// https://create.arduino.cc/projecthub/electropeak/interfacing-ds1307-rtc-module-with-arduino-make-a-reminder-08cb61

// todo:
// verify compliance with spec
// cleanup

#include <Wire.h>
#include <IRremote.h>
#include <LiquidCrystal.h>
#include "RTClib.h" // note: need to copy RTClib/ to "Arduino/libraries/"

// pin declarations
const int buttonPin = 2;
const int receiverPin = 3;
const int motorDirA = 4;
const int motorDirB = 5;
const int motorEnable = 6;
const int RS = 7;
const int EN = 8;
const int D4 = 9;
const int D5 = 10;
const int D6 = 11;
const int D7 = 12;

RTC_DS1307 rtc; // rtc object
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); // establish lcd object

// variable declarations
bool buttonFlag = false; // used for button ISR
bool timerFlag = false; // used for timer ISR
bool fanDirection = true; // clockwise if true
bool fanAutoControl = true; // enables automatic fan control
unsigned fanSpeed = 0; // fan control; 0-3

// ISR's for button and timer
ISR(TIMER1_COMPA_vect) { timerFlag = true; }
void buttonPress() { buttonFlag = true; }

void setup(){
  Serial.begin(9600);
  Wire.begin(); // establish I2C bus
  rtc.begin(); // start RTC
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // only needed once

  IrReceiver.begin(receiverPin, 0, 0);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(receiverPin);

  lcd.begin(16, 2);

  pinMode(motorDirA, OUTPUT);
  pinMode(motorDirB, OUTPUT);
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPress, RISING);

  cli(); // disable interrupts

  // set timer1 interrupt at 1Hz
  TCCR1A = 0; // set entire Tirrecv.enableIRIn(); // Start the receiverCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  OCR1A = 15624; // frequency set, = (16*10^6) / (1hz*1024) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  
  sei(); // enable interrupts
}

void loop() {
  if (IrReceiver.decode()) {
    // Print a short summary of received data
    //IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.command == 0x45) {
      if (!fanSpeed) fanSpeed = 3; // power button
      else fanSpeed = 0;
      Serial.print("Received on/off command. fanSpeed = ");
      Serial.println(fanSpeed);
    }
    else if (IrReceiver.decodedIRData.command == 0x44) {
      fanDirection = false; // fast backward (counter-clockwise)
      Serial.print("Received CCW command. fanDirection = ");
      Serial.println(fanDirection);
    }
    else if (IrReceiver.decodedIRData.command == 0x43) {
      fanDirection = true; // fast forward (clockwise)
      Serial.print("Received CW command. fanDirection = ");
      Serial.println(fanDirection);
    }
    else if (IrReceiver.decodedIRData.command == 0x47) {
      fanAutoControl = !fanAutoControl; // func/stop (auto control / manual control)
      Serial.print("Received auto/man command. fanAutoControl = ");
      Serial.println(fanAutoControl);
    }
    else if (IrReceiver.decodedIRData.command == 0x09) {
      if (fanSpeed < 3) fanSpeed += 1; // up (speed up)
      Serial.print("Received up command. New fanSpeed = ");
      Serial.println(fanSpeed);
    }
    else if (IrReceiver.decodedIRData.command == 0x07) {
      if (fanSpeed > 0) fanSpeed -= 1; // down (slow down)
      Serial.print("Received down command. New fanSpeed = ");
      Serial.println(fanSpeed);
    }
    delay(250);
    IrReceiver.resume(); // Enable receiving of the next value
  }
  
  if (fanSpeed == 1) analogWrite(motorEnable, 127);
  else if (fanSpeed == 2) analogWrite(motorEnable, 191);
  else if (fanSpeed == 3) analogWrite(motorEnable, 255);
  else analogWrite(motorEnable, 0);
  digitalWrite(motorDirA, !fanDirection);
  digitalWrite(motorDirB, fanDirection);

  if (buttonFlag) {
    delay(100);
    if (digitalRead(buttonPin)) {
      Serial.println("Button was pressed.");
      fanDirection = !fanDirection; // reverse direction
    }
    buttonFlag = false;
  }

  if (timerFlag) {
    DateTime now = rtc.now();
    // display time on lcd
    lcd.setCursor(1, 0);
    if (now.month() < 10) lcd.print("0");
    lcd.print(now.month(), DEC);
    lcd.print('/');
    if (now.day() < 10) lcd.print("0");
    lcd.print(now.day(), DEC);
    lcd.print(" ");
    if (now.hour() < 10) lcd.print("0");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second() < 10) lcd.print("0");
    lcd.print(now.second(), DEC);
    lcd.print(" ");

    // display fan information on lcd
    lcd.setCursor(2, 1);
    if (fanDirection) lcd.print("C  ");
    else lcd.print("CC ");
    if (fanAutoControl) lcd.print("AUTO ");
    else lcd.print("MAN  ");
    if (fanSpeed == 1) lcd.print("1/2 ");
    else if (fanSpeed == 2) lcd.print("3/4 ");
    else if (fanSpeed == 3) lcd.print("Full");
    else (lcd.print("0   "));
    lcd.print("  ");

    // if at the beginning of a minute, turn fan on for 30 seconds
    if (now.second() == 0 && fanAutoControl) { fanSpeed = 3; }
    if (now.second() == 30 && fanAutoControl) { fanSpeed = 0; }
  
    timerFlag = false;
  }
  delay(10);
}
