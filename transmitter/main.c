#include "sdk_config.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>



#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "rcea_cmd.h"

#include "nrf_cli.h"
#include "nrf_cli_libuarte.h"

#include "nrf_drv_clock.h"


/** CLI */
#define CLI_LOG_QUEUE_SIZE  (4)

NRF_CLI_LIBUARTE_DEF(m_cli_libuarte_transport, 256, 256);
NRF_CLI_DEF(m_cli_libuarte,
            "cli:~$ ",
            &m_cli_libuarte_transport.transport,
            '\r',
            CLI_LOG_QUEUE_SIZE);





/**@brief Function for starting a CLI
 */
static void cli_start(void)
{
    ret_code_t ret;

    ret = nrf_cli_start(&m_cli_libuarte);
    APP_ERROR_CHECK(ret);
}


// TODO libuarte or just uart
/**@brief Function for configuring libuarte for CLI.
 */
static void cli_init(void)
{
    ret_code_t ret;

    cli_libuarte_config_t libuarte_config;
    libuarte_config.tx_pin   = TX_PIN_NUMBER;
    libuarte_config.rx_pin   = RX_PIN_NUMBER;
    libuarte_config.baudrate = NRF_UARTE_BAUDRATE_115200;
    libuarte_config.parity   = NRF_UARTE_PARITY_EXCLUDED;
    libuarte_config.hwfc     = NRF_UARTE_HWFC_DISABLED;

    ret = nrf_cli_init(&m_cli_libuarte, &libuarte_config, true, true, NRF_LOG_SEVERITY_INFO);
    APP_ERROR_CHECK(ret);
}


static void cli_process(void)
{
    nrf_cli_process(&m_cli_libuarte);
}


/**@brief Function for initializing logging.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(app_timer_cnt_get);

    APP_ERROR_CHECK(err_code);

    //NRF_LOG_DEFAULT_BACKENDS_INIT(); //??
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


//TODO sdk_config - legacy support or maybe change some vars - read more
//TODO also pca10040 does not support ieee_802154_250 and ieee at all
//TODO idk what to do about that
//TODO make with support for different modes of radio and ieee only if supported? 
//TODO but what to do on receiver. how to check mode - read more
int main(void)
{
    uint32_t err_code;

    bsp_board_init(BSP_INIT_LEDS);

    clock_init();

    log_init();

    err_code = app_timer_init();
    
    APP_ERROR_CHECK(err_code);

    //spis_init();



    rcea_cmd_init();


    cli_init();
    cli_start();

    
    NRF_LOG_RAW_INFO("Radio channel error analyzer started.\n");
    NRF_LOG_RAW_INFO("Please press the Tab key to see all available commands.\n");

    while (true)
    {
        UNUSED_RETURN_VALUE(NRF_LOG_PROCESS());
        cli_process();
    }

}
