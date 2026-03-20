#include "../../inc/animation/AnimationStars.h"

AnimationStars::AnimationStars(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed)
    : strip(strip), numPixels(numPixels), pixelDelay(0), r(r), g(g), b(b), intensity(intensity), speed(speed), currentPixel(0), active(false) {
    stripValues[numPixels] = {};
    stripStatus[numPixels] = {};
}

AnimationStars::~AnimationStars() {
    stop();
}

void AnimationStars::start() {
    currentPixel = 0;
    active = true;
    if (xTaskCreatePinnedToCore(taskFunction, "SparkleTask", 4096, this, 1, &xHandle, 0) != pdPASS) {
        Serial.println("Failed to create task");
    }
}

void AnimationStars::stop() {
    active = false;
    if (xHandle != NULL) {
        vTaskDelete(xHandle);
        xHandle = NULL;
    }
}

void AnimationStars::update() {
    if (!active) return;

    bool createPixel = pixelDelay == 0;

    CRGB baseColor = CRGB(
        scale8_video(r, intensity), 
        scale8_video(g, intensity),
        scale8_video(b, intensity)
    );

    uint8_t delta = speed + 1;
    
    for (int i = 0; i < numPixels; i++) {
        uint8_t val = stripValues[i];
        if (stripStatus[i] == 2) {
            stripValues[i] = qadd8(val, delta);
            if (stripValues[i] == 255) stripStatus[i] = 1;
        } 
        else {
            stripValues[i] = qsub8(val, delta);
            if (stripValues[i] == 0) stripStatus[i] = 0;
        }
        
        if (stripStatus[i] == 2) {
            if (stripValues[i] + speed + 1 > 255) {
                stripValues[i] = 255;
                stripStatus[i] = 1;
            }
            if (stripValues[i] < 255) {
                stripValues[i] = stripValues[i] + speed + 1;
            }
        } else {
            if (stripValues[i] - speed - 1 < 0) {
                stripValues[i] = 0;
                stripStatus[i] = 0;
            }
            if (stripValues[i] > 0) {
                stripValues[i] = stripValues[i] - speed - 1;
            }
        }

        strip[i] = baseColor;
        strip[i].nscale8(stripValues[i]);
    }
    
    if (createPixel) {
        uint16_t offset = 1 + (esp_random() % (numPixels - 1));
        currentPixel = (currentPixel + offset) % numPixels;

        stripStatus[currentPixel] = 2;
    } else if (pixelDelay >= 60) {
        pixelDelay = 0;
    }

    //FastLED.show();
}

bool AnimationStars::isActive() const {
    return active;
}

void AnimationStars::setColor(uint8_t r, uint8_t g, uint8_t b) {
    this->r = constrain(r, 0, 255);
    this->g = constrain(g, 0, 255);
    this->b = constrain(b, 0, 255);
}

void AnimationStars::setSpeed(float speed) {
    this->speed = constrain(speed, 1.0, 30.0);
}

void AnimationStars::setIntensity(uint8_t intensity) {
    this->intensity = constrain(intensity, 0, 255);
}

void AnimationStars::taskFunction(void* parameter) {
    AnimationStars* instance = static_cast<AnimationStars*>(parameter);
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
