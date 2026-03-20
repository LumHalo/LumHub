#include "../../inc/animation/animationSparkle.h"

AnimationSparkle::AnimationSparkle(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed)
    : strip(strip), numPixels(numPixels), pixelDelay(0), r(r), g(g), b(b), intensity(intensity), speed(speed), currentPixel(0), active(false) {
    stripValues[numPixels] = {};
}

AnimationSparkle::~AnimationSparkle() {
    stop();
}

void AnimationSparkle::start() {
    currentPixel = 0;
    active = true;
    if (xTaskCreatePinnedToCore(taskFunction, "SparkleTask", 4096, this, 1, &xHandle, 0) != pdPASS) {
        Serial.println("Failed to create task");
    }
}

void AnimationSparkle::stop() {
    active = false;
    if (xHandle != NULL) {
        vTaskDelete(xHandle);
        xHandle = NULL;
    }
}

void AnimationSparkle::update() {
    if (!active) return;

    bool createPixel = pixelDelay == 0;

    pixelDelay += speed;

    CRGB baseColor = CRGB(
        scale8_video(r, intensity), 
        scale8_video(g, intensity),
        scale8_video(b, intensity)
    );

    uint8_t fadeAmt = speed + 1;
    
    for (int i = 0; i < numPixels; i++) {
        stripValues[i] = qsub8(stripValues[i], fadeAmt);

        strip[i] = baseColor;
        strip[i].nscale8(stripValues[i]);
    }
    
    if (createPixel) {
        uint16_t offset = 1 + (esp_random() % (numPixels - 1));
        currentPixel = (currentPixel + offset) % numPixels;

        stripValues[currentPixel] = 255;

        strip[currentPixel] = baseColor;
    } else if (pixelDelay >= 60) {
        pixelDelay = 0;
    }

    //FastLED.show();
}

bool AnimationSparkle::isActive() const {
    return active;
}

void AnimationSparkle::setColor(uint8_t r, uint8_t g, uint8_t b) {
    this->r = constrain(r, 0, 255);
    this->g = constrain(g, 0, 255);
    this->b = constrain(b, 0, 255);
}

void AnimationSparkle::setSpeed(float speed) {
    this->speed = constrain(speed, 1.0, 40.0);
}

void AnimationSparkle::setIntensity(uint8_t intensity) {
    this->intensity = constrain(intensity, 0, 255);
}

void AnimationSparkle::taskFunction(void* parameter) {
    AnimationSparkle* instance = static_cast<AnimationSparkle*>(parameter);
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
