#ifndef SPIS_H
#define SPIS_H

#include "nrfx_spis.h"

#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif


#define RCEA_SPI_EMPTY             0
#define RCEA_SPI_ACKNOWLEDGE       1
#define RCEA_SPI_CONFIG_CHANNEL    2
#define RCEA_SPI_CONFIG_MODE       3
#define RCEA_SPI_WAITING_FOR_ACK   4
#define RCEA_SPI_PACKET            5


typedef enum {
    EMPTY = RCEA_SPI_EMPTY,
    ACKNOWLEDGE = RCEA_SPI_ACKNOWLEDGE,
    CONFIG_MODE = RCEA_SPI_CONFIG_CHANNEL,
    CONFIG_CHANNEL = RCEA_SPI_CONFIG_MODE,
    WAIT_ACK = RCEA_SPI_WAITING_FOR_ACK,
    PACKET = RCEA_SPI_PACKET
} spi_message_t;


typedef struct {
    spi_message_t type;
    union {
        nrf_radio_mode_t mode;
        uint8_t channel;
        bool success;
    } params;
} spi_payload_t;




void spis_init();

bool spis_check_config_received();

spi_payload_t spis_get_payload();

void spis_set_tx_message(uint8_t * p_packet, size_t length);

void spis_receive_ack();

void spis_receive_config();


#ifdef __cplusplus
}
#endif

#endif