#ifndef RCEA_H
#define RCEA_H

#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif



/**@brief Function for initializing rcea module and etc.
 */
void rcea_init(void);


//TODO
/**@brief Function for sending radio packet. 
 */
void rcea_send_packet(void);


/**@brief Function for setting radio mode.
 */
void rcea_set_mode(nrf_radio_mode_t mode);


/**@brief Function for setting radio output power.
 */
void rcea_set_tx_power(nrf_radio_txpower_t txpower);


/**@brief Function for setting radio channel.
 */
void rcea_set_channel(uint8_t channel);


/**@brief Function for checking if radio channel is valid.
 */
bool rcea_check_channel(uint8_t channel);


#ifdef __cplusplus
}
#endif

#endif