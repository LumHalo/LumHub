# LumHub

LumHub est un projet permettant de gérer des tubes LEDs adressables WS2814 RGBW à l'aide d'une architecture modulaire. Il permet de contrôler jusqu'à 4 tubes simultanément avec différentes animations.

## Fonctionnalités principales
- Gestion de LEDs individuellement ou au complet.
- Prise en charge de LED WS2814 RGBW.
- Contrôle jusqu'à 4 tubes simultanément.
- Animations dynamiques : flash, respiration, lumière fixe.
- Code structuré en classes pour une extensibilité facile.

## Arborescence du projet
```
LumHub/
├main/
| ├─inc/                   # Code d'en-tête principal
│ │ ├── fixture.h              # Gestion d'un ruban
│ │ ├── fixtureManagement.h    # Gestion des fixtures
│ │ ├── serialManagement.h     # Communication via DMX
│ │ ├── main.h                 # Programme principal pour Arduino
| ├─src/                   # Code source principal
│ │ ├── fixture.cpp            # Gestion d'un ruban
│ │ ├── fixtureManagement.cpp  # Gestion des fixtures
│ │ ├── serialManagement.cpp   # Communication via DMX
│ ├── main.ino         # Programme principal pour Arduino
│
├── docs/                # Documentation du projet
├── README.md            # Présentation du projet
```

## Installation et utilisation

### Lib

FastLED

### Matériel requis
- ESP32
- Tubes LEDs WS2814
- Alimentation adaptée aux LEDs

### Déploiement
1. Cloner le dépôt :
   ```sh
   git clone https://github.com/LumHalo/LumHub.git
   ```
2. Ouvrir `main.ino` dans l'IDE Arduino.
3. Installer les bibliothèques nécessaires :
   - `FastLED`
4. Configurer les paramètres dans le code (nombre de tubes, type de LED, mode, etc.).
5. Compiler et téléverser sur l'ESP32.

## Contribuer
Les contributions sont les bienvenues ! N'hésitez pas à proposer des améliorations, corrections ou nouvelles animations.

## Licence
Ce projet est sous licence MIT - voir le fichier LICENSE pour plus de détails.

