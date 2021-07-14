#ifndef RADIO_H
#define RADIO_H

#include "nrf_radio.h"


#ifdef __cplusplus
extern "C" {
#endif

#define IEEE_MAX_PAYLOAD_LEN      127   /**< IEEE 802.15.4 maximum payload length. */
#define IEEE_MIN_CHANNEL          11    /**< IEEE 802.15.4 minimum channel. */
#define IEEE_MAX_CHANNEL          26    /**< IEEE 802.15.4 maximum channel. */

#ifdef __cplusplus
}
#endif

#endif