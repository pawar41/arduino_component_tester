#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

const int OUT_PIN = A2;
const int IN_PIN = A0;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;
const int MAX_ADC_VALUE = 1023;

float capacitance;


#define ResistorAnalogPin 0
#define resistorValueResCalcR2 10000.0F

char x = 'D';
char y = 'E';
bool circuitTest[3] = { 0 };
float resistor_val;
float resitor_calculated;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

int latchPin = 5;  // Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6;  // Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4;   // Data pin of 74HC595 is connected to Digital pin 4

byte leds = 0;  // Variable to hold the pattern of which LEDs are currently turned on or off


void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

void writeError() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Error !!!");
}


void relayUpdate(uint8_t stat) {
  Serial.println("319 relay up");
  delay(3000);
  if (stat == 0) {
    leds = 0b111111;  // -r6-r5-r4-r3-r2-r1

  } else if (stat == 1) {
    leds = 0b111100;  // -r6-r5-r4-r3-r2-r1

  } else if (stat == 2) {
    leds = 0b110011;  // -r6-r5-r4-r3-r2-r1

  } else if (stat == 3) {
    leds = 0b001111;  // -r6-r5-r4-r3-r2-r1
  }
  updateShiftRegister();
}

void setup() {
  relayUpdate(0);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Component tester");

  lcd.setCursor(0, 1);
  lcd.print("ENTC-TE  press A");


  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  //leds = 0;  // -r6-r5-r4-r3-r2-r1
  //updateShiftRegister();

  Serial.begin(9600);

  //Wire.begin();
  while (Wire.available())
    Serial.println(Wire.read());
  delay(3000);

  Serial.println("setup");

  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  delay(3000);

  relayUpdate(0);

}

//  leds = 0b0000000000000001; // -r6-r5-r4-r3-r2-r1

bool checkA() {
  if (x == "A") {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Select options");

    lcd.setCursor(0, 1);
    lcd.print("1,2 or 3    A ->");

    return (0);
  } else {
    return (1);
  }
}

