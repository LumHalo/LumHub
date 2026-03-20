#include "../../inc/animation/animationPingPong.h"

AnimationPingPong::AnimationPingPong(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed)
    : strip(strip), numPixels(numPixels), r(r), g(g), b(b), intensity(intensity), speed(speed), currentPixel(0), movingForward(true), active(false) {
}

AnimationPingPong::~AnimationPingPong() {
    stop();
}

void AnimationPingPong::start() {
    currentPixel = 0;
    movingForward = true;
    active = true;
    if (xTaskCreatePinnedToCore(taskFunction, "PingPongTask", 4096, this, 1, &xHandle, 0) != pdPASS) {
        Serial.println("Failed to create task");
    }
}

void AnimationPingPong::stop() {
    active = false;
    if (xHandle != NULL) {
        vTaskDelete(xHandle);
        xHandle = NULL;
    }
}

void AnimationPingPong::update() {
    if (!active) return;

    CRGB baseColor = CRGB(r, g, b);
    baseColor.r = scale8_video(baseColor.r, intensity);
    baseColor.g = scale8_video(baseColor.g, intensity);
    baseColor.b = scale8_video(baseColor.b, intensity);

    CRGB color100 = baseColor;
    CRGB color60  = baseColor;
    color60.nscale8(153);
    CRGB color30  = baseColor;
    color30.nscale8(77);
    CRGB colorBG  = baseColor;
    colorBG.nscale8(51);
    
    fill_solid(
        strip,
        numPixels, 
        colorBG
    );

    int8_t dir = movingForward ? -1 : 1;

    auto drawSafe = [&](int16_t idx, CRGB c) {
        if (idx >= 0 && idx < numPixels) strip[idx] = c;
    };

    drawSafe(currentPixel, color100);
    drawSafe(currentPixel + dir, color60);
    drawSafe(currentPixel + 2 * dir, color30);
    
    //FastLED.show();

    pixelDelay += speed;

    if (pixelDelay >= 15) {
        pixelDelay = 0;
        int16_t s3 = (int16_t)speed - 3;
        if (s3 < 0) s3 = 0; 
        uint16_t stepSize = 1 + (s3 * 5 / 27);

        if (movingForward) {
            currentPixel += stepSize;
            if (currentPixel >= (int16_t)numPixels - 1) {
                currentPixel = numPixels - 1;
                movingForward = false;
            }
        } else {
            currentPixel -= stepSize;
            if (currentPixel <= 0|| currentPixel > numPixels) {
                currentPixel = 0;
                movingForward = true;
            }
        }
    }
}

bool AnimationPingPong::isActive() const {
    return active;
}

void AnimationPingPong::setColor(uint8_t r, uint8_t g, uint8_t b) {
    this->r = constrain(r, 0, 255);
    this->g = constrain(g, 0, 255);
    this->b = constrain(b, 0, 255);
}

void AnimationPingPong::setSpeed(float speed) {
    this->speed = constrain(speed, 3.0, 40.0);
}

void AnimationPingPong::setIntensity(uint8_t intensity) {
    this->intensity = constrain(intensity, 0, 255);
}

void AnimationPingPong::taskFunction(void* parameter) {
    AnimationPingPong* instance = static_cast<AnimationPingPong*>(parameter);
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
