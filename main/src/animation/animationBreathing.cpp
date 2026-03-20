#include "../../inc/animation/animationBreathing.h"

AnimationBreathing::AnimationBreathing(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed)
    : strip(strip), numPixels(numPixels), r(r), g(g), b(b), intensity(intensity), xHandle(nullptr), speed(speed) {}

AnimationBreathing::~AnimationBreathing() {
    stop();
}

void AnimationBreathing::start() {
    breathingIn = true;
    currentIntensity = 0;
    active = true;
    if (xTaskCreatePinnedToCore(taskFunction, "BreathingTask", 4096, this, 1, &xHandle, 0) != pdPASS) {
        Serial.println("Failed to create task");
    }
}

void AnimationBreathing::stop() {
    active = false;
    if (xHandle != NULL) {
        vTaskDelete(xHandle);
        xHandle = NULL;
    }
}

void AnimationBreathing::update() {
    if (!active) return;

    if (breathingIn) {
        currentIntensity += speed;
        if (currentIntensity >= 255) {
            currentIntensity = 255;
            breathingIn = false;
        }
    } else {
        currentIntensity -= speed;
        if (currentIntensity <= 30 || currentIntensity > 255) {
            currentIntensity = 30;
            breathingIn = true;
        }
    }

    uint8_t scaledR = (currentIntensity * r) >> 8;
    uint8_t scaledG = (currentIntensity * g) >> 8;
    uint8_t scaledB = (currentIntensity * b) >> 8;

    fill_solid(
        strip,
        numPixels, 
        CRGB(
            scale8_video(scaledR, intensity),
            scale8_video(scaledG, intensity),
            scale8_video(scaledB, intensity)
        )
    );
    //FastLED.show();
}

bool AnimationBreathing::isActive() const {
    return active;
}

void AnimationBreathing::setColor(uint8_t r, uint8_t g, uint8_t b) {
    this->r = constrain(r, 0, 255);
    this->g = constrain(g, 0, 255);
    this->b = constrain(b, 0, 255);
}

void AnimationBreathing::setSpeed(float speed) {
    this->speed = constrain(speed, 1.0, 40.0);
}

void AnimationBreathing::setIntensity(uint8_t intensity) {
    this->intensity = constrain(intensity, 0, 255);
}

void AnimationBreathing::taskFunction(void* parameter) {
    AnimationBreathing *instance = static_cast<AnimationBreathing*>(parameter);
    bool init = true;

    while (instance->isActive()) {
        instance->update();
        /* if (init) {
            uint8_t delayTime = esp_random() % 255;
            vTaskDelay(pdMS_TO_TICKS(delayTime));
            init = false;
        } else {
            vTaskDelay(pdMS_TO_TICKS(23));
        } */
        vTaskDelay(pdMS_TO_TICKS(23));
    }
    vTaskDelete(NULL);
}
