#ifndef SERVERMANAGEMENT_H
#define SERVERMANAGEMENT_H

#include <WiFi.h>
#include <AsyncUDP.h>
#include "fixture.h"
#include "fixtureManagement.h"

class ServerManagement {
  public:
    ServerManagement();
    void begin(FixtureManagement& fixtureManagement);

  private:
    AsyncUDP udp;
    const unsigned int ARTNET_PORT = 6454;
    const int PACKET_SIZE = 530; // Taille max d'un paquet Art-Net
    // Configuration Wi-Fi
    const char* ssid = "LumHub";
    const char* password = "";
};

#endif // SERVERMANAGEMENT_H