void loop() {
  relayUpdate(0);

  int val = analogRead(IN_PIN);

  Serial.println(x);

  while (x == 'D') {
    Wire.requestFrom(8, 1);

    Serial.println("available :");
    Serial.println(Wire.available());

    while (!Wire.available()) {
      Serial.println("No avail");
      delay(100);
    };

    while (Wire.available()) {
      x = Wire.read();
      while (x == "" || x == " ") {
        x = Wire.read();
        Serial.println("Reading");
      }

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("You entered :");

      Serial.println("available stat");
      Serial.println(Wire.available());

      lcd.setCursor(0, 1);
      if (x != 'D')
        lcd.print(x);
      Serial.println("Entered");
      Serial.println(x);
      delay(300);

      /*if (x == 'A') {
      while (Wire.available())
        Serial.println(Wire.read());
      break;
    }*/
    }
  }
  delay(1000);

  if (x != 'A') {
    lcd.clear();
    Serial.println("x is :");
    Serial.println(x);
    lcd.setCursor(0, 0);
    lcd.print("WRONG INPUT !!!");

    lcd.setCursor(0, 1);
    lcd.print("Restart device");

    while (true) {
    }
  } else {
    Serial.println("In block");
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Select options");

    lcd.setCursor(0, 1);
    lcd.print("1,2 or 3    A ->");
    delay(300);

    //''''
    x = 'D';
    bool init = 0;
    while (x == 'D') {

      Wire.requestFrom(8, 1);

      //Serial.println("available :");
      //Serial.println(Wire.available());

      while (!Wire.available()) {
        //Serial.println("No avail");
        delay(100);
      };


      while (Wire.available()) {
        y = Wire.read();
        while (y == ' ') {
          y = Wire.read();
          //Serial.println("Reading");
        }

        if (init == 0) {
          lcd.clear();
          lcd.setCursor(11, 1);
          lcd.print("A -->");
          init = 1;
        }



        if (y == '1') {
          lcd.setCursor(0, 0);
          lcd.print('1');
          circuitTest[0] = 1;

        } else if (y == '2') {
          lcd.setCursor(2, 0);
          lcd.print('2');
          circuitTest[1] = 1;

        } else if (y == '3') {
          lcd.setCursor(4, 0);
          lcd.print('3');
          circuitTest[2] = 1;

        } else if (y == 'A') {
          x = 'A';
        }

        //Serial.println("available stat");
        //Serial.println(Wire.available());

        //lcd.setCursor(0, 1);
        //if(x != 'D')
        //  lcd.print(x);
        //Serial.println("Entered");
        //Serial.println(x);
        //delay(300);

        /*if (x == 'A') {
          while (Wire.available())
            Serial.println(Wire.read());
          break;
        }*/
        delay(100);
      }
    }

    // while 2 ended
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Connect");

    lcd.setCursor(0, 1);
    lcd.print("Component");

    lcd.setCursor(11, 1);
    lcd.print("A -->");


    x = 'D';
    while (x == 'D') {

      Wire.requestFrom(8, 1);

      //Serial.println("available :");
      //Serial.println(Wire.available());

      while (!Wire.available()) {
        //Serial.println("No avail");
        delay(100);
      };


      while (Wire.available()) {
        y = Wire.read();
        while (y == ' ') {
          y = Wire.read();
          //Serial.println("Reading");
        }

        if (y == 'A') {
          x = 'A';
        }
        delay(100);
      }
    }

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Testing in");

    lcd.setCursor(0, 1);
    lcd.print("Progress");

    //lcd.setCursor(11, 1);
    //lcd.print("A -->");

    if (circuitTest[0]) {
      relayUpdate(1);

      shuntvoltage = ina219.getShuntVoltage_mV();
      busvoltage = ina219.getBusVoltage_V();
      current_mA = ina219.getCurrent_mA();
      power_mW = ina219.getPower_mW();
      loadvoltage = busvoltage + (shuntvoltage / 1000);

      relayUpdate(0);
    }

    if (circuitTest[1]) {
      relayUpdate(2);

      resistor_val = analogRead(ResistorAnalogPin) * (5.0 / 1023.0);
      resitor_calculated = ((resistorValueResCalcR2 * 5) / resistor_val) - resistorValueResCalcR2;

      //Serial.println("Calculated Resistance : ");

      //if (resitor_calculated < 1000) {
      //  Serial.print(resitor_calculated);
      //  Serial.print(" Ω");

      // } else {
      //  Serial.print(resitor_calculated / 1000.00);
      // Serial.print(" kΩ");
      //}

      relayUpdate(0);
    }

    if (circuitTest[2]) {
      relayUpdate(3);

      pinMode(IN_PIN, INPUT);
      digitalWrite(OUT_PIN, HIGH);
      digitalWrite(OUT_PIN, LOW);

      if (val < 1000) {
        pinMode(IN_PIN, OUTPUT);

        float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);


      } else {
        pinMode(IN_PIN, OUTPUT);
        delay(1);
        pinMode(OUT_PIN, INPUT_PULLUP);
        unsigned long u1 = micros();
        unsigned long t;
        int digVal;

        do {
          digVal = digitalRead(OUT_PIN);
          unsigned long u2 = micros();
          t = u2 > u1 ? u2 - u1 : u1 - u2;
        }

        while ((digVal < 1) && (t < 400000L));

        pinMode(OUT_PIN, INPUT);
        val = analogRead(OUT_PIN);
        digitalWrite(IN_PIN, HIGH);
        int dischargeTime = (int)(t / 1000L) * 5;
        delay(dischargeTime);
        pinMode(OUT_PIN, OUTPUT);
        digitalWrite(OUT_PIN, LOW);
        digitalWrite(IN_PIN, LOW);

        float capacitance = -(float)t / R_PULLUP / log(1.0 - (float)val / (float)MAX_ADC_VALUE);
      }
      // while (micros() % 1000 != 0);

      relayUpdate(0);
    }



    lcd.clear();

    lcd.setCursor(0, 1);
    lcd.print("Results");

    lcd.setCursor(0, 0);
    lcd.print("Processing");




    x = 'D';
    while (x == 'D') {


      if (circuitTest[0]) {



        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bus Voltage:");
        lcd.setCursor(0, 1);
        lcd.print(busvoltage);
        lcd.setCursor(14, 1);
        lcd.print(" V");
        delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Shunt Voltage:");
        lcd.setCursor(0, 1);
        lcd.print(shuntvoltage);
        lcd.setCursor(13, 1);
        lcd.print(" mV");
        delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Load Voltage:");
        lcd.setCursor(0, 1);
        lcd.print(loadvoltage);
        lcd.setCursor(13, 1);
        lcd.print(" V");
        delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Current:");
        lcd.setCursor(0, 1);
        lcd.print(current_mA);
        lcd.setCursor(13, 1);
        lcd.print(" mA");
        delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Power:");
        lcd.setCursor(0, 1);
        lcd.print(power_mW);
        lcd.setCursor(13, 1);
        lcd.print(" mW");
        delay(3000);
      }

      if (circuitTest[1]) {

        lcd.clear();

        if (resitor_calculated < 1000) {

          lcd.setCursor(0, 0);
          lcd.print("Resistance:");
          lcd.setCursor(0, 1);
          lcd.print(resitor_calculated);
          lcd.setCursor(13, 1);
          lcd.print(" Ω");

        } else {
          lcd.setCursor(0, 0);
          lcd.print("Resistance:");
          lcd.setCursor(0, 1);
          lcd.print(resitor_calculated / 1000.00);
          lcd.setCursor(13, 1);
          lcd.print(" kΩ");
        }

        delay(3000);
      }

      if (circuitTest[2]) {

        if (val < 1000) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Scale:   1pF-1nF");
          lcd.setCursor(0, 1);
          lcd.print(capacitance, 3);
          lcd.setCursor(14, 1);
          lcd.print("pF");

          //lcd.setCursor(0,0);
          //lcd.print("Scale:   1pF-1nF");
          delay(3000);

          if (capacitance > 1000.0) {
            lcd.setCursor(0, 1);
            lcd.print(capacitance / 1000.0, 2);
            lcd.setCursor(14, 1);
            lcd.print("uF ");

          }

          else {
            lcd.setCursor(0, 1);
            lcd.print(capacitance);
            lcd.setCursor(14, 1);
            lcd.print("nF");
          }
          delay(3000);
        }
      }

      Wire.requestFrom(8, 1);

      //Serial.println("available :");
      //Serial.println(Wire.available());

      while (!Wire.available()) {
        //Serial.println("No avail");
        delay(100);
      };


      while (Wire.available()) {
        y = Wire.read();
        while (y == ' ') {
          y = Wire.read();
          //Serial.println("Reading");
        }

        if (y == 'A') {
          x = 'A';
        }
        delay(1000);
      }
    }

    while (1)
      ;
  }
}
