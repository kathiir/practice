#include "radio.h"

#include <stdbool.h>
#include "nrf.h"
#include "nrf_log.h"


#include "nrf_rng.h"



static uint8_t * m_p_tx_packet;


static const radio_config_t * m_p_config = NULL;

static bool radio_random_message = true;


/**@brief Function for generating an 8-bit random number with the internal random generator.
 */
static uint8_t rnd8(void)
{
    nrf_rng_event_clear(NRF_RNG_EVENT_VALRDY);

    while (!nrf_rng_event_get(NRF_RNG_EVENT_VALRDY))
    {
        /* Do nothing. */
    }

    return nrf_rng_random_value_get();
}
                                 

/**@brief Function for disabling radio and clearing events.
 */
static void radio_disable(void)
{
    nrf_radio_shorts_set(0);
    nrf_radio_int_disable(~0);
    nrf_radio_event_clear(NRF_RADIO_EVENT_DISABLED);

    nrf_radio_task_trigger(NRF_RADIO_TASK_DISABLE);
    while (!nrf_radio_event_check(NRF_RADIO_EVENT_DISABLED))
    {
        /* wait */
    }

    nrf_radio_event_clear(NRF_RADIO_EVENT_DISABLED);
}


/**@brief Function for setting radio channel. Inner.
 */
static void radio_channel_set(nrf_radio_mode_t mode, uint8_t channel)
{
#if EXTENDED_SUPPORT
    if (mode == NRF_RADIO_MODE_IEEE802154_250KBIT)
    {
        if ((channel >= IEEE_MIN_CHANNEL) && (channel <= IEEE_MAX_CHANNEL))
        {
            nrf_radio_frequency_set(CHAN_TO_FREQ(IEEE_FREQ_CALC(channel)));
        }
        else
        {
            nrf_radio_frequency_set(CHAN_TO_FREQ(IEEE_DEFAULT_FREQ));
        }
    } else {
        nrf_radio_frequency_set(CHAN_TO_FREQ(channel));
    }
#else
    nrf_radio_frequency_set(CHAN_TO_FREQ(channel));
#endif
}


//TODO diff modes
/**@brief Function for configuring the radio packet.
 */
static void radio_config(nrf_radio_mode_t mode)
{
    nrf_radio_packet_conf_t packet_conf;

    nrf_radio_modecnf0_set(false, RADIO_MODECNF0_DTX_Center);
    nrf_radio_crc_configure(RADIO_CRCCNF_LEN_Two, NRF_RADIO_CRC_ADDR_SKIP, 0x1021);


    nrf_radio_txaddress_set(0);
    nrf_radio_rxaddresses_set(1);

// TODO 
    nrf_radio_prefix0_set(0xCC);
    nrf_radio_base0_set(0xCCCCCCCC);

    memset(&packet_conf, 0, sizeof(packet_conf));
    packet_conf.lflen = RADIO_LENGTH_LENGTH_FIELD;
    packet_conf.maxlen = RADIO_MAX_PAYLOAD_LEN - 1;
    packet_conf.statlen = 0;
    packet_conf.balen = 4;
    packet_conf.big_endian = true;
    packet_conf.whiteen = true;


    switch (mode)
    {
    #if EXTENDED_SUPPORT
        case NRF_RADIO_MODE_IEEE802154_250KBIT:
            /* Packet configuration:
             * S1 size = 0 bits,
             * S0 size = 0 bytes,
             * 32-bit preamble.
             */
            packet_conf.plen = NRF_RADIO_PREAMBLE_LENGTH_32BIT_ZERO;
            packet_conf.maxlen = IEEE_MAX_PAYLOAD_LEN;

            /* Set fast ramp-up time. */
            nrf_radio_modecnf0_set(true, RADIO_MODECNF0_DTX_Center);
            break;
    #endif
        default:
            /* Packet configuration:
             * S1 size = 0 bits,
             * S0 size = 0 bytes,
             * 8-bit preamble.
             */
            packet_conf.plen = NRF_RADIO_PREAMBLE_LENGTH_8BIT;
            break;
    }


    nrf_radio_packet_configure(&packet_conf);

}


/**@brief Function for generating packet payload.
 */
static void generate_packet(nrf_radio_mode_t mode, uint16_t count)
{

#if EXTENDED_SUPPORT
    if (mode == NRF_RADIO_MODE_IEEE802154_250KBIT)
    {
        m_p_tx_packet[0] = IEEE_MAX_PAYLOAD_LEN - 1;
    }
    else
    {
        m_p_tx_packet[0] = RADIO_MAX_PAYLOAD_LEN - 1;
    }
#else
    m_p_tx_packet[0] = RADIO_MAX_PAYLOAD_LEN - 1;
#endif

    memcpy(m_p_tx_packet + 1, &count, sizeof(count));

    if (radio_random_message) {
        for (uint8_t i = 3; i < RADIO_MAX_PAYLOAD_LEN; i++)
        {
            //m_p_tx_packet[i] = rnd8();
            m_p_tx_packet[i] = 0xAA;
        }
    }

    nrf_radio_packetptr_set(m_p_tx_packet);
}


