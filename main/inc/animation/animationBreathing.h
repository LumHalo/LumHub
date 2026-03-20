#ifndef ANIMATION_BREATHING_H
#define ANIMATION_BREATHING_H

#include "../animation.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_RMT_BUILTIN_DRIVER 1
#include <FastLED.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstdint>

class AnimationBreathing : public Animation {
    public :
    AnimationBreathing(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed);
    void start() override;
    void stop() override;
    void update() override;
    bool isActive() const;
    void setColor(uint8_t red, uint8_t green, uint8_t blue) override;
    void setSpeed(float speed) override;
    void setIntensity(uint8_t intensity) override;
    ~AnimationBreathing() override;

    private :
    CRGB* strip; // strip associée à l'animation
    uint8_t numPixels;
    uint8_t r, g, b; // Couleur de la respiration
    uint8_t intensity; // Intensité de la respiration
    float speed; // Vitesse de la respiration
    bool breathingIn; // Indique si l'animation est en phase d'inspiration
    uint16_t currentIntensity; // Intensité actuelle
    TaskHandle_t xHandle; // Handle de la tâche FreeRTOS
    bool active; // Indique si l'animation est active
    static void taskFunction(void* parameter);

};






#endif // ANIMATION_BREATHING_H