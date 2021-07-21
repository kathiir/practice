#include <stdlib.h>
#include "nrf_cli.h"

#include "nrf_cli_libuarte.h"


#include "radio.h"


static radio_config_t m_config;


void rcea_cmd_init(void)
{
    radio_init(&m_config);
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


static void cmd_send_packet(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    send_packet(NRF_RADIO_MODE_NRF_250KBIT, 0, 11);

    nrf_cli_fprintf(p_cli,NRF_CLI_NORMAL,
                    "Packet sent... I hope \n\n");
}


NRF_CLI_CMD_REGISTER(hello, NULL, "Print Hello", cmd_hello);
NRF_CLI_CMD_REGISTER(send, NULL, "Send packet", cmd_send_packet);