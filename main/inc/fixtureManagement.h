#ifndef FIXTUREMANAGEMENT_H
#define FIXTUREMANAGEMENT_H

#include "fixture.h"


class FixtureManagement {
public:
    uint8_t MAX_FIXTURE = 4; // Nombre maximum de fixtures
    FixtureManagement(); // Constructeur
    void addFixture(Fixture* fixture); // Ajouter une fixture
    void dispatchDMXData(uint8_t* dmxData); // Dispatcher les données DMX

private:
    Fixture* fixture[4]; // Tableau de fixtures
    uint8_t fixtureCount = 0;
};

#endif // FIXTUREMANAGEMENT_H
