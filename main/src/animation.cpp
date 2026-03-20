#include "../inc/animation.h"

// Constructeur
Animation::Animation() : isRunning(false) {}

// Démarrer l’animation
void Animation::start() {
    isRunning = true;
}

// Arrêter l’animation
void Animation::stop() {
    isRunning = false;
}

void Animation::update() {
}

bool Animation::isActive() const {
    return isRunning;
}

void Animation::setColor(uint8_t r, uint8_t g, uint8_t b) {
}

void Animation::setSpeed(float speed) {
}

void Animation::setIntensity(uint8_t i){
}


