#include "radio.h"

#include <stdbool.h>
#include "nrf.h"
#include "nrf_log.h"



static uint8_t m_tx_packet[RADIO_MAX_PAYLOAD_LEN];
static uint32_t m_tx_packet_cnt;


static const radio_config_t * m_p_config = NULL;
                                 


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


static void radio_channel_set(nrf_radio_mode_t mode, uint8_t channel)
{
#if SUPPORT_IEEE802154_250KBIT
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


/**@brief Function for configuring the radio.
 */
static void radio_config(nrf_radio_mode_t mode)
{
    nrf_radio_packet_conf_t packet_conf;

    nrf_radio_modecnf0_set(false, RADIO_MODECNF0_DTX_Center);
    nrf_radio_crc_configure(RADIO_CRCCNF_LEN_Disabled, NRF_RADIO_CRC_ADDR_INCLUDE, 0);


    nrf_radio_txaddress_set(0);
    nrf_radio_rxaddresses_set(1);

// TODO 
    nrf_radio_prefix0_set(0xCC);
    nrf_radio_base0_set(0xCCCCCCCC);

    memset(&packet_conf, 0, sizeof(packet_conf));
    packet_conf.lflen = RADIO_LENGTH_LENGTH_FIELD;
    packet_conf.maxlen = (sizeof(m_tx_packet) - 1);
    packet_conf.statlen = 0;
    packet_conf.balen = 4;
    packet_conf.big_endian = true;
    packet_conf.whiteen = true;


    switch (mode)
    {
    #if SUPPORT_IEEE802154_250KBIT
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


static void generate_packet(nrf_radio_mode_t mode)
{
    radio_config(mode);


#if SUPPORT_IEEE802154_250KBIT
    if (mode == NRF_RADIO_MODE_IEEE802154_250KBIT)
    {
        m_tx_packet[0] = IEEE_MAX_PAYLOAD_LEN - 1;
    }
    else
    {
        m_tx_packet[0] = sizeof(m_tx_packet) - 1;
    }
#else
    m_tx_packet[0] = sizeof(m_tx_packet) - 1;
#endif

    for (uint8_t i = 0; i < sizeof(m_tx_packet) - 1; i++)
    {
        m_tx_packet[i + 1] = 0xAF;
    }

    nrf_radio_packetptr_set(m_tx_packet);
}


void radio_config_mode(nrf_radio_mode_t mode) {

    radio_disable();
    radio_config(mode);
    
    
    nrf_radio_mode_set(mode);
}

void radio_config_channel(uint8_t channel) {
    radio_disable();

    radio_channel_set(m_p_config->mode, channel);

}

void radio_config_tx_power(nrf_radio_txpower_t txpower) {
    radio_disable();
    nrf_radio_txpower_set(txpower);
}


// TODO
void send_packet(nrf_radio_mode_t mode,
                 nrf_radio_txpower_t txpower,
                 uint8_t channel)
{
    radio_disable();
    generate_packet(mode);

    switch (mode)
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

    nrf_radio_mode_set(mode);
    nrf_radio_txpower_set(txpower);

    radio_channel_set(mode, channel);


    nrf_radio_event_clear(NRF_RADIO_EVENT_END);
    nrf_radio_int_enable(NRF_RADIO_INT_END_MASK);

    nrf_radio_task_trigger(NRF_RADIO_TASK_TXEN);


    while (!nrf_radio_event_check(NRF_RADIO_EVENT_END))
    {
        /* wait */
    }

}

/*
void send_packet()
{
    generate_packet(m_p_config->mode);

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
        /* wait */ /*
    }

}
*/

void RADIO_IRQHandler(void)
{
    if (nrf_radio_event_check(NRF_RADIO_EVENT_END))
    {
        nrf_radio_event_clear(NRF_RADIO_EVENT_END);

        

        NRF_LOG_RAW_INFO("Delivered.\n");

        
        //check num of packets
        radio_disable();
        
    }
}


void radio_init(radio_config_t * p_config)
{
    if (!m_p_config)
    {

        NVIC_EnableIRQ(RADIO_IRQn);
        //__enable_irq();

        m_p_config = p_config;
    }
    else
    {
        // Already initialized, do nothing
    }
}