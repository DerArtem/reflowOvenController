#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Arduino.h>
#include <SPI.h>

typedef struct Thermocouple {
  double temperature;
  uint8_t stat;
  uint8_t chipSelect;
};

#define LCD_CS 10

void readThermocouple(struct Thermocouple* input) {
  volatile uint16_t value;
  uint8_t lcdState = digitalRead(LCD_CS);
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(input->chipSelect, LOW);
  
  delay(1);
  value = SPI.transfer(0x00);
  value <<= 8;
  value |= SPI.transfer(0x00);

  if (value & 0x4) {
      // uh oh, no thermocouple attached!
      return NAN;
  }

  value >>= 3;

  input->temperature = value * 0.25;

  digitalWrite(input->chipSelect, HIGH);
  digitalWrite(LCD_CS, lcdState);
}

#endif

