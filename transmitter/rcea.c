#include "rcea.h"

#include "boards.h"



#include <stdbool.h>
#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "nrf_delay.h"


#include "spim.h"


static radio_config_t m_config;

static volatile uint16_t count = 0;

static uint8_t m_tx_packet[RADIO_MAX_PAYLOAD_LEN];
static uint8_t m_rx_packet[RADIO_MAX_PAYLOAD_LEN]; 


/**@brief Function for comparing radio packet. 
 */
bool rcea_analyze_packet() {
    for (uint8_t i = 0; i < m_tx_packet[0]; i++) {
        if (m_tx_packet[i] != m_rx_packet[i]) {
            return false;
        }
    }
    return true;    
}


/**@brief Function for sending radio packet. 
 */
bool rcea_send_packet(void) {

    NRF_LOG_INFO("Sending packet");
    
    count++;

    radio_send_packet(count);

    bool received = spim_receive_packet(m_rx_packet, RADIO_MAX_PAYLOAD_LEN, count);

    if (received) {
        return rcea_analyze_packet();
    }

    return false;
}


/**@brief Function for checking if radio channel is valid.
 */
bool rcea_check_channel(uint8_t channel) {

#if EXTENDED_SUPPORT
    if (m_config.mode == NRF_RADIO_MODE_IEEE802154_250KBIT)
    {
        if ((channel >= IEEE_MIN_CHANNEL) && (channel <= IEEE_MAX_CHANNEL)) {
            return true;
        }
        return false;
    }
#endif
    return true;

}

/**@brief Function for checking if message length is valid.
 */
bool rcea_check_length(uint8_t length) {

#if EXTENDED_SUPPORT
    if (m_config.mode == NRF_RADIO_MODE_IEEE802154_250KBIT && (length >= IEEE_MAX_PAYLOAD_LEN - 3))
    {
        return false;
    }
#endif
    if (length >= RADIO_MAX_PAYLOAD_LEN - 3) {
        return false;
    }
    return true;

}


/**@brief Function for setting radio mode.
 */
void rcea_set_mode(nrf_radio_mode_t mode) {
    count++;

    radio_config_mode(mode);

    spim_transfer_mode(mode, count); 
}


/**@brief Function for setting radio output power.
 */
void rcea_set_tx_power(nrf_radio_txpower_t txpower) {
    radio_config_tx_power(txpower);
}


/**@brief Function for setting radio channel.
 */
void rcea_set_channel(uint8_t channel) {
    count++;

    radio_config_channel(channel);

    spim_transfer_channel(channel, count);
}

/**@brief Function for setting message.
 */
bool rcea_set_message(uint8_t * message, size_t size) {
    return radio_set_message(message, size);
}

/**@brief Function for setting generation of random message.
 */
void rcea_generate_random_message(void) {
    radio_generate_random_message();
}


/**@brief Function for getting radio channel.
 */
uint8_t rcea_get_channel() {
    return m_config.channel;
}

/**@brief Function for getting radio output power.
 */
nrf_radio_txpower_t rcea_get_txpower() {
    return m_config.txpower;
}

/**@brief Function for getting radio mode.
 */
nrf_radio_mode_t rcea_get_mode() {
    return m_config.mode;
}



/**@brief Function for initializing rcea module and etc.
 */
void rcea_init(void) {

    spim_init();

    m_config.channel = 11;
    m_config.mode = NRF_RADIO_MODE_NRF_250KBIT;
    m_config.txpower = NRF_RADIO_TXPOWER_0DBM;

    radio_init(&m_config, m_tx_packet);
}


