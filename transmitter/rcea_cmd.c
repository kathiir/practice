#include <stdlib.h>
#include "nrf_cli.h"

#include "nrf_cli_libuarte.h"


/** Indicates devices that support 802.15.4 radio mode. */
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)// || defined(NRF52832_XXAA)
    #define SUPPORT_IEEE802154_250KBIT 1
#else
    #define SUPPORT_IEEE802154_250KBIT 0
#endif



void rcea_cmd_init(void)
{
}

static void cmd_hello(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_OPTION,
                    "\n"
                    "   ------¬  ------¬-------¬ -----¬ \n"
                    "   --ã==--¬--ã====---ã====---ã==--¬\n"
                    "   ------ã---¦     -----¬  -------¦\n"
                    "   --ã==--¬--¦     --ã==-  --ã==--¦\n"
                    "   --¦  --¦L------¬-------¬--¦  --¦\n"
                    "   L=-  L=- L=====-L======-L=-  L=-\n"
                    "\n");

    nrf_cli_fprintf(p_cli,NRF_CLI_NORMAL,
                    "Radio channel error analyzer \n\n");
}


NRF_CLI_CMD_REGISTER(hello, NULL, "Print Hello", cmd_hello);
