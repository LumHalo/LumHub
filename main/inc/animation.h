#ifndef ANIMATION_H
#define ANIMATION_H
#include <cstdint>

// Classe Animation servant de base pour toutes les animations. 
class Animation {
  public:
    bool isRunning;                   // État de l’animation

    Animation();
    // Démarrer l’animation
    virtual void start();
    // Arrêter l’animation
    virtual void stop();
    // Mettre à jour l’animation
    virtual void update();
    // Mettre a jour les couleurs
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);
    // Mettre a jour la vitesse
    virtual void setSpeed(float speed);
    // Mettre a jour l'intensité
    virtual void setIntensity(uint8_t i);
    // Vérifier si l'animation est en cours
    bool isActive() const;
    // Destructeur virtuel pour assurer une destruction propre des objets dérivés
    virtual ~Animation() {} 

};

enum AnimationType {
  NONE,
  BREATHING,
  SWEEPUP,
  LOADINGBAR,
  PINGPONG,
  SPARKLE,
  STARS,
  STROBE
};


#endif // ANIMATION_H
