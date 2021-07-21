#include "radio.h"

#include <stdbool.h>
#include "nrf.h"
#include "nrf_log.h"


#include "boards.h"

static uint8_t m_rx_packet[RADIO_MAX_PAYLOAD_LEN];


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
    packet_conf.maxlen = (sizeof(m_rx_packet) - 1);
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


void radio_rx(nrf_radio_mode_t mode, uint8_t channel)
{
    radio_disable();

    nrf_radio_mode_set(mode);

    nrf_radio_shorts_enable(NRF_RADIO_SHORT_READY_START_MASK |
                            NRF_RADIO_SHORT_END_START_MASK);

    nrf_radio_packetptr_set(m_rx_packet);

    radio_config(mode);
    radio_channel_set(mode, channel);

    nrf_radio_int_enable(NRF_RADIO_INT_CRCOK_MASK);

    nrf_radio_int_enable(NRF_RADIO_INT_CRCERROR_MASK);


    

    nrf_radio_task_trigger(NRF_RADIO_TASK_RXEN);



    //bsp_board_led_invert(BSP_BOARD_LED_3);


    /* TODO busy loop on receive and init*/

}


void RADIO_IRQHandler(void)
{


    bsp_board_led_invert(BSP_BOARD_LED_3);

    //what if not received at all. timeout?

    if (nrf_radio_event_check(NRF_RADIO_EVENT_CRCOK))
    {
        nrf_radio_event_clear(NRF_RADIO_EVENT_CRCOK);
       

        bsp_board_led_invert(BSP_BOARD_LED_2);

    }

    if (nrf_radio_event_check(NRF_RADIO_EVENT_CRCERROR))
    {
        nrf_radio_event_clear(NRF_RADIO_EVENT_CRCERROR);
        

        bsp_board_led_invert(BSP_BOARD_LED_1);

    }



}


void radio_init(radio_config_t * p_config){

    bsp_board_led_invert(BSP_BOARD_LED_1);

    if (!m_p_config)
    {
        NVIC_EnableIRQ(RADIO_IRQn);
        __enable_irq();

        bsp_board_led_invert(BSP_BOARD_LED_3);

        m_p_config = p_config;
    }
    else
    {
        // Already initialized, do nothing
    }

}

