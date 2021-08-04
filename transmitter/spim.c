#include "spim.h"

#include "boards.h"

#include "nrfx_spim.h"

#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "crc16.h"

#include "nrf_delay.h"

#define SPIM_INSTANCE 0
static const nrfx_spim_t spim = NRFX_SPIM_INSTANCE(SPIM_INSTANCE);

static spi_payload_t spi_payload;

#define TX_SIZE 3 + sizeof(spi_payload) + 2
#define RX_SIZE 255

static uint8_t       m_tx_buf[TX_SIZE];
static uint8_t       m_rx_buf[RX_SIZE];   
static const size_t m_tx_length = sizeof(m_tx_buf);  
static const size_t m_rx_length = sizeof(m_rx_buf);  

static nrfx_spim_xfer_desc_t xfer_data;

static volatile bool spi_xfer_done;

static volatile bool message_received;



/**@brief Function for handling SPI events
 */
static void spim_event_handler(const nrfx_spim_evt_t * event,
                       void * p_context)
{

    spi_xfer_done = true;
    message_received = false;


    if (m_rx_buf[0] != 0 || m_rx_buf[0] != 255)
    {
        message_received = true;

        NRF_LOG_RAW_INFO(" Received: \r\n");
        NRF_LOG_HEXDUMP_INFO(m_rx_buf, RX_SIZE);

    }
}

/**@brief Function for setting payload.
 */
static void spim_set_tx_message(uint16_t num, spi_payload_t * payload) {
        
    memcpy(m_tx_buf, &num, sizeof(num));
    uint8_t size = sizeof(spi_payload);
    memcpy(m_tx_buf + sizeof(num), &size, 1);
    memcpy(m_tx_buf + sizeof(num) + 1, payload, sizeof(spi_payload));

    uint16_t crc = crc16_compute(m_tx_buf, TX_SIZE-2, NULL);
    memcpy(m_tx_buf + sizeof(num) + 1 + sizeof(spi_payload), &crc, sizeof(crc));

}

/**@brief Function for sending boot message.
 */
void spim_boot(void) {
    spi_payload.type = BOOT;

    spim_set_tx_message(0, &spi_payload);

    spi_xfer_done = false;

    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));
        
    while (!spi_xfer_done)
    {
        __WFE();
    }
}


/**@brief Function for initializing SPIM module.
 */
void spim_init(void)
{
    ret_code_t ret;

    nrfx_spim_config_t spim_config = NRFX_SPIM_DEFAULT_CONFIG;
    spim_config.ss_pin               = SPI_SS_PIN;
    spim_config.miso_pin              = SPI_MISO_PIN;
    spim_config.mosi_pin              = SPI_MOSI_PIN;
    spim_config.sck_pin               = SPI_SCK_PIN;

    ret = nrfx_spim_init(&spim, &spim_config, spim_event_handler, NULL);
    APP_ERROR_CHECK(ret);

    xfer_data.p_tx_buffer = m_tx_buf;
    xfer_data.p_rx_buffer = m_rx_buf;
    xfer_data.tx_length = m_tx_length;
    xfer_data.rx_length = m_rx_length;

    spim_boot();

}


/**@brief Function for getting payload number.
 */
static uint16_t spim_get_rx_number() {
    uint16_t num;

    memcpy(&num, m_rx_buf, sizeof(num));

    return num;
}

/**@brief Function for getting payload length.
 */
static uint8_t spim_get_rx_length() {
    uint8_t length;

    memcpy(&length, m_rx_buf + 2, sizeof(length));

    return length;
}


/**@brief Function for getting payload type.
 */
static spi_message_t spim_get_rx_payload_type() {
    spi_message_t payload_type;

    memcpy(&payload_type, m_rx_buf + 3, sizeof(payload_type));

    return payload_type;
}


/**@brief Function for receiving acknowledge.
 */
static bool spim_receive_ack(uint16_t num) {

    memset(m_rx_buf, 0, m_rx_length);

    nrf_delay_ms(100);


    for (int i = 0; i < 20; i++) {

        message_received = false;
        spi_xfer_done = false;

        APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));
        
        while (!spi_xfer_done)
        {
            __WFE();
        }

        if (message_received) {
            
            if (spim_get_rx_number() != num) {
                
                continue;
            }

            spi_message_t payload_type = spim_get_rx_payload_type();
            
            if (payload_type == ACKNOWLEDGE) {
                return true;
            }
        }

        nrf_delay_ms(100);

    }

    return false;
}


/**@brief Function for sending acknowledge.
 */
static void spim_send_ack(uint16_t num) {
    
    spi_payload.type = ACKNOWLEDGE;

    spim_set_tx_message(num, &spi_payload);

    spi_xfer_done = false;

    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));
        
    while (!spi_xfer_done)
    {
        __WFE();
    }

}




/**@brief Function for transferring channel via SPI.
 */
bool spim_transfer_channel(uint8_t channel, uint16_t count) {

    spi_payload.type = CONFIG_CHANNEL;
    spi_payload.params.channel = channel;


    spim_set_tx_message(count, &spi_payload);

    spi_xfer_done = false;
    message_received = false;
    memset(m_rx_buf, 0, m_rx_length);

    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));

    while (!spi_xfer_done)
    {
        __WFE();
    }

    nrf_delay_ms(100);

    return spim_receive_ack(count);
}


/**@brief Function for transferring mode via SPI.
 */
bool spim_transfer_mode(nrf_radio_mode_t mode, uint16_t count) {

    spi_payload.type = CONFIG_MODE;
    spi_payload.params.mode = mode;


    spim_set_tx_message(count, &spi_payload);

    spi_xfer_done = false;
    message_received = false;
    memset(m_rx_buf, 0, m_rx_length);

    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));

                
    while (!spi_xfer_done)
    {
        __WFE();
    }

    nrf_delay_ms(100);

    return spim_receive_ack(count);
}


/**@brief Function for transferring radio packet from receiver via SPI.
 */
bool spim_receive_packet(uint8_t * p_packet, size_t length, uint16_t count) {
    
    message_received = false;

    memset(m_rx_buf, 0, m_rx_length);

    spi_payload.type = PACKET;

    spim_set_tx_message(count, &spi_payload);

    for (int i = 0; i < 20; i++) {

        message_received = false;
        spi_xfer_done = false;

        APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));
        
        while (!spi_xfer_done)
        {
            __WFE();
        }

        if (message_received) {
            
            if (spim_get_rx_number() != count) {
                NRF_LOG_INFO("Wrong number.");
                continue;
            }

            spi_message_t payload_type = spim_get_rx_payload_type();
            
            if (payload_type == PACKET) {

                memcpy(p_packet, xfer_data.p_rx_buffer + 4, length);                

                spim_send_ack(count);

                return true;
            }
        }

        message_received = false;
        spi_xfer_done = false;

        nrf_delay_ms(100);
    }

    return false;
}



