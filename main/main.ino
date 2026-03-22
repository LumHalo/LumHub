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
typedef WS2811<LED_PIN_2_GPIO, RGB> ControllerT2;
typedef WS2811<LED_PIN_3_GPIO, RGB> ControllerT3;
typedef WS2811<LED_PIN_4_GPIO, RGB> ControllerT4;

static RGBWEmulatedController<ControllerT1, RGB> rgbwEmu1(rgbw);
static RGBWEmulatedController<ControllerT2, RGB> rgbwEmu2(rgbw);
static RGBWEmulatedController<ControllerT3, RGB> rgbwEmu3(rgbw);
static RGBWEmulatedController<ControllerT4, RGB> rgbwEmu4(rgbw);

CRGB strip[NUM_LEDS];
CRGB strip2[NUM_LEDS];
CRGB strip3[NUM_LEDS];
CRGB strip4[NUM_LEDS];

Fixture fixture(1, 1, 2, NUM_LEDS, strip); //Id, Dmx, Mode, NumLeds, Strip
Fixture fixture2(2, 66, 2, NUM_LEDS, strip2);
Fixture fixture3(3, 131, 2, NUM_LEDS, strip3);
Fixture fixture4(4, 196, 2, NUM_LEDS, strip4);

FixtureManagement fixtureManagement;
SerialManagement serialManagement;
//ServerManagement serverManagement;

void setup() {
    Serial.begin(115200);  // Initialisation de la communication série
    delay(1000);
    btStop();
    WiFi.mode(WIFI_OFF);

    FastLED.addLeds(&rgbwEmu1, strip, NUM_LEDS);
    FastLED.addLeds(&rgbwEmu2, strip2, NUM_LEDS);
    FastLED.addLeds(&rgbwEmu3, strip3, NUM_LEDS);
    FastLED.addLeds(&rgbwEmu4, strip4, NUM_LEDS);
    FastLED.setBrightness(255);
    FastLED.setDither(true);
    
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