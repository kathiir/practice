#ifndef SPIS_H
#define SPIS_H

#include "nrfx_spis.h"

#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    EMPTY,
    CONFIG_MODE,
    CONFIG_CHANNEL,
    PACKET_SENT,
    CONFIRMATION
} spi_message_t;


typedef struct {
    spi_message_t type;

    union {
        nrf_radio_mode_t mode;
        uint8_t channel;
        struct {

        } packet;
    } params;

    uint8_t end;
    
} spi_payload_t;



void spis_init();

bool spis_check_config_received();

spi_payload_t spis_get_payload();

void spis_set_tx_message(uint8_t * p_packet, size_t length);

void spis_receive_config();


#ifdef __cplusplus
}
#endif

#endif