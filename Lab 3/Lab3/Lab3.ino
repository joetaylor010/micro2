#include<Wire.h>
#include "RTClib.h" // note: need to copy RTClib/ to "Arduino/libraries/"
RTC_DS1307 rtc; // establish ds1307 object

// next line only needed is days of week end up utilized
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// pin declarations
const int buttonPin = 2;

// variable declarations
bool buttonFlag = false;
bool timerFlag = false;

// ISR's for button and timer
ISR(TIMER1_COMPA_vect) { timerFlag = true; }
void buttonPress() { buttonFlag = true; }

void setup(){
  Serial.begin(9600);
  Wire.begin(); // establish I2C bus
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // only needed once
  
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPress, RISING);

  cli(); // disable interrupts

  // set timer1 interrupt at 1Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  OCR1A = 15624; // frequency set, = (16*10^6) / (1hz*1024) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  
  sei(); // enable interrupts
}

void loop() {
  if (buttonFlag) {
    // reverse direction
    Serial.write("Button pressed");
    while (digitalRead(buttonPin)) delay(10); // wait for button release
    buttonFlag = false;
  }

  if (timerFlag) {
    DateTime now = rtc.now();
    /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    */
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    timerFlag = false;
  }
  delay(10);
}
