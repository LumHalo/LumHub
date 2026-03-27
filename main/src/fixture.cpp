#include "../inc/fixture.h"
#include "../inc/animation/animationBreathing.h"
#include "../inc/animation/animationSweepUp.h"
#include "../inc/animation/animationLoadingBar.h"
#include "../inc/animation/animationPingPong.h"
#include "../inc/animation/animationSparkle.h"
#include "../inc/animation/AnimationStars.h"
#include "../inc/animation/AnimationStrobe.h"


Fixture::Fixture(uint8_t id, uint8_t dmxStart, uint8_t mode, uint8_t numPixels, CRGB* strip)
    : id(id), dmxStart(dmxStart), mode(mode), strip(strip), numPixels(numPixels), animation(nullptr), animationType(NONE) {}

uint8_t Fixture::getDMXStart() {
    return dmxStart;
}

uint8_t Fixture::getMode() {
    return mode;
}

void Fixture::setDMXStart(uint8_t dmxStart) {
    this->dmxStart = dmxStart;
}

void Fixture::processDMXData(uint8_t* dmxData) {
    if (previousCheck(dmxData)){
        return;
    }

    setPreviousData(dmxData);

    if (mode == 0 || mode == 2 && (dmxData[1] != 0 || dmxData[2] != 0 || dmxData[3] != 0 || dmxData[4] != 0)) {
        uint8_t globalIntensity = dmxData[0];
        uint8_t redValue = dmxData[1];
        uint8_t greenValue = dmxData[2];
        uint8_t blueValue = dmxData[3];
        uint8_t animationIndicator = dmxData[4];
        
        if (animationIndicator != 0) {
            // Activer l'animation appropriée
            activateAnimation(animationIndicator, globalIntensity, redValue, greenValue, blueValue);
        } else {
            noAnimation(globalIntensity, redValue, greenValue, blueValue);
        }
    } else {
        deleteAnimation();
        if (mode == 2) {
            setPixels(&dmxData[5], dmxData[0]);
        } else {
            setPixels(dmxData, 255);
        }
    }
}

bool Fixture::previousCheck(uint8_t* dmxData) {
    if (previousDmxData == nullptr){
        return true;
    }
    uint8_t previousCount = (mode == 0) ? 5 : (mode == 1) ? 60 : 65;

    for (int i = 0; i < previousCount; i++) {
        if (previousDmxData[i] != dmxData[i]) {
            return false;
        }
    }
    return true;
}

void Fixture::setPreviousData(uint8_t* dmxData) {
    uint8_t previousCount = (mode == 0) ? 5 : (mode == 1) ? 60 : 65;

    for (int i = 0; i < previousCount; i++) {
        previousDmxData[i] = dmxData[i];
    }
}

void Fixture::setPixels(uint8_t* dmxData, uint8_t intensity) {
    for (int i = 0; i < numPixels; i++) {
        strip[i] = CRGB(
            scale8_video(dmxData[i * 3], intensity),
            scale8_video(dmxData[1 + i * 3], intensity),
            scale8_video(dmxData[2 + i * 3], intensity)
        );
    }

    //FastLED.show();
}

void Fixture::noAnimation(uint8_t intensity, uint8_t redValue, uint8_t greenValue, uint8_t blueValue) {
    deleteAnimation();

    fill_solid(
        strip,
        numPixels, 
        CRGB(
            scale8_video(redValue, intensity),
            scale8_video(greenValue, intensity),
            scale8_video(blueValue, intensity)
        )
    );

    //FastLED.show();
}

void Fixture::deleteAnimation(){
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
        animationType = NONE;
    }
}

void Fixture::activateAnimation(uint8_t animationIndicator, uint8_t intensity, uint8_t redValue, uint8_t greenValue, uint8_t blueValue) {
    if (animationIndicator > 0 && animationIndicator <= 30) { //BREATHING
        float speed = map(animationIndicator, 1, 30, 1.0, 40.0);
        if (animationType != BREATHING) {
            deleteAnimation();
            animation = new AnimationBreathing(strip, numPixels, redValue, greenValue, blueValue, intensity, speed);
            animationType = BREATHING;
            animation->start();
        } else {
            animation->setColor(redValue, greenValue, blueValue);
            animation->setSpeed(speed);
            animation->setIntensity(intensity);
        }
    } else if (animationIndicator > 30 && animationIndicator <= 60){ //SWEEPUP
        float speed = map(animationIndicator, 31, 60, 12.0, 1.0);
        if (animationType != SWEEPUP) {
            deleteAnimation();
            animation = new AnimationSweepUp(strip, numPixels, redValue, greenValue, blueValue, intensity, speed);
            animationType = SWEEPUP;
            animation->start();
        } else {
            animation->setColor(redValue, greenValue, blueValue);
            animation->setSpeed(speed);
            animation->setIntensity(intensity);
        }
    } else if (animationIndicator > 60 && animationIndicator <= 90){ //LOADINGBAR
        float speed = map(animationIndicator, 61, 90, 3.0, 30.0);
        if (animationType != LOADINGBAR) {
            deleteAnimation();
            animation = new AnimationLoadingBar(strip, numPixels, redValue, greenValue, blueValue, intensity, speed);
            animationType = LOADINGBAR;
            animation->start();
        } else {
            animation->setColor(redValue, greenValue, blueValue);
            animation->setSpeed(speed);
            animation->setIntensity(intensity);
        }
    } else if (animationIndicator > 90 && animationIndicator <= 120){ //PINGPONG
        float speed = map(animationIndicator, 91, 120, 3.0, 40.0);
        if (animationType != PINGPONG) {
            deleteAnimation();
            animation = new AnimationPingPong(strip, numPixels, redValue, greenValue, blueValue, intensity, speed);
            animationType = PINGPONG;
            animation->start();
        } else {
            animation->setColor(redValue, greenValue, blueValue);
            animation->setSpeed(speed);
            animation->setIntensity(intensity);
        }
    } else if (animationIndicator > 120 && animationIndicator <= 150){ //SPARKLE
        float speed = map(animationIndicator, 121, 150, 1.0, 40.0);
        if (animationType != SPARKLE) {
            deleteAnimation();
            animation = new AnimationSparkle(strip, numPixels, redValue, greenValue, blueValue, intensity, speed);
            animationType = SPARKLE;
            animation->start();
        } else {
            animation->setColor(redValue, greenValue, blueValue);
            animation->setSpeed(speed);
            animation->setIntensity(intensity);
        }
    } else if (animationIndicator > 150 && animationIndicator <= 180){ //STARS
        float speed = map(animationIndicator, 151, 180, 1.0, 30.0);
        if (animationType != STARS) {
            deleteAnimation();
            animation = new AnimationStars(strip, numPixels, redValue, greenValue, blueValue, intensity, speed);
            animationType = STARS;
            animation->start();
        } else {
            animation->setColor(redValue, greenValue, blueValue);
            animation->setSpeed(speed);
            animation->setIntensity(intensity);
        }
    } else if (animationIndicator > 180 && animationIndicator <= 210){ //STROBE
        float speed = map(animationIndicator, 181, 210, 1.0, 29.0);
        if (animationType != STROBE) {
            deleteAnimation();
            animation = new AnimationStrobe(strip, numPixels, redValue, greenValue, blueValue, intensity, speed);
            animationType = STROBE;
            animation->start();
        } else {
            animation->setColor(redValue, greenValue, blueValue);
            animation->setSpeed(speed);
            animation->setIntensity(intensity);
        }
    } 
    else {
        noAnimation(intensity, redValue, greenValue, blueValue);
    }
}