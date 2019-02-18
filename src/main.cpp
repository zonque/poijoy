#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

MPU6050 mpu6050(Wire);

long timer = 0;
#define NUM_PIXEL 5
#define PIXEL_PIN 14
Adafruit_NeoPixel *strip;

void HSVtoRGB(float h, float s, float v, int *r, int *g, int *b)
{
  int i;
  float f, p, q, t;

  h = max(0.0f, min(360.0f, h));
  s = max(0.0f, min(100.0f, s));
  v = max(0.0f, min(100.0f, v));

  s /= 100;
  v /= 100;

  if (s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v * 255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));

  switch (i) {
  case 0:
    *r = round(255 * v);
    *g = round(255 * t);
    *b = round(255 * p);
    break;

  case 1:
    *r = round(255 * q);
    *g = round(255 * v);
    *b = round(255 * p);
    break;
  case 2:
    *r = round(255 * p);
    *g = round(255 * v);
    *b = round(255 * t);
    break;

  case 3:
    *r = round(255 * p);
    *g = round(255 * q);
    *b = round(255 * v);
    break;

  case 4:
    *r = round(255 * t);
    *g = round(255 * p);
    *b = round(255 * v);
    break;

  default: // case 5:
    *r = round(255 * v);
    *g = round(255 * p);
    *b = round(255 * q);
    break;
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();

  strip = new Adafruit_NeoPixel(NUM_PIXEL, PIXEL_PIN, NEO_GRB | NEO_KHZ800);
  strip->begin();
  strip->show();

  //mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();

  if (millis() - timer > 20) {
    float h = mpu6050.getAngleY() + 180.0;
    int r, g, b;

    Serial.printf("h: %.2f\n", h);
    HSVtoRGB(h, 100.0, 100.0, &r, &g, &b);

    for (int i = 0; i < NUM_PIXEL; i++) {
      strip->setPixelColor(i, r, g, b);
    }

    strip->show();
    timer = millis();
  }
}
