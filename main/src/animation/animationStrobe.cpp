#include "../../inc/animation/animationStrobe.h"

AnimationStrobe::AnimationStrobe(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed)
    : strip(strip), numPixels(numPixels), r(r), g(g), b(b), intensity(intensity), speed(speed), active(false), state(false) {}

AnimationStrobe::~AnimationStrobe() {
    stop();
}

void AnimationStrobe::start() {
    active = true;
    state = false;
    if (xTaskCreatePinnedToCore(taskFunction, "StrobeTask", 4096, this, 1, &xHandle, 0) != pdPASS) {
        Serial.println("Failed to create strobe task");
    }
}

void AnimationStrobe::stop() {
    active = false;
    if (xHandle != NULL) {
        vTaskDelete(xHandle);
        xHandle = NULL;
    }
}

void AnimationStrobe::update() {
    if (!active) return;

    state = !state;

    if (state) {
        fill_solid(strip, numPixels, CRGB(
            scale8_video(r, intensity),
            scale8_video(g, intensity),
            scale8_video(b, intensity)
        ));
    } else {
        fill_solid(strip, numPixels, CRGB(0, 0, 0));
    }

    //FastLED.show();
}

bool AnimationStrobe::isActive() const {
    return active;
}

void AnimationStrobe::setColor(uint8_t r, uint8_t g, uint8_t b) {
    this->r = constrain(r, 0, 255);
    this->g = constrain(g, 0, 255);
    this->b = constrain(b, 0, 255);
}

void AnimationStrobe::setSpeed(float speed) {
    this->speed = constrain(speed, 1.0, 29.0); // 1Hz = lent, 29Hz = très rapide
}

void AnimationStrobe::setIntensity(uint8_t intensity) {
    this->intensity = constrain(intensity, 0, 255);
}

void AnimationStrobe::taskFunction(void* parameter) {
    AnimationStrobe* instance = static_cast<AnimationStrobe*>(parameter);

    while (instance->isActive()) {
        instance->update();
        // Vitesse en flashs par seconde → on la convertit en délai
        float delayMs = 500.0f / instance->speed;
        vTaskDelay(pdMS_TO_TICKS(delayMs));
    }
    vTaskDelete(NULL);
}
