/*
 * wand1.ino
 *
 * Arduino controlled Hitachi massager controller box thingy.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 */

#include<Arduino.h>
#include<Adafruit_BLE.h>
#include<Adafruit_BluefruitLE_SPI.h>
#include<Adafruit_SSD1306.h>
#include<SPI.h>
#include<Wire.h>
#include<ACE128.h>
#include<ACE128map12345678.h>

#define SSD1306_ADDR 0x3C
#define ACE_ADDR 0x38

#define POS_MIN 0
#define POS_MAX 127
#define PWM_MIN 0
#define PWM_MAX 255
Adafruit_SSD1306 display = Adafruit_SSD1306();
ACE128 encoder = ACE128(ACE_ADDR, (uint8_t*)encoderMap_12345678);

uint8_t curr_pwm = 0;
uint8_t last_pwm = 0;
uint8_t percent = 0;

int8_t  curr_pos = 0;
int8_t  last_pos = 0;
uint8_t upos = 0;
uint8_t rawpos = 0;
uint8_t greycode = 0;

void setup() {

  delay(1000);

  if (Serial) {
    Serial.begin(57600);
    Serial.println("hi");
  }

  Wire.begin();

  //delay(100);

  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDR);
  display.clearDisplay();
  display.dim(true);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("hi");
  display.display();

  encoder.begin();

}

void loop() {

  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);

  curr_pos = encoder.pos();
  upos = encoder.upos();
  rawpos = encoder.rawPos();
  greycode = encoder.acePins();

  // If encoder has updated, update PWM values
  if (curr_pos != last_pos) {
    if (Serial) {
      Serial.print(F("pos="));
      Serial.print(curr_pos);
      Serial.print(F(" upos="));
      Serial.print(upos);
      Serial.print(F(" raw="));
      Serial.print(rawpos);
      Serial.print(F(" gc="));
      Serial.print(greycode, BIN);
      Serial.println();
    }
    last_pos = curr_pos;
    curr_pwm = map(curr_pos, POS_MIN, POS_MAX, PWM_MIN, PWM_MAX);
  }

  // If PWM values have changed, update display to match
  if (curr_pwm != last_pwm) {

    percent = map(curr_pwm, PWM_MIN, PWM_MAX, 0, 100);
    //display.print(percent);

    display.clearDisplay();
    //display.setTextSize(3);

    display.setCursor(0, 0);
    display.print(F("pos="));
    display.print(curr_pos);

    display.setCursor(0, 8);
    display.print(F(" upos="));
    display.print(upos);

    display.setCursor(0, 16);
    display.print(F(" raw="));
    display.print(rawpos);

    display.setCursor(0, 24);
    display.print(F(" gc="));
    display.print(greycode, BIN);

    display.display();

    if (Serial) {
      Serial.print(F("pwm = "));
      Serial.println(curr_pwm);
    }

    last_pwm = curr_pwm;

  }

}

// vi: syntax=cpp ts=4 sw=4 expandtab
