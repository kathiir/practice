#include "rcea_receiver.h"


#include "boards.h"

#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "spis.h"

#include "nrf_delay.h"


static uint8_t m_rx_packet[RADIO_MAX_PAYLOAD_LEN];

static radio_config_t m_config;



void rcea_receiver_init() {
    
    spis_init();

    radio_init(&m_config, m_rx_packet);


}


void rcea_process() {

    radio_rx(m_config.mode, m_config.channel);

    receive_packet();
    spis_receive_config();

    while(true) {

        while(!spis_check_config_received() && !radio_check_packet_received()) {
           // __WFE();
           /* wait */
        }

        if (radio_check_packet_received()) {
            bsp_board_led_invert(BSP_BOARD_LED_1);

            spis_set_tx_message(m_rx_packet, RADIO_MAX_PAYLOAD_LEN);
                
            spis_receive_config();
            receive_packet();
        }

        

        if (spis_check_config_received()) {
            bsp_board_led_invert(BSP_BOARD_LED_2);

            spi_payload_t payload = spis_get_payload();

            if (payload.type == CONFIG_MODE) {
                m_config.mode = payload.params.mode;
            } else if (payload.type == CONFIG_CHANNEL) {
                m_config.channel = payload.params.channel;
            }

            uint16_t count = spis_get_rx_number();

            spis_send_ack(count);

            radio_rx(m_config.mode, m_config.channel);

            spis_receive_config();
            receive_packet();
        }

    }
}
