#include "spis.h"

#include "nrfx_spis.h"

#include "boards.h"

#include "nrf.h"
#include "nrf_log.h"

#include "nrf_delay.h"

static spi_payload_t spi_payload;


#define SPIS_INSTANCE 1 /**< SPIS instance index. */
static const nrfx_spis_t spis = NRFX_SPIS_INSTANCE(SPIS_INSTANCE); /**< SPIS instance. */


static uint8_t       m_tx_buf[RADIO_MAX_PAYLOAD_LEN-1];           /**< TX buffer. */
static uint8_t       m_rx_buf[sizeof(spi_payload)];    /**< RX buffer. */
static const size_t m_tx_length = sizeof(m_tx_buf);  
static const size_t m_rx_length = sizeof(m_rx_buf);
  

static volatile bool spis_xfer_done;

static volatile bool spis_config_received = false;


/**
 * @brief SPIS user event handler.
 * @param event
 */
static void spis_event_handler(const nrfx_spis_evt_t * event,
                        void * p_context)
{
    spis_config_received = false;

    if (event->evt_type == NRFX_SPIS_XFER_DONE)
    {
        
        NRF_LOG_INFO(" Transfer completed. Received: %s",(uint32_t)m_rx_buf);

        //if sent config
        memcpy(&spi_payload, m_rx_buf, m_rx_length);


        if (spi_payload.type == CONFIG_MODE || spi_payload.type == CONFIG_CHANNEL) {
            spis_xfer_done = true;
            spis_config_received = true;

            bsp_board_led_invert(BSP_BOARD_LED_0);

        }

    }
}


/**@brief Function for configuring and initializing SPIS.
 */
void spis_init()
{
    ret_code_t ret;

    nrfx_spis_config_t spis_config = NRFX_SPIS_DEFAULT_CONFIG;
    spis_config.csn_pin               = APP_SPIS_CS_PIN;
    spis_config.miso_pin              = APP_SPIS_MISO_PIN;
    spis_config.mosi_pin              = APP_SPIS_MOSI_PIN;
    spis_config.sck_pin               = APP_SPIS_SCK_PIN;

    ret = nrfx_spis_init(&spis, &spis_config, spis_event_handler, NULL);
    APP_ERROR_CHECK(ret);
}


bool spis_check_config_received() {
    return spis_config_received;
}

spi_payload_t spis_get_payload() {
    return spi_payload;
}

void spis_set_tx_message(uint8_t * p_packet, size_t length) {
    memset(m_rx_buf, 0, m_rx_length);

    memcpy(m_tx_buf, p_packet, length);

    APP_ERROR_CHECK(nrfx_spis_buffers_set(&spis, m_tx_buf, m_tx_length, m_rx_buf, m_rx_length));

    //need to wait for transfer?

}


void spis_receive_config() {
    spis_config_received = false;

    memset(m_rx_buf, 0, m_rx_length);
    //memset(m_tx_buf, 0, m_tx_length);

/*
    NRF_LOG_INFO(" Transfer completed. Received: %s",(uint32_t)m_rx_buf);
  */  


    
    uint32_t err_code;

    err_code = nrfx_spis_buffers_set(&spis, m_tx_buf, m_tx_length, m_rx_buf, m_rx_length);

    NRF_LOG_INFO(" Error code: %d", (uint32_t) err_code);

    for (int i = 0; i < err_code; i++) {

        bsp_board_led_invert(BSP_BOARD_LED_2);
        //nrf_delay_ms(500);
      //  bsp_board_led_invert(BSP_BOARD_LED_2);
    }

    nrf_delay_ms(500);

    APP_ERROR_CHECK(err_code);

}

