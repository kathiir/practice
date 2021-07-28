#include "rcea_receiver.h"



#include "boards.h"

#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "spis.h"




static radio_config_t m_config;



//config



void rcea_receiver_init() {
    
    spis_init();

    radio_init(&m_config);


}


void rcea_process() {

    radio_rx(NRF_RADIO_MODE_NRF_250KBIT, 11);

    while(true) {
        

        spis_receive_config();

        bsp_board_led_invert(BSP_BOARD_LED_1);


        while(!spis_check_config_received()) {
            __WFE();
        }
        
        //config radio aka set values for m_config and send to radio_rx
        //spis_get_payload();

        bsp_board_led_invert(BSP_BOARD_LED_1);
/*
        radio_rx(NRF_RADIO_MODE_NRF_250KBIT, 11);
*/
    }
}
