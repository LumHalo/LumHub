#include "../inc/serverManagement.h"

ServerManagement::ServerManagement() {}

void ServerManagement::begin(FixtureManagement& fixtureManagement) {
    /* WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.print("Reseau WIFI créé :");
    Serial.println(ssid); */

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nWiFi connecté !");
    Serial.print("Adresse IP: ");
    /* Serial.println(WiFi.softAPIP()); */
    Serial.println(WiFi.localIP());

    // Démarrage de l'écoute UDP avec AsyncUDP
    if (udp.listen(ARTNET_PORT)) {
        Serial.print("Écoute sur le port UDP ");
        Serial.println(ARTNET_PORT);

        /* udp.onPacket([&fixtureManagement, &serialManagement](AsyncUDPPacket packet) {
            uint8_t* packetBuffer = packet.data();
            int packetSize = packet.length();

            // Vérification de l'entête Art-Net "Art-Net\0"
            if (packetSize >= 8 && 
                packetBuffer[0] == 'A' && packetBuffer[1] == 'r' &&
                packetBuffer[2] == 't' && packetBuffer[3] == '-' &&
                packetBuffer[4] == 'N' && packetBuffer[5] == 'e' &&
                packetBuffer[6] == 't' && packetBuffer[7] == 0x00) {
                // Si c'est un paquet DMX (ArtDmx)
                if (packetBuffer[8] == 0x00 && packetBuffer[9] == 0x50) {
                    fixtureManagement.dispatchDMXData(&packetBuffer[18]);
                    //serialManagement.dispatchDMXData(&packetBuffer[18]);
                }
            }
        }); */
        udp.onPacket([&fixtureManagement](AsyncUDPPacket packet) {
            uint8_t* packetBuffer = packet.data();
            int packetSize = packet.length();

            if (strncmp((char*)packetBuffer, "Art-Net", 7) == 0) {
                uint16_t opcode = packetBuffer[9] << 8 | packetBuffer[8];

                switch (opcode) {
                    case 0x5000: { // ArtDMX
                        if (packetSize >= 18) {
                            uint16_t universe = packetBuffer[14] | (packetBuffer[15] << 8);
    
                            fixtureManagement.dispatchDMXData(&packetBuffer[17]);
                        }
                        break;
                    }
    
                    default:
                        Serial.print("Opcode inconnu : 0x");
                        Serial.println(opcode, HEX);
                        break;
                }
            }
        });
    }
}