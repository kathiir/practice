#include "radio.h"

#include <stdbool.h>
#include "nrf.h"
#include "nrf_log.h"


#include "boards.h"

#include "spis.h"

static uint8_t * m_p_rx_packet = NULL;


static radio_config_t * m_p_config = NULL;

static volatile bool packet_received;


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


void receive_packet() {
    packet_received = false;

}


void radio_rx(nrf_radio_mode_t mode, uint8_t channel)
{
    radio_disable();

    nrf_radio_mode_set(mode);

    nrf_radio_shorts_enable(NRF_RADIO_SHORT_READY_START_MASK |
                            NRF_RADIO_SHORT_END_START_MASK);

    nrf_radio_packetptr_set(m_p_rx_packet);

    radio_config(mode);
    radio_channel_set(mode, channel);

    nrf_radio_int_enable(NRF_RADIO_INT_CRCOK_MASK);

    nrf_radio_int_enable(NRF_RADIO_INT_CRCERROR_MASK);

    nrf_radio_task_trigger(NRF_RADIO_TASK_RXEN);

}


bool radio_check_packet_received(void) {
    return packet_received;
}


void RADIO_IRQHandler(void)
{
    if (nrf_radio_event_check(NRF_RADIO_EVENT_CRCOK))
    {
        packet_received = true;
        nrf_radio_event_clear(NRF_RADIO_EVENT_CRCOK);
       
        bsp_board_led_invert(BSP_BOARD_LED_3);
        
    }

    if (nrf_radio_event_check(NRF_RADIO_EVENT_CRCERROR))
    {
        packet_received = true;
        nrf_radio_event_clear(NRF_RADIO_EVENT_CRCERROR);
       
        bsp_board_led_invert(BSP_BOARD_LED_3);
    }
}


void radio_init(radio_config_t * p_config, uint8_t * p_rx_packet){


    if (!m_p_config)
    {
        NVIC_EnableIRQ(RADIO_IRQn);

        m_p_config = p_config;

        m_p_rx_packet = p_rx_packet;

        m_p_config->mode = NRF_RADIO_MODE_NRF_250KBIT;
        m_p_config->channel = 11;

    }
    else
    {
        // Already initialized, do nothing
    }

}

