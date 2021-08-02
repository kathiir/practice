#include "spis.h"

#include "crc16.h"


#include "nrfx_spis.h"

#include "boards.h"

#include "nrf.h"
#include "nrf_log.h"


#include "nrf_delay.h"

static spi_payload_t spi_payload;


#define SPIS_INSTANCE 1 
static const nrfx_spis_t spis = NRFX_SPIS_INSTANCE(SPIS_INSTANCE);


#define TX_SIZE 255
#define RX_SIZE 3 + sizeof(spi_payload) + 2

static uint8_t       m_tx_buf[TX_SIZE];
static uint8_t       m_rx_buf[RX_SIZE];
static const uint8_t m_tx_length = sizeof(m_tx_buf);  
static const uint8_t m_rx_length = sizeof(m_rx_buf);


static volatile bool spis_xfer_done;

static volatile bool spis_config_received;

static volatile bool spis_ack_received;


static spi_payload_t spis_get_rx_payload() {
    spi_payload_t payload;

    memcpy(&payload, m_rx_buf + 3, sizeof(payload));

    return payload;
}


/**
 * @brief SPIS user event handler.
 */
static void spis_event_handler(const nrfx_spis_evt_t * event,
                        void * p_context)
{
    /*spis_config_received = false;
    spis_ack_received = false;
    spis_xfer_done = false;
*/
    if (event->evt_type == NRFX_SPIS_XFER_DONE)
    {
        spis_xfer_done = true;

        memcpy(&spi_payload, m_rx_buf + 3, sizeof(spi_payload));


        if (spi_payload.type == CONFIG_MODE || spi_payload.type == CONFIG_CHANNEL) {   
            spis_config_received = true;

        }

        if (spi_payload.type == ACKNOWLEDGE) {   
            spis_ack_received = true;

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


uint16_t spis_get_rx_number() {
    uint16_t num;

    memcpy(&num, m_rx_buf, sizeof(num));

    return num;
}

static uint8_t spis_get_rx_length() {
    uint8_t length;

    memcpy(&length, m_rx_buf + 2, sizeof(length));

    return length;
}




bool spis_check_config_received() {
    return spis_config_received;
}

spi_payload_t spis_get_payload() {
    return spi_payload;
}


bool spis_receive_ack(uint16_t count) {


    for (int i = 0; i < 5; i++) {

        spis_ack_received = false;
        spis_xfer_done = false;

        memset(m_rx_buf, 0, m_rx_length);

        APP_ERROR_CHECK(nrfx_spis_buffers_set(&spis, m_tx_buf, m_tx_length, m_rx_buf, m_rx_length));

        uint8_t timeout = 100;
        while (!spis_xfer_done && timeout--)
        {
            __WFE();
            nrf_delay_ms(1);
        }

        if (spis_ack_received) {
            if (spis_get_rx_number() != count) {
                continue;
            }

            return true;
        }
    }

    return false;
}



//TODO count
void spis_set_tx_message(uint8_t * p_packet, uint8_t length) {

    uint16_t num = 0;
    memcpy(&num, p_packet + 1, sizeof(num));

    memset(m_rx_buf, 0, m_rx_length);
    memset(m_tx_buf, 0, m_tx_length);

    memcpy(m_tx_buf, &num, sizeof(num));
    memcpy(m_tx_buf + sizeof(num), &length, 1);

    spi_message_t message = PACKET;

    memcpy(m_tx_buf + sizeof(num) + 1, &message, 1);
    
    memcpy(m_tx_buf + sizeof(num) + 1 + 1, p_packet, length);



    uint16_t crc = crc16_compute(m_tx_buf, sizeof(num) + 1 + 1 + length, NULL); //TODO length
    memcpy(m_tx_buf + sizeof(num) + 1 + 1 + length, &crc, sizeof(crc));
    
    spis_xfer_done = false;
    
    APP_ERROR_CHECK(nrfx_spis_buffers_set(&spis, m_tx_buf, m_tx_length, m_rx_buf, m_rx_length));

    uint8_t timeout = 100;

    while (!spis_xfer_done && timeout--) //TODO timeout
    {
        __WFE();
        nrf_delay_ms(1);
    }

    spis_receive_ack(num);

    memset(m_rx_buf, 0, m_rx_length);
    memset(m_tx_buf, 0, m_tx_length);
/*
    spis_config_received = false;
    spis_ack_received = false;
    spis_xfer_done = false;

    APP_ERROR_CHECK(nrfx_spis_buffers_set(&spis, m_tx_buf, m_tx_length, m_rx_buf, m_rx_length));
*/
}


void spis_send_ack(uint16_t count) {

    memset(m_tx_buf, 0, m_tx_length);
    memset(m_rx_buf, 0, m_rx_length);

    memcpy(m_tx_buf, &count, sizeof(count));


    spi_message_t message = ACKNOWLEDGE;

    uint8_t length = 2;

    memcpy(m_tx_buf + sizeof(count), &length, 1);

    memcpy(m_tx_buf + sizeof(count) + 1, &message, 1);    
    
    
    uint16_t crc = crc16_compute(m_tx_buf, sizeof(count) + 1 + length, NULL); //TODO length
    memcpy(m_tx_buf + sizeof(count) + 1 + length, &crc, sizeof(crc));

    spis_xfer_done = false;

    APP_ERROR_CHECK(nrfx_spis_buffers_set(&spis, m_tx_buf, m_tx_length, m_rx_buf, m_rx_length));

    uint8_t timeout = 255;
    while (!spis_xfer_done && timeout--) //TODO timeout
    {
        __WFE();
        nrf_delay_ms(1);
    }

    //nrf_delay_ms(100);
}


void spis_receive_config() {
    spis_config_received = false;
    spis_ack_received = false;
    spis_xfer_done = false;

    memset(m_rx_buf, 0, m_rx_length);
    memset(m_tx_buf, 0, m_tx_length);

    
    uint32_t err_code;

    err_code = nrfx_spis_buffers_set(&spis, m_tx_buf, m_tx_length, m_rx_buf, m_rx_length);
    APP_ERROR_CHECK(err_code);
  
}

