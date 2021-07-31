#include "spim.h"

#include "boards.h"

#include "nrfx_spim.h"

#include "nrf.h"
#include "nrf_log.h"

#include "crc16.h"

#define SPIM_INSTANCE 0
static const nrfx_spim_t spim = NRFX_SPIM_INSTANCE(SPIM_INSTANCE);

static spi_payload_t spi_payload;


//header + payload + crc
//3 + size + crc16?

#define TX_SIZE 3 + sizeof(spi_payload) + 2
#define RX_SIZE 255

static uint8_t       m_tx_buf[TX_SIZE];
static uint8_t       m_rx_buf[RX_SIZE];   
static const size_t m_tx_length = sizeof(m_tx_buf);  
static const size_t m_rx_length = sizeof(m_rx_buf);  

static nrfx_spim_xfer_desc_t xfer_data;

static volatile bool spi_xfer_done;

static volatile bool message_received;

static volatile bool ack_received;



/**@brief Function for handling SPI events
 */
static void spim_event_handler(const nrfx_spim_evt_t * event,
                       void * p_context)
{

    spi_xfer_done = true;
    NRF_LOG_INFO("Transfer completed.");
    message_received = false;
    if (m_rx_buf[0] != 0)
    {
        message_received = true;

        NRF_LOG_RAW_INFO(" Received:");
        NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
        bsp_board_led_invert(BSP_BOARD_LED_1);
    }



        //TODO here timeout on receiving answer
        //TODO analyze packet
        //TODO set ready to send flag
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

}


static void spim_set_tx_message(uint16_t num, spi_payload_t * payload) {
    
    
    memcpy(m_tx_buf, &num, sizeof(num));
    uint8_t size = sizeof(payload);
    memcpy(m_tx_buf + sizeof(num), &size, 1);
    memcpy(m_tx_buf + sizeof(num) + 1, payload, sizeof(payload));

    uint16_t crc = crc16_compute(m_tx_buf, TX_SIZE-2, NULL);
    memcpy(m_tx_buf + sizeof(num) + 1 + sizeof(payload), &crc, sizeof(crc));

}

static uint16_t spim_get_rx_number() {
    uint8_t num;

    memcpy(&num, m_rx_buf, sizeof(num));

    return num;
}

static uint8_t spim_get_rx_length() {
    uint8_t length;

    memcpy(&length, m_rx_buf + 2, sizeof(length));

    return length;
}


static spi_message_t spim_get_rx_payload_type() {
    spi_message_t payload_type;

    memcpy(&payload_type, m_rx_buf + 3, sizeof(payload_type));

    return payload_type;
}


static bool spim_receive_ack(uint16_t num) {

    ack_received = false;
    memset(m_rx_buf, 0, m_rx_length);

    spi_payload.type = WAIT_ACK;

    spim_set_tx_message(num, &spi_payload);


    //while (!ack_received) {

        for (int i = 0; i < 10 && !ack_received; i++) {
    
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

            //TODO delay

        }

        
    //}

    return false;
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

    //TODO delay?

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

    return spim_receive_ack(count);

}


//TODO
/**@brief Function for transferring radio packet from receiver via SPI.
 */
bool spim_receive_packet(uint8_t * p_packet, size_t length, uint16_t count) {
    

    message_received = false;

    memset(m_rx_buf, 0, m_rx_length);


    


    APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));

    //TODO timeout
    while(!message_received) {
        __WFE();
    }

    memcpy(p_packet, xfer_data.p_rx_buffer + 3, length);

}



