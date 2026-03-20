#ifndef SERIALMANAGEMENT_H
#define SERIALMANAGEMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "fixtureManagement.h"

class SerialManagement {
  public:
    SerialManagement();

    void initOutput(uint8_t dmxTxPin, uart_port_t uartNum);
    void initInput(uint8_t dmxRxPin, uart_port_t uartNum, FixtureManagement* fixtureManagement);
    
    void setChannel(uint16_t channel, uint8_t value);
    void clear();
    bool isActive() const;
    void dispatchDMXData(uint8_t* dmxData);
    
  private:
    bool _active = false;

    uint8_t _data[513] = {0};

    uint8_t dmxEnablePin;
    uint8_t _dmxTxPin;
    uint8_t _dmxRxPin;
    TaskHandle_t _xTaskHandle;
    QueueHandle_t _uartQueue;
    uart_port_t _rxUartPort = UART_NUM_1;
    uart_port_t _txUartPort = UART_NUM_2;

    FixtureManagement* _fixtureManagement = nullptr;

    SemaphoreHandle_t dmxDataMutex = NULL;
    uart_port_t _uartNum;

    static void send_dmx(void* parameter);
    static void receive_dmx(void* parameter);
};

#endif // SERIALMANAGEMENT_H