#include "../inc/fixtureManagement.h"

FixtureManagement::FixtureManagement() {
    for (int i = 0; i < MAX_FIXTURE; i++) {
        fixture[i] = nullptr;
    }
}

void FixtureManagement::addFixture(Fixture* fixture) {
    for (int i = 0; i < MAX_FIXTURE; i++) {
        if (this->fixture[i] == nullptr) {
            this->fixture[i] = fixture;
            fixtureCount += 1;
            break;
        }
    }
}

void FixtureManagement::dispatchDMXData(uint8_t* dmxData) {
    for (int i = 0; i < fixtureCount; i++) {
        if (fixture[i] != nullptr) {
            uint8_t dmxStart = fixture[i]->getDMXStart();
            if (fixture[i]->getMode() == 1) {
                uint8_t dmxDataFixture[60];
                memcpy(dmxDataFixture, dmxData + dmxStart, 60);
                fixture[i]->processDMXData(dmxDataFixture);
            } else if (fixture[i]->getMode() == 2) {
                uint8_t dmxDataFixture[65];
                memcpy(dmxDataFixture, dmxData + dmxStart, 65);
                fixture[i]->processDMXData(dmxDataFixture);
            } else {
                uint8_t dmxDataFixture[5] = {
                    dmxData[dmxStart],
                    dmxData[dmxStart + 1],
                    dmxData[dmxStart + 2],
                    dmxData[dmxStart + 3],
                    dmxData[dmxStart + 4]
                };
                fixture[i]->processDMXData(dmxDataFixture);
            }
        }
    }
}
