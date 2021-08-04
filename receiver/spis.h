#ifndef SPIS_H
#define SPIS_H

#include "nrfx_spis.h"

#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif


#define RCEA_SPI_EMPTY             0
#define RCEA_SPI_ACKNOWLEDGE       1
#define RCEA_SPI_CONFIG_CHANNEL    2
#define RCEA_SPI_CONFIG_MODE       3
#define RCEA_SPI_BOOT              4
#define RCEA_SPI_PACKET            5


typedef enum {
    EMPTY = RCEA_SPI_EMPTY,
    ACKNOWLEDGE = RCEA_SPI_ACKNOWLEDGE,
    CONFIG_MODE = RCEA_SPI_CONFIG_CHANNEL,
    CONFIG_CHANNEL = RCEA_SPI_CONFIG_MODE,
    BOOT = RCEA_SPI_BOOT,
    PACKET = RCEA_SPI_PACKET
} spi_message_t;


typedef struct {
    spi_message_t type;
    union {
        nrf_radio_mode_t mode;
        uint8_t channel;
        bool success;
    } params;
} spi_payload_t;

/**@brief Function for configuring and initializing SPIS.
 */
void spis_init(void);

/**@brief Function for checking if config was received.
 */
bool spis_check_config_received(void);

/**@brief Function for getting payload number.
 */
uint16_t spis_get_rx_number(void);

/**@brief Function for getting payload.
 */
spi_payload_t spis_get_payload(void);

/**@brief Function for setting payload.
 */
void spis_set_tx_message(uint8_t * p_packet, uint8_t length);

/**@brief Function for sending acknoledge.
 */
void spis_send_ack(uint16_t count);

/**@brief Function for receiving acknoledge.
 */
bool spis_receive_ack(uint16_t count);

/**@brief Function for receiving config.
 */
void spis_receive_config(void);


#ifdef __cplusplus
}
#endif

#endif