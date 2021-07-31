#include "spis.h"

#include "nrfx_spis.h"

#include "boards.h"

#include "nrf.h"
#include "nrf_log.h"

#include "nrf_delay.h"

static spi_payload_t spi_payload;


#define SPIS_INSTANCE 1 
static const nrfx_spis_t spis = NRFX_SPIS_INSTANCE(SPIS_INSTANCE);


#define RX_SIZE 3 + sizeof(spi_payload) + 2

static uint8_t       m_tx_buf[RX_SIZE];
static uint8_t       m_rx_buf[RX_SIZE];
static const size_t m_tx_length = sizeof(m_tx_buf);  
static const size_t m_rx_length = sizeof(m_rx_buf);


static volatile bool spis_xfer_done;

static volatile bool spis_config_received = false;

static volatile bool ack_received;


/**
 * @brief SPIS user event handler.
 */
static void spis_event_handler(const nrfx_spis_evt_t * event,
                        void * p_context)
{
    spis_config_received = false;

    if (event->evt_type == NRFX_SPIS_XFER_DONE)
    {
        
        NRF_LOG_INFO(" Transfer completed. Received: %s",(uint32_t)m_rx_buf);

        //if sent config
//        memcpy(&spi_payload, m_rx_buf, m_rx_length);

        spi_payload = spis_get_payload();


        if (spi_payload.type == CONFIG_MODE || spi_payload.type == CONFIG_CHANNEL) {
            spis_xfer_done = true;
            spis_config_received = true;

            bsp_board_led_invert(BSP_BOARD_LED_0);

            NRF_LOG_INFO("Transfer completed. Config Received: %s",(uint32_t)m_rx_buf);

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


static uint16_t spis_get_rx_number() {
    uint8_t num;

    memcpy(&num, m_rx_buf, sizeof(num));

    return num;
}

static uint8_t spis_get_rx_length() {
    uint8_t length;

    memcpy(&length, m_rx_buf + 2, sizeof(length));

    return length;
}

static spi_payload_t spis_get_rx_payload() {
    spi_payload_t payload;

    memcpy(&payload, m_rx_buf + 3, sizeof(payload));

    return payload;
}

bool spis_check_config_received() {
    return spis_config_received;
}

spi_payload_t spis_get_payload() {
    return spi_payload;
}


//TODO
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
    APP_ERROR_CHECK(err_code);

    //TODO wait for ack


    

}

