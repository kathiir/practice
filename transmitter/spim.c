#include "spim.h"

#include "boards.h"

#include "nrfx_spim.h"

#include "nrf.h"
#include "nrf_log.h"

#define SPIM_INSTANCE 0
static const nrfx_spim_t spim = NRFX_SPIM_INSTANCE(SPIM_INSTANCE);

static spi_payload_t spi_payload;

#define TEST_STRING "Test"
static uint8_t       m_tx_buf[sizeof(spi_payload)];          
static uint8_t       m_rx_buf[RADIO_MAX_PAYLOAD_LEN];   
static const size_t m_tx_length = sizeof(m_tx_buf);  
static const size_t m_rx_length = sizeof(m_rx_buf);  

static nrfx_spim_xfer_desc_t xfer_data;

static volatile bool spi_xfer_done;

static volatile bool packet_received;


/**@brief Function for handling SPI events
 */
static void spim_event_handler(const nrfx_spim_evt_t * event,
                       void * p_context)
{

    spi_xfer_done = true;
    NRF_LOG_INFO("Transfer completed.");
    packet_received = false;
    if (m_rx_buf[0] != 0)
    {
        packet_received = true;

        NRF_LOG_RAW_INFO(" Received:");
        NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
        bsp_board_led_invert(BSP_BOARD_LED_1);
    }

        //TODO here timeout on receiving answer
        //TODO analyze packet
        //TODO set ready to send flag
}

/**@brief Function for configuring SPI.
 */
void spim_init()
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

    spi_payload.end = 0;
}


void spim_transfer(spi_payload_t payload) {
    memset(m_rx_buf, 0, m_rx_length);

}



void spim_transfer_channel(uint8_t channel) {

    spi_payload.type = CONFIG_CHANNEL;
    spi_payload.params.channel = channel;

    memcpy(m_tx_buf, &spi_payload, m_tx_length);

    spi_xfer_done = false;
    packet_received = false;
    memset(m_rx_buf, 0, m_rx_length);

    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));

                
    while (!spi_xfer_done)
    {
        __WFE();
    }

}

void spim_transfer_mode(nrf_radio_mode_t mode) {

    spi_payload.type = CONFIG_MODE;
    spi_payload.params.mode = mode;

    memcpy(m_tx_buf, &spi_payload, m_tx_length);

    spi_xfer_done = false;
    packet_received = false;
    memset(m_rx_buf, 0, m_rx_length);

    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));

                
    while (!spi_xfer_done)
    {
        __WFE();
    }

    spi_xfer_done = false;

}


void spim_receive_packet(uint8_t * p_packet, size_t length) {
    
    uint8_t packet[RADIO_MAX_PAYLOAD_LEN];

    packet_received = false;

    memset(m_rx_buf, 0, m_rx_length);


    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));

    //TODO timeout
    while(!packet_received) {
        __WFE();
    }

}



