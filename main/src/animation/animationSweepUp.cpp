#include "../../inc/animation/animationSweepUp.h"

AnimationSweepUp::AnimationSweepUp(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed)
    : strip(strip), numPixels(numPixels), r(r), g(g), b(b), intensity(intensity), speed(speed), sweepingUp(true), currentPixel(0), previousMillis(0), cycleDuration(1000), active(false) {
}

AnimationSweepUp::~AnimationSweepUp() {
    stop();
}

void AnimationSweepUp::start() {
    sweepingUp = true;
    currentPixel = 0;
    previousMillis = millis();
    active = true;
    if (xTaskCreatePinnedToCore(taskFunction, "SweepUpTask", 2048, this, 2, &xHandle,0) != pdPASS) {
        Serial.println("Failed to create task");
    }
}

void AnimationSweepUp::stop() {
    active = false;
    if (xHandle != NULL) {
        vTaskDelete(xHandle);
        xHandle = NULL;
    }
}

void AnimationSweepUp::update() {
    if (!active) {
        return;
    }
    unsigned long currentMillis = millis();
    unsigned long elapsedTime = currentMillis - previousMillis;
    if (elapsedTime >= cycleDuration / speed) {
        previousMillis = currentMillis;

        if (sweepingUp) {
            if (currentPixel < numPixels) {
                strip[currentPixel] = CRGB(
                    scale8_video(r, intensity), 
                    scale8_video(g, intensity),
                    scale8_video(b, intensity)
                );
                currentPixel++;
            } else {
                sweepingUp = false;
            }
        } else {
            if (currentPixel > 0) {
                strip[currentPixel - 1] = CRGB(0,0,0);
                currentPixel--;
            } else {
                sweepingUp = true;
            }
        }
        //FastLED.show();
    }
}

bool AnimationSweepUp::isActive() const {
    return active;
}

void AnimationSweepUp::setColor(uint8_t r, uint8_t g, uint8_t b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void AnimationSweepUp::setSpeed(float speed) {
    this->speed = speed;
}

void AnimationSweepUp::setIntensity(uint8_t intensity) {
    this->intensity = intensity;
}

void AnimationSweepUp::taskFunction(void* parameter) {
    AnimationSweepUp* instance = static_cast<AnimationSweepUp*>(parameter);
    bool init = true;

    while (instance->isActive()) {
        instance->update();
        if (init) {
            uint8_t delayTime = esp_random() % 255;
            vTaskDelay(pdMS_TO_TICKS(delayTime));
            init = false;
        } else {
            vTaskDelay(pdMS_TO_TICKS(23));
        }
    }
    vTaskDelete(NULL);
}
