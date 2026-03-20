#ifndef ANIMATION_PING_PONG_H
#define ANIMATION_PING_PONG_H

#include "../animation.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_RMT_BUILTIN_DRIVER 1
#include <FastLED.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstdint>

class AnimationPingPong : public Animation {
public:
    AnimationPingPong(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed);
    void start() override;
    void stop() override;
    void update() override;
    bool isActive() const;
    void setColor(uint8_t r, uint8_t g, uint8_t b) override;
    void setSpeed(float speed) override;
    void setIntensity(uint8_t intensity);
    ~AnimationPingPong() override;

private:
    CRGB* strip; // strip associée à l'animation
    uint8_t numPixels;
    uint8_t r, g, b; // Couleur de la LED à intensité maximale
    uint8_t intensity; // Intensité de la LED à intensité maximale
    float speed; // Vitesse de la LED à intensité maximale
    uint16_t currentPixel; // Pixel actuel
    uint8_t pixelDelay;
    bool movingForward; // Indique si la LED se déplace vers l'avant
    TaskHandle_t xHandle; // Handle de la tâche FreeRTOS
    bool active; // Indique si l'animation est active

    static void taskFunction(void* parameter);
};

#endif // ANIMATION_PING_PONG_H
