// TrafficLight.ino
// Joseph Taylor, Elizabeth Brown, Cody Bellec
// this code incorporates components of the Elegoo sample code, found at www.elegoo.com
// also incorporates components of https://www.circuitbasics.com/arduino-7-segment-display-tutorial/

// revision history:
// 9/8/21 jct - file init

// pin declarations
int button = 2;  // button; other end to +5V
int red = 3;     // red LED; through a 220ohm to GND
int yellow = 4;  // yellow LED; through a 220ohm to GND
int green = 5;   // green LED; through a 220ohm to GND
int buzzer = 13; // active buzzer; positive to pin 13, negative to GND

// pin declarations for 7-segment display
int data = 6;    // 74HC595 pin 6 DS
int latch = 7;   // 74HC595 pin 7 STCP
int clock = 8;   // 74HC595 pin 8 SHCP
int dig1 = 9;    // cathode for digit 1
int dig2 = 10;   // cathode for digit 2

unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);
  pinMode(dig1,OUTPUT);
  pinMode(dig2,OUTPUT);
}

void Display(unsigned char num)
{
  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch,HIGH);
}

void setLight(int color) {
  digitalWrite(red, color == red);
  digitalWrite(yellow, color == yellow);
  digitalWrite(green, color == green);
}

// the code below works, but the ELEGOO kit only provides code that displays all 8 segments at the same time.

void loop() {
  if (digitalRead(button)) {
    digitalWrite(buzzer, HIGH);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
  setLight(green);
  delay(500);
  setLight(yellow);
  delay(500);
  setLight(red);
  delay(500);
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, LOW);
  for (int i=0; i<10; i++) {
    Display(i);
    delay(500);
  }
  digitalWrite(dig1, LOW);
  digitalWrite(dig2, HIGH);
  for (int i=0; i<10; i++) {
    Display(i);
    delay(500);
  }
}
