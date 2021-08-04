#ifndef SPIM_H
#define SPIM_H

#include "nrfx_spim.h"

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


/**@brief Function for initializing SPIM module.
 */
void spim_init(void);


/**@brief Function for transferring channel via SPI.
 */
bool spim_transfer_channel(uint8_t channel, uint16_t count);


/**@brief Function for transferring mode via SPI.
 */
bool spim_transfer_mode(nrf_radio_mode_t mode, uint16_t count);


/**@brief Function for transferring radio packet from receiver via SPI.
 */
bool spim_receive_packet(uint8_t * p_packet, size_t length, uint16_t count);





#ifdef __cplusplus
}
#endif

#endif