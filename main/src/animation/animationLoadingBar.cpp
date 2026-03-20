#include "../../inc/animation/animationLoadingBar.h"

AnimationLoadingBar::AnimationLoadingBar(CRGB* strip, uint8_t numPixels, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, float speed)
    : strip(strip), numPixels(numPixels), pixelDelay(0), r(r), g(g), b(b), intensity(intensity), speed(speed), currentPixel(0), active(false) {}

AnimationLoadingBar::~AnimationLoadingBar() {
    stop();
}

void AnimationLoadingBar::start() {
    currentPixel = 0;
    active = true;
    if (xTaskCreatePinnedToCore(taskFunction, "LoadingBarTask", 4096, this, 1, &xHandle, 0) != pdPASS) {
        Serial.println("Failed to create task");
    }
}

void AnimationLoadingBar::stop() {
    active = false;
    if (xHandle != NULL) {
        vTaskDelete(xHandle);
        xHandle = NULL;
    }
}

void AnimationLoadingBar::update() {
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

    int p0 = currentPixel;
    int p1 = (currentPixel - 1 + numPixels) % numPixels;
    int p2 = (currentPixel - 2 + numPixels) % numPixels;

    strip[p0] = color100;
    strip[p1] = color60;
    strip[p2] = color30;

    //FastLED.show();

    pixelDelay += speed;

    if (pixelDelay >= 15) {
        uint8_t stepSize = 1 + ((uint16_t)(speed - 3) * 5 / 27); 
        currentPixel = (currentPixel + stepSize) % numPixels;
        pixelDelay = 0;
    }
}

bool AnimationLoadingBar::isActive() const {
    return active;
}

void AnimationLoadingBar::setColor(uint8_t r, uint8_t g, uint8_t b) {
    this->r = constrain(r, 0, 255);
    this->g = constrain(g, 0, 255);
    this->b = constrain(b, 0, 255);
}

void AnimationLoadingBar::setSpeed(float speed) {
    this->speed = constrain(speed, 3.0, 30.0);
}

void AnimationLoadingBar::setIntensity(uint8_t intensity) {
    this->intensity = constrain(intensity, 0, 255);
}

void AnimationLoadingBar::taskFunction(void* parameter) {
    AnimationLoadingBar* instance = static_cast<AnimationLoadingBar*>(parameter);
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
