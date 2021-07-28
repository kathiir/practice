#include "rcea.h"

#include "boards.h"



#include <stdbool.h>
#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "nrf_delay.h"


#include "spim.h"


static radio_config_t m_config;




void rcea_send_packet() {

    send_packet(NRF_RADIO_MODE_NRF_250KBIT, NRF_RADIO_TXPOWER_0DBM, 11);

    //send_packet();


    //wait spi for analyzing - timeout

}


void rcea_set_mode(nrf_radio_mode_t mode) {



    radio_config_mode(mode);
/*
    spi_payload.type = CONFIG_MODE;
    spi_payload.params.mode = mode;
*/
    // send via spi

}


void rcea_set_tx_power(nrf_radio_txpower_t txpower) {
    radio_config_tx_power(txpower);

    


}


//TODO check channel
void rcea_set_channel(uint8_t channel) {
    //radio_config_channel(channel);


    spim_transfer_channel(11);

}

/*
void set_config(nrf_radio_mode_t mode,
                nrf_radio_txpower_t txpower,
                uint8_t channel) {

}
*/



void rcea_init() {

    spim_init();
    radio_init(&m_config);


    //send config

    /*
    while (1)
    {
        // Reset rx buffer and transfer done flag
        memset(m_rx_buf, 0, m_rx_length);
        spi_xfer_done = false;

        APP_ERROR_CHECK(nrfx_spim_xfer(&spim, &xfer_data, 0));

                
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


