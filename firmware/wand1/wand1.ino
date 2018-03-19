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

#define SSD1306_ADDR 0x78
#define ACE_ADDR 0x38

Adafruit_SSD1306 display = Adafruit_SSD1306();
ACE128 encoder = ACE128(ACE_ADDR, (uint8_t*)encoderMap_12345678);

uint8_t curr_pwm = 0;
uint8_t last_pwm = 0;
uint8_t curr_pos = 0;
uint8_t last_pos = 0;
uint8_t percent = 0;

void setup() {

  if (Serial) {
    Serial.begin(57600);
    Serial.println("hi");
  }

  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDR);
  display.clearDisplay();
  display.dim(false);
  display.setTextSize(3);
  display.print("hi");
  display.display();

  encoder.begin();

}

void loop() {

  curr_pos = encoder.upos();
  if (curr_pos != last_pos) {
    Serial.print(F("upos="));
    Serial.print(curr_pos);
    Serial.print(F(" pos="));
    Serial.print(encoder.pos());
    Serial.print(F(" mpos="));
    Serial.print(encoder.mpos());
    Serial.print(F(" raw="));
    Serial.print(encoder.rawPos());
    Serial.print(F(" gc="));
    Serial.print(encoder.acePins(), BIN);
    Serial.println();
    last_pos = curr_pos;
  }

  //curr_pwm = curr_pos;
  if (curr_pwm != last_pwm) {
    percent = map(curr_pwm, 0, 255, 0, 100);
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(0, 0);
    display.print(percent);
    display.display();
    Serial.print(F("pwm = "));
    Serial.println(curr_pwm);
    last_pwm = curr_pwm;
  }

}

// vi: syntax=cpp ts=4 sw=4 expandtab
