#ifndef RADIO_H
#define RADIO_H

#include "nrf_radio.h"

/*
#include "nrfx_spis.h"
*/
#ifdef __cplusplus
extern "C" {
#endif


/** Indicates devices that support 802.15.4 radio mode. */
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
    #define SUPPORT_IEEE802154_250KBIT 1
#else
    #define SUPPORT_IEEE802154_250KBIT 0
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
    uint8_t channel; 

} radio_config_t;

/**@brief Function for getting ready to receive packet.
 */
void receive_packet(void);

/**@brief Function for enebling RX mode.
 */
void radio_rx(nrf_radio_mode_t mode, uint8_t channel);

/**@brief Function for initializing radio module.
 */
void radio_init(radio_config_t * p_config, uint8_t * p_rx_packet);

/**@brief Function checking if packet was received.
 */
bool radio_check_packet_received(void);


#ifdef __cplusplus
}
#endif

#endif