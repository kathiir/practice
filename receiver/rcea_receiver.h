#ifndef RCEA_RECEIVER_H
#define RCEA_RECEIVER_H

#include "radio.h"


#ifdef __cplusplus
extern "C" {
#endif

/**@brief Function for initializing rcea module.
 */
void rcea_receiver_init();

/**@brief Main rcea process.
 */
void rcea_process();

#ifdef __cplusplus
}
#endif

#endif