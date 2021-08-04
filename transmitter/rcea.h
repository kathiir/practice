#ifndef RCEA_H
#define RCEA_H

#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif


/**@brief Function for initializing rcea module and etc.
 */
void rcea_init(void);


/**@brief Function for sending radio packet. 
 */
bool rcea_send_packet(void);


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

/**@brief Function for checking if message length is valid.
 */
bool rcea_check_length(uint8_t length);

/**@brief Function for setting message.
 */
bool rcea_set_message(uint8_t * message, size_t size);

/**@brief Function for getting radio channel.
 */
uint8_t rcea_get_channel();

/**@brief Function for getting radio output power.
 */
nrf_radio_txpower_t rcea_get_txpower();

/**@brief Function for getting radio mode.
 */
nrf_radio_mode_t rcea_get_mode();

/**@brief Function for setting generation of random message.
 */
void rcea_generate_random_message(void);



#ifdef __cplusplus
}
#endif

#endif