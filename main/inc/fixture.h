#ifndef FIXTURE_H
#define FIXTURE_H

#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_RMT_BUILTIN_DRIVER 1
#include <FastLED.h>
#include <cstdint>
#include "animation.h"

class AnimationBreathing; //TODO supprimer ?

class Fixture {
public:
    Fixture() : id(0), dmxStart(0), mode(0), strip(nullptr), numPixels(0), animation(nullptr), animationType(NONE) {} // Constructeur par défaut

    Fixture(uint8_t id, uint8_t dmxStart, uint8_t mode, uint8_t numPixels, CRGB* strip); // Constructeur
    uint8_t getDMXStart(); // Obtenir l'adresse DMX de départ
    void setDMXStart(uint8_t dmxStart); // Définir l'adresse DMX de départ
    void processDMXData(uint8_t* dmxData); // Traiter les données DMX
    void noAnimation(uint8_t intensity, uint8_t redValue, uint8_t greenValue, uint8_t blueValue); // Désactiver l'animation
    void deleteAnimation();
    void activateAnimation(uint8_t animationIndicator, uint8_t intensity, uint8_t redValue, uint8_t greenValue, uint8_t blueValue); // Activer une animation
    uint8_t getMode();

private:
    uint8_t id;              // Identifiant de la fixture
    uint8_t numPixels;       // Nombre de LEDs
    uint8_t dmxStart;        // Adresse DMX de départ
    uint8_t mode = 0;            // Mode du fixture
    // Mode :
    // 0 -> 5 canaux
    // 1 -> 60 canaux : 3 (rgb) * 20
    // 2 -> 65 canaux : intensité, r, g, b, animation + 3 (rgb) * 20
    CRGB* strip; // Pointeur NeoPixel
    Animation* animation;     // Animation en cours
    AnimationType animationType; // Type d'animation en cours
    uint8_t previousDmxData[65] = {};

    bool previousCheck(uint8_t* dmxStart);
    void setPreviousData(uint8_t* dmxStart);
    void setPixels(uint8_t* dmxStart, uint8_t intensity);
};

#endif // FIXTURE_H
