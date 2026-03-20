#include "../inc/serialManagement.h"

SerialManagement::SerialManagement() {
    if (dmxDataMutex == NULL) {
        dmxDataMutex = xSemaphoreCreateMutex();
    }
    _active = false;
    _fixtureManagement = nullptr;
}

void SerialManagement::initOutput(uint8_t dmxTxPin, uart_port_t uartNum) {
    _active = true;
    _dmxTxPin = dmxTxPin;
    _uartNum = uartNum;

    if (dmxDataMutex == NULL) dmxDataMutex = xSemaphoreCreateMutex();
	
    if (xTaskCreatePinnedToCore(send_dmx, "sendDMX", 4096, this, 1, &_xTaskHandle, 0) != pdPASS) {
        Serial.println("Failed to create task");
    }
}

void SerialManagement::setChannel(uint16_t channel, uint8_t data) {
	if (channel >= 1 && channel <= 512 && dmxDataMutex != NULL) {
        if (xSemaphoreTake(dmxDataMutex, portMAX_DELAY)) {
            _data[channel] = data;
            xSemaphoreGive(dmxDataMutex);
        }
    }
}

void SerialManagement::clear(void) {
	memset(_data, 0, sizeof(_data));
}

bool SerialManagement::isActive() const {
    return _active;
}

void SerialManagement::dispatchDMXData(uint8_t* dmxData) {
    memcpy(_data + 1, dmxData, 512);
}

void SerialManagement::send_dmx(void * parameter) { 
	SerialManagement* instance = static_cast<SerialManagement*>(parameter);
    const uart_port_t uart_num = instance->_uartNum; //UART_NUM_1

    uart_config_t uart_config = {
        .baud_rate = 250000,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB
    };
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, instance->_dmxTxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, 1024, 0, 0, NULL, 0);

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT); // DE/RE
    gpio_set_level(GPIO_NUM_4, 1); // MAX485 en émission

    uint8_t dmx_frame[513];
    dmx_frame[0] = 0;

    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frame_period = pdMS_TO_TICKS(25); //40Hz AVANT 23 44Hz ~ 22.7ms

    while (instance->isActive()) {
        if (xSemaphoreTake(instance->dmxDataMutex, pdMS_TO_TICKS(5))) {
            memcpy(&dmx_frame[1], &instance->_data[1], 512);
            xSemaphoreGive(instance->dmxDataMutex);
        }
        
        uart_write_bytes_with_break(uart_num, (const char*)dmx_frame, 513, 100);
        vTaskDelayUntil(&last_wake_time, frame_period);
    }

    uart_driver_delete(uart_num);
    vTaskDelete(NULL);
}

void SerialManagement::initInput(uint8_t rxPin, uart_port_t uartNum, FixtureManagement* fixtureManagement) {
    _dmxRxPin = rxPin;
    _uartNum = uartNum;
    _fixtureManagement = fixtureManagement;
    _active = true;

    if (dmxDataMutex == NULL) dmxDataMutex = xSemaphoreCreateMutex();

    if (xTaskCreatePinnedToCore(receive_dmx, "receiveDMX", 4096, this, 1, &_xTaskHandle, 0) != pdPASS) {
        Serial.println("Failed to create RX task");
    }
}

void SerialManagement::receive_dmx(void* parameter) {
    SerialManagement* instance = static_cast<SerialManagement*>(parameter);
    const uart_port_t uart_num = instance->_uartNum; //UART_NUM_1

    // Config UART
    uart_config_t uart_config = {
        .baud_rate = 250000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB
    };

    uart_driver_install(uart_num, 1024 * 2, 0, 20, &instance->_uartQueue, 0);
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, UART_PIN_NO_CHANGE, instance->_dmxRxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_event_t event;
    uint8_t dmx_buffer[513];

    while (instance->isActive()) {
        if (xQueueReceive(instance->_uartQueue, (void *)&event, portMAX_DELAY)) {
            switch (event.type) {
                case UART_BREAK:
                    uart_flush_input(uart_num);
                    if (uart_read_bytes(uart_num, dmx_buffer, 513, pdMS_TO_TICKS(30)) == 513) {
                        if (dmx_buffer[0] == 0) {
                            if (xSemaphoreTake(instance->dmxDataMutex, pdMS_TO_TICKS(10))) {
                                memcpy(instance->_data, dmx_buffer, 513);
                                xSemaphoreGive(instance->dmxDataMutex);
                                
                                instance->_fixtureManagement->dispatchDMXData(instance->_data);
                            }
                        }
                    }
                    break;
                case UART_FIFO_OVF:
                case UART_BUFFER_FULL:
                    uart_flush_input(uart_num);
                    xQueueReset(instance->_uartQueue);
                    break;
                    
                default:
                    break;
            }

            /* if (event.type == UART_BREAK || event.type == UART_FRAME_ERR)
            {
                // Avant de lire la trame DMX, on vide le FIFO pour éviter toute donnée résiduelle.
                uart_flush(uart_num);

                // Lecture de la trame DMX complète (start code + 512 canaux)
                int len = uart_read_bytes(uart_num, dmx_buffer, 513, pdMS_TO_TICKS(20));
                if (len == 513)
                {
                    // Pour DMX classique, le start code doit être 0
                    if (dmx_buffer[0] == 0)
                    {
                        memcpy(instance->_data, dmx_buffer, sizeof(dmx_buffer));
                        instance->_fixtureManagement->dispatchDMXData(instance->_data);
                        // Exemple de traitement : affichage de la valeur du canal 1
                        //printf("DMX reçu : Canal 1 = %d\n", instance->_data[0]);
                        //printf("DMX reçu : Canal 2 = %d\n", instance->_data[1]);
                    }
                }
            } */
        }
    }

    uart_driver_delete(uart_num);
    vTaskDelete(NULL);
}

