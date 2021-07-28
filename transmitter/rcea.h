#ifndef RCEA_H
#define RCEA_H

#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif




void rcea_init();

void rcea_send_packet();

void rcea_set_mode(nrf_radio_mode_t mode);

void rcea_set_tx_power(nrf_radio_txpower_t txpower);

void rcea_set_channel(uint8_t channel);

/*
void set_config(nrf_radio_mode_t mode,
                nrf_radio_txpower_t txpower,
                uint8_t channel);

*/


#ifdef __cplusplus
}
#endif

#endif