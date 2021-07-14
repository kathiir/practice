#include "radio.h"
#include "radio_config.h"
#include "nrf_radio.h"


#define IEEE_DEFAULT_FREQ         (5)   /**< IEEE 802.15.4 default frequency. */

#define IEEE_FREQ_CALC(_channel) (IEEE_DEFAULT_FREQ + \
                                  (IEEE_DEFAULT_FREQ * ((_channel) - IEEE_MIN_CHANNEL)))

//TODO congig
//TODO set channel, power etc

//TODO redo to ieee - read more
//TODO set with radio config or nrf_radio 
static void send_packet()
{

//TODO set pointer to packet

    // send the packet:
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN   = 1;

    while (NRF_RADIO->EVENTS_READY == 0U)
    {
        // wait
    }
    NRF_RADIO->EVENTS_END  = 0U;
    NRF_RADIO->TASKS_START = 1U;

    while (NRF_RADIO->EVENTS_END == 0U)
    {
        // wait
    }

    //uint32_t err_code = bsp_indication_set(BSP_INDICATE_SENT_OK);
    //NRF_LOG_INFO("The packet was sent");
    //APP_ERROR_CHECK(err_code);

    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }



    //TODO or

    /*

        nrf_radio_task_trigger(NRF_RADIO_TASK_TXEN);

            while (!nrf_radio_event_check(NRF_RADIO_EVENT_END))
    {
        /* Do nothing *//*
    }
*/
}