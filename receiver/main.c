#include "sdk_config.h"

#include "nrf_drv_spis.h"
//#include "app_util_platform.h"
#include "nrf_gpio.h"
//#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"

#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_drv_clock.h"
#include "app_timer.h"

#include "radio.h"

/*
#define SPI_INSTANCE  0 /**< SPI instance index. */
/*static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
/*static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */


//#define SPIS_INSTANCE 1 /**< SPIS instance index. */
//static const nrf_drv_spis_t spis = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE); /**< SPIS instance. */

//static volatile bool spis_xfer_done;

#define TEST_STRING "Test"
static uint8_t       m_tx_buf[] = TEST_STRING;           /**< TX buffer. */
static uint8_t       m_rx_buf[sizeof(TEST_STRING) + 1];    /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */



static radio_config_t m_config;


/**
 * @brief SPI user event handler.
 * @param event
 *//*
void spis_event_handler(nrf_drv_spis_event_t event)
{
    if (event.evt_type == NRF_DRV_SPIS_XFER_DONE)
    {
        spis_xfer_done = true;
        NRF_LOG_INFO(" Transfer completed. Received: %s",(uint32_t)m_rx_buf);
        bsp_board_led_invert(BSP_BOARD_LED_1);
        //TODO analyze packet
        //TODO set ready to send flag
    }
}


/**@brief Function for configuring SPI.
 *//*
static void spis_init(void)
{
    ret_code_t ret;

    nrf_drv_spis_config_t spis_config = NRF_DRV_SPIS_DEFAULT_CONFIG;
    spis_config.csn_pin               = APP_SPIS_CS_PIN;
    spis_config.miso_pin              = APP_SPIS_MISO_PIN;
    spis_config.mosi_pin              = APP_SPIS_MOSI_PIN;
    spis_config.sck_pin               = APP_SPIS_SCK_PIN;

    ret = nrf_drv_spis_init(&spis, &spis_config, spis_event_handler);
    APP_ERROR_CHECK(ret);
}


/**@brief Function for initializing logging.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(app_timer_cnt_get);

    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT(); //??
}


// TODO is nrf drv legacy? or maybe init as NRF_CLOCK what is difference
/**@brief Function for initializing oscillators.
 */
static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    
    // low frequency clock - app timer - used by cli and etc
    nrf_drv_clock_lfclk_request(NULL);

    // high frequency clock - radio
    nrf_drv_clock_hfclk_request(NULL); 

}


int main(void)
{
    uint32_t err_code;

    bsp_board_init(BSP_INIT_LEDS);

    clock_init();

    err_code = app_timer_init();

    APP_ERROR_CHECK(err_code);

    //log_init();

    bsp_board_led_invert(BSP_BOARD_LED_0);


    radio_init(&m_config);

    bsp_board_led_invert(BSP_BOARD_LED_1);

    radio_rx(NRF_RADIO_MODE_NRF_250KBIT, 11);

    bsp_board_led_invert(BSP_BOARD_LED_2);


    while (true)
    {
        __WFI();
        //bsp_board_led_invert(BSP_BOARD_LED_0);
    }

    //spi_init();

    //TODO radio receiver
        
    //TODO standby for titan... radio
    //TODO then send packet via spi


/*
    while (1)
    {
        // Reset rx buffer and transfer done flag
        memset(m_rx_buf, 0, m_length);
        spi_xfer_done = false;

        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, m_tx_buf, m_length, m_rx_buf, m_length));

        while (!spi_xfer_done)
        {
            __WFE();
        }

        NRF_LOG_FLUSH();

        bsp_board_led_invert(BSP_BOARD_LED_0);
        nrf_delay_ms(200);
    }

    */
}
