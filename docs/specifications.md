# **Spécifications LumHub**

## **1. Gestion de l’éclairage des tubes LEDs**
### **1.1. Allumer une LED spécifique**
- Permettre l’allumage d’une LED unique dans un tube.
- Paramètres : couleur (RVB), intensité lumineuse.

### **1.2. Allumer un tube entier**
- Activer toutes les LEDs d’un tube avec une couleur et une intensité uniformes.

### **1.3. Contrôler une LED individuellement**
- Modifier la couleur et l’intensité d’une LED en temps réel.

### **1.4. Contrôler un tube entier**
- Changer la couleur et l’intensité de toutes les LEDs d’un tube simultanément.

### **1.5. Créer des animations sur un tube**
- Générer des effets lumineux programmés (flash, pulsation).
- Gestion des animations en temps réel avec ajustement des paramètres.

---

## **2. Gestion des animations**
### **2.1. Jouer une animation en boucle sur un tube**
- Exécuter une animation prédéfinie en boucle.
- Gestion du démarrage, de l’arrêt et du redémarrage de l’animation.

### **2.2. Faire varier les paramètres de l’animation**
- Ajustement dynamique des paramètres :
  - **Vitesse** : modifier la rapidité de l’animation.
  - **Couleurs** : choisir une palette ou laisser les couleurs évoluer dynamiquement.

### **2.3. Associer une animation à chaque tube**
- Chaque tube peut exécuter une animation indépendante.

---

## **3. Implémentation d’un système DMX**
### **3.1. Création de canaux DMX pour les commandes**
- Simulation de canaux DMX sans connexion physique :
  - Intensité (0-255)
  - Rouge (0-255)
  - Vert (0-255)
  - Bleu (0-255)
  - Effet (sélection des animations)

### **3.2. Association des canaux DMX**
- Mapper chaque canal DMX à une action spécifique sur LumHub.
- Possibilité de sauvegarder et de charger des profils de configuration DMX.
