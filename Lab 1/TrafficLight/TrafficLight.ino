// TrafficLight.ino
// Joseph Taylor, Elizabeth Brown, Cody Bellec
// this code incorporates components of the Elegoo sample code, found at www.elegoo.com

// revision history:
// 9/8/21 jct - file init

// pin declarations
int button = 2;  // button; other end to +5V
int red = 3;     // red LED; through a 220ohm to GND
int yellow = 4;  // yellow LED; through a 220ohm to GND
int green = 5;   // green LED; through a 220ohm to GND
int buzzer = 13; // active buzzer; positive to pin 13, negative to GND
int data = 8;    // 74HC595 pin 8 DS
int latch = 9;   // 74HC595 pin 9 STCP
int clock = 10;  // 74HC595 pin 10 SHCP

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
void loop() {
  if (digitalRead(button)) {
    digitalWrite(buzzer, HIGH);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
  delay(1000);
  setLight(green);
  delay(1000);
  setLight(yellow);
  delay(1000);
  setLight(red);

  // setLight, button, and buzzer all work as intended
  // need to wire up 7-segment and make sure samples below work
  // also need to look up how the heck interrupts work on arduino
  
  /*
  Display(1);
  delay(500);
  Display(2);
  delay(500);
  Display(3);
  delay(500);
  Display(4);
  delay(500);
  Display(5);
  delay(500);
  Display(6);
  delay(500);
  Display(7);
  delay(500);
  Display(8);
  delay(500);
  Display(9);
  delay(500);
  Display(10);
  delay(500);
  Display(11);
  delay(500);
  Display(12);
  delay(500);
  Display(13);
  delay(500);
  Display(14);
  delay(500);
  Display(15);
  delay(500);
  */
}
