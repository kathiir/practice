#ifndef RADIO_H
#define RADIO_H

#include "nrf_radio.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Indicates devices that support 802.15.4 radio mode. */
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
    #define EXTENDED_SUPPORT 1
#else
    #define EXTENDED_SUPPORT 0
#endif



#define IEEE_DEFAULT_FREQ         (5)   /**< IEEE 802.15.4 default frequency. */

#define IEEE_FREQ_CALC(_channel) (IEEE_DEFAULT_FREQ + \
                                  (IEEE_DEFAULT_FREQ * ((_channel) - IEEE_MIN_CHANNEL)))
#define CHAN_TO_FREQ(_channel)   (2400 + _channel)                                       /**< Frequency calculation for a given channel. */

#define RADIO_LENGTH_LENGTH_FIELD (8UL) /**< Length on air of the LENGTH field. */


#define RADIO_MAX_PAYLOAD_LEN     249   /**< Maximum radio RX or TX payload. Had to decrease due to SPI limitations */
#define IEEE_MAX_PAYLOAD_LEN      127   /**< IEEE 802.15.4 maximum payload length. */
#define IEEE_MIN_CHANNEL          11    /**< IEEE 802.15.4 minimum channel. */
#define IEEE_MAX_CHANNEL          26    /**< IEEE 802.15.4 maximum channel. */


/**@brief Radio configuration. */
typedef struct {

    nrf_radio_mode_t mode;
    nrf_radio_txpower_t txpower;
    uint8_t channel; 

} radio_config_t;


/**@brief Function for initializing radio module.
 */
void radio_init(radio_config_t * p_config);


//TODO
/**@brief Function for sending radio packet.
 *//*
void send_packet(nrf_radio_mode_t mode,
                 nrf_radio_txpower_t txpower,
                 uint8_t channel);
*/
void radio_send_packet(uint16_t count);


bool radio_set_message(uint8_t * message, size_t size);

void radio_generate_random_message();


/**@brief Function for setting radio mode.
 */
void radio_config_mode(nrf_radio_mode_t mode);


/**@brief Function for setting radio channel.
 */
void radio_config_channel(uint8_t channel);


/**@brief Function for setting radio output power.
 */
void radio_config_tx_power(nrf_radio_txpower_t txpower);


#ifdef __cplusplus
}
#endif

#endif