//TODO check size, include crc and lenght with packet number
bool radio_set_message(uint8_t * message, size_t size) {

    if (size >= RADIO_MAX_PAYLOAD_LEN - 3) {
        return false;
    }

    radio_random_message = false;

    memcpy(m_p_tx_packet + 3, message, size);

    return true;
}


void radio_generate_random_message() {
    radio_random_message = true;
}


/**@brief Function for setting radio mode.
 */
void radio_config_mode(nrf_radio_mode_t mode) {

    radio_disable();
    radio_config(mode);
    
    
    nrf_radio_mode_set(mode);
}


/**@brief Function for setting radio channel.
 */
void radio_config_channel(uint8_t channel) {
    radio_disable();

    radio_channel_set(m_p_config->mode, channel);

}


/**@brief Function for setting radio output power.
 */
void radio_config_tx_power(nrf_radio_txpower_t txpower) {

    radio_disable();

    nrf_radio_txpower_set(txpower);
}


// TODO
/**@brief Function for sending radio packet.
 *//*
void send_packet(nrf_radio_mode_t mode,
                 nrf_radio_txpower_t txpower,
                 uint8_t channel)
{
    radio_disable();
    generate_packet(mode);

    switch (mode)
    {
#if EXTENDED_SUPPORT
        case NRF_RADIO_MODE_IEEE802154_250KBIT:
            nrf_radio_shorts_enable(NRF_RADIO_SHORT_READY_START_MASK |
                                    NRF_RADIO_SHORT_PHYEND_START_MASK);
            break;
#endif
        default:
#ifdef NRF52832_XXAA
        case NRF_RADIO_MODE_NRF_250KBIT:
#endif
            nrf_radio_shorts_enable(NRF_RADIO_SHORT_READY_START_MASK |
                                    NRF_RADIO_SHORT_END_START_MASK);
            break;
    }

    nrf_radio_mode_set(mode);
    nrf_radio_txpower_set(txpower);

    radio_channel_set(mode, channel);


    nrf_radio_event_clear(NRF_RADIO_EVENT_END);
    nrf_radio_int_enable(NRF_RADIO_INT_END_MASK);

    nrf_radio_task_trigger(NRF_RADIO_TASK_TXEN);


    while (!nrf_radio_event_check(NRF_RADIO_EVENT_END))
    {
        // wait 
    }

}
*/

void radio_send_packet(uint16_t count)
{
    generate_packet(m_p_config->mode, count);

    switch (m_p_config->mode)
    {
#if SUPPORT_IEEE802154_250KBIT
        case NRF_RADIO_MODE_IEEE802154_250KBIT:
            nrf_radio_shorts_enable(NRF_RADIO_SHORT_READY_START_MASK |
                                    NRF_RADIO_SHORT_PHYEND_START_MASK);
            break;
#endif
        default:
#ifdef NRF52832_XXAA
        case NRF_RADIO_MODE_NRF_250KBIT:
#endif
            nrf_radio_shorts_enable(NRF_RADIO_SHORT_READY_START_MASK |
                                    NRF_RADIO_SHORT_END_START_MASK);
            break;
    }


    nrf_radio_event_clear(NRF_RADIO_EVENT_END);
    nrf_radio_int_enable(NRF_RADIO_INT_END_MASK);

    nrf_radio_task_trigger(NRF_RADIO_TASK_TXEN);

    while (!nrf_radio_event_check(NRF_RADIO_EVENT_END))
    {
        /* wait */ 
    }

}



//TODO
/**@brief Interrupt handler.
 */
void RADIO_IRQHandler(void)
{
    if (nrf_radio_event_check(NRF_RADIO_EVENT_END))
    {
        nrf_radio_event_clear(NRF_RADIO_EVENT_END);


        NRF_LOG_RAW_INFO("Delivered.\n");

        //TODO
        radio_disable();
        
    }
}


/**@brief Function for initializing radio module.
 */
void radio_init(radio_config_t * p_config, uint8_t * p_tx_packet)
{
    if (!m_p_config)
    {

        nrf_rng_task_trigger(NRF_RNG_TASK_START);

        NVIC_EnableIRQ(RADIO_IRQn);

        m_p_config = p_config;
        
        radio_config_mode(m_p_config->mode);
        radio_config_channel(m_p_config->channel);
        radio_config_tx_power(m_p_config->txpower);

        m_p_tx_packet = p_tx_packet;

    }
    else
    {
        // Already initialized, do nothing
    }
}