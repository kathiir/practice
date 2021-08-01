#include "rcea_receiver.h"


#include "boards.h"

#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "spis.h"


static uint8_t m_rx_packet[RADIO_MAX_PAYLOAD_LEN];

static radio_config_t m_config;



void rcea_receiver_init() {
    
    spis_init();

    radio_init(&m_config, m_rx_packet);


}


void rcea_process() {

    radio_rx(m_config.mode, m_config.channel);

    while(true) {
        

        spis_receive_config();

        bsp_board_led_invert(BSP_BOARD_LED_1);


        while(!spis_check_config_received()) {
            receive_packet();

            __WFE();

            if (radio_check_packet_received()) {
                        bsp_board_led_invert(BSP_BOARD_LED_0);

                spis_set_tx_message(m_rx_packet, RADIO_MAX_PAYLOAD_LEN);
            }
        }
        
        spi_payload_t payload = spis_get_payload();

        if (payload.type == CONFIG_MODE) {
            m_config.mode = payload.params.mode;
        } else if (payload.type == CONFIG_CHANNEL) {
            m_config.channel = payload.params.channel;
        }

        uint16_t count = spis_get_rx_number();
        bsp_board_led_invert(BSP_BOARD_LED_1);
        spis_send_ack(count);
        

        radio_rx(m_config.mode, m_config.channel);

        


        

    }
}
