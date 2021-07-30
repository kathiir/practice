#ifndef SPIM_H
#define SPIM_H

#include "nrfx_spim.h"

#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif


#define RCEA_SPIM_EMPTY             0
#define RCEA_SPIM_ACKNOWLEDGE       1
#define RCEA_SPIM_CONFIG_CHANNEL    2
#define RCEA_SPIM_CONFIG_MODE       3
#define RCEA_SPIM_WAITING_FOR_ACK   4


typedef enum {
    EMPTY = RCEA_SPIM_EMPTY,
    ACKNOWLEDGE = RCEA_SPIM_ACKNOWLEDGE,
    CONFIG_MODE = RCEA_SPIM_CONFIG_CHANNEL,
    CONFIG_CHANNEL = RCEA_SPIM_CONFIG_MODE,
    WAIT_ACK = RCEA_SPIM_WAITING_FOR_ACK
} spi_message_t;


typedef struct {
    spi_message_t type;
    union {
        nrf_radio_mode_t mode;
        uint8_t channel;
        bool success;
    } params;
} spi_payload_t;


/**@brief Function for initializing SPIM module.
 */
void spim_init(void);


//TODO
/**@brief Function for transferring payload via SPI.
 */
void spim_transfer(spi_payload_t payload);


/**@brief Function for transferring channel via SPI.
 */
bool spim_transfer_channel(uint8_t channel, uint16_t count);


/**@brief Function for transferring mode via SPI.
 */
bool spim_transfer_mode(nrf_radio_mode_t mode, uint16_t count);


/**@brief Function for transferring radio packet from receiver via SPI.
 */
bool spim_receive_packet(uint8_t * p_packet, size_t length);



#ifdef __cplusplus
}
#endif

#endif