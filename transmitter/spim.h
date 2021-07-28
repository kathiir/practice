#ifndef SPIM_H
#define SPIM_H

#include "nrfx_spim.h"

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



void spim_init();


void spim_transfer(spi_payload_t payload);

void spim_transfer_channel(uint8_t channel);

void spim_transfer_mode(nrf_radio_mode_t mode);

void spim_receive_packet(uint8_t * p_packet, size_t length);




#ifdef __cplusplus
}
#endif

#endif