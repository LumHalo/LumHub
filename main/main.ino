#include "inc/main.h"

#define LED_PIN_GPIO     26
#define LED_PIN_2_GPIO   25
#define LED_PIN_3_GPIO   33
#define LED_PIN_4_GPIO   32
#define NUM_LEDS  20

#define DMX_TX_PIN 17 //DI
#define DMX_RX_PIN 16 //RO
#define DMX_ENABLE_PIN 4 //DE/RE

Rgbw rgbw = Rgbw(
    kRGBWDefaultColorTemp,
    kRGBWExactColors,
    W0
);

typedef WS2811<LED_PIN_GPIO, RGB> ControllerT1;

static RGBWEmulatedController<ControllerT1, RGB> rgbwEmu1(rgbw);

CRGB strip[NUM_LEDS];
CRGB strip2[NUM_LEDS];
CRGB strip3[NUM_LEDS];
CRGB strip4[NUM_LEDS];

Fixture fixture(1, 1, 2, NUM_LEDS, strip); //Id, Dmx, Mode, NumLeds, Strip
Fixture fixture2(2, 1, 2, NUM_LEDS, strip2);
Fixture fixture3(3, 11, 0, NUM_LEDS, strip3);
Fixture fixture4(4, 16, 0, NUM_LEDS, strip4);

FixtureManagement fixtureManagement;
SerialManagement serialManagement;
//ServerManagement serverManagement;

void setup() {
    Serial.begin(115200);  // Initialisation de la communication série
    delay(1000);
    btStop();
    WiFi.mode(WIFI_OFF);

    //FastLED.addLeds<WS2811, LED_PIN_GPIO, RGB>(strip, NUM_LEDS).setRgbw(RgbwDefault());
    FastLED.addLeds(&rgbwEmu1, strip, NUM_LEDS);
    FastLED.addLeds<WS2811, LED_PIN_2_GPIO, BRG>(strip2, NUM_LEDS);
    FastLED.addLeds<WS2811, LED_PIN_3_GPIO, RGB>(strip3, NUM_LEDS).setCorrection(0xFFE0B1);
    FastLED.addLeds<WS2811, LED_PIN_4_GPIO, RGB>(strip4, NUM_LEDS).setCorrection(0xFFE0B1);
    FastLED.setBrightness(255);
    FastLED.setDither(true);

    /* strip.fill(0, 0, 0);
    strip2.fill(0, 0, 0);
    strip3.fill(0, 0, 0);
    strip4.fill(0, 0, 0); */

    //.setTemperature(CRGB(255, 235, 220))
    
    fixtureManagement.addFixture(&fixture);  // Ajout de la fixture au gestionnaire de fixtures
    fixtureManagement.addFixture(&fixture2);
    fixtureManagement.addFixture(&fixture3);
    fixtureManagement.addFixture(&fixture4);
    //serverManagement.begin(fixtureManagement);
    serialManagement.initInput(DMX_RX_PIN, UART_NUM_1, &fixtureManagement);
    Serial.println("Init");
}

void loop() {
    EVERY_N_MILLISECONDS(16) {
        FastLED.show(); 
    }
}