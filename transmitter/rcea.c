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



//TODO
/**@brief Function for sending radio packet. 
 */
void rcea_send_packet(void) {

    NRF_LOG_INFO("Func.");
    
    count++;

    NRF_LOG_INFO("%d", count);

    radio_send_packet(count);

    NRF_LOG_INFO("SPIM");

    spim_receive_packet(m_rx_packet, RADIO_MAX_PAYLOAD_LEN, count);



}


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


//TODO
/**@brief Function for setting radio mode.
 */
void rcea_set_mode(nrf_radio_mode_t mode) {
    count++;

    radio_config_mode(mode);

//    spim_transfer_mode(mode, count); 
}


/**@brief Function for setting radio output power.
 */
void rcea_set_tx_power(nrf_radio_txpower_t txpower) {

    radio_config_tx_power(txpower);

}


//TODO check channel, return error code?
/**@brief Function for setting radio channel.
 */
void rcea_set_channel(uint8_t channel) {
    count++;

    radio_config_channel(channel);

    spim_transfer_channel(channel, count);
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


