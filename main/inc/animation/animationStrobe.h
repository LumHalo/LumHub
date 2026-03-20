#ifndef ANIMATION_STROBE_H
#define ANIMATION_STROBE_H

#include "../animation.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_RMT_BUILTIN_DRIVER 1
#include <FastLED.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstdint>

class AnimationStrobe : public Animation {
public:
    AnimationStrobe(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed);
    ~AnimationStrobe();

    void start();
    void stop();
    void update();
    bool isActive() const;

    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setSpeed(float speed);
    void setIntensity(uint8_t intensity);

private:
    static void taskFunction(void* parameter);

    CRGB* strip;
    uint8_t numPixels;
    uint8_t r, g, b;
    uint8_t intensity;
    float speed;
    bool active;
    bool state; // true = on, false = off
    TaskHandle_t xHandle = NULL;
};

#endif
