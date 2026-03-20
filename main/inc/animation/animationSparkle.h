#ifndef ANIMATION_SPARKLE_H
#define ANIMATION_SPARKLE_H

#include "../animation.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_RMT_BUILTIN_DRIVER 1
#include <FastLED.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstdint>

class AnimationSparkle : public Animation {
public:
    AnimationSparkle(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed);
    void start() override;
    void stop() override;
    void update() override;
    bool isActive() const;
    void setColor(uint8_t r, uint8_t g, uint8_t b) override;
    void setSpeed(float speed) override;
    void setIntensity(uint8_t intensity);
    ~AnimationSparkle() override;

private:
    CRGB* strip; // strip associée à l'animation
    uint8_t numPixels;
    uint8_t r, g, b; // Couleur de la LED scintillante
    uint8_t intensity; // Intensité de la LED scintillante
    float speed; // Vitesse de l'animation
    uint8_t currentPixel; // Pixel actuel
    uint8_t pixelDelay; // Delais pour allumer un nouveau pixel
    TaskHandle_t xHandle; // Handle de la tâche FreeRTOS
    bool active; // Indique si l'animation est active
    uint8_t stripValues[60];

    static void taskFunction(void* parameter);
};

#endif // ANIMATION_SPARKLE_H
