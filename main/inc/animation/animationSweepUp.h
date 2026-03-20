#ifndef ANIMATION_SWEEP_UP_H
#define ANIMATION_SWEEP_UP_H

#include "../animation.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_RMT_BUILTIN_DRIVER 1
#include <FastLED.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <cstdint>

class AnimationSweepUp : public Animation {
public:
    AnimationSweepUp(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed);
    void start() override;
    void stop() override;
    void update() override;
    bool isActive() const;
    void setColor(uint8_t r, uint8_t g, uint8_t b) override;
    void setSpeed(float speed) override;
    void setIntensity(uint8_t intensity) override;
    ~AnimationSweepUp() override;

private:
    CRGB* strip; // strip associée à l'animation
    uint8_t numPixels;
    uint8_t r, g, b; // Couleur de l'animation
    uint8_t intensity; // Intensité de l'animation
    float speed; // Vitesse de l'animation
    bool sweepingUp; // Indique si l'animation est en phase de balayage vers le haut
    uint8_t currentPixel; // Pixel actuel
    unsigned long previousMillis; // Temps précédent pour la gestion du délai
    TaskHandle_t xHandle; // Handle de la tâche FreeRTOS
    bool active; // Indique si l'animation est active
    uint8_t cycleDuration;
    static void taskFunction(void* parameter);
};

#endif // ANIMATION_SWEEP_UP_H
