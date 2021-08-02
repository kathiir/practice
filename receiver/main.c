#include "sdk_config.h"


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

#include "rcea_receiver.h"



/**@brief Function for initializing logging.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(app_timer_cnt_get);

    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT(); //??
}


/**@brief Function for initializing oscillators.
 */
static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    
    // low frequency clock - app timer - used by cli and etc
    nrf_drv_clock_lfclk_request(NULL);

    while (!nrf_drv_clock_lfclk_is_running())
    {
        /* wait */
    }

    // high frequency clock - radio
    nrf_drv_clock_hfclk_request(NULL); 

    while (!nrf_drv_clock_hfclk_is_running())
    {
        /* wait */
    }
}


int main(void)
{
    uint32_t err_code;

    bsp_board_init(BSP_INIT_LEDS);

    clock_init();

    bsp_board_led_invert(BSP_BOARD_LED_0);

    err_code = app_timer_init();

    APP_ERROR_CHECK(err_code);
    
    bsp_board_led_invert(BSP_BOARD_LED_1);

    log_init();

    bsp_board_led_invert(BSP_BOARD_LED_2);

    rcea_receiver_init();
    
    bsp_board_led_invert(BSP_BOARD_LED_3);

    rcea_process();

}
