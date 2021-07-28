#include <stdlib.h>
#include "nrf_cli.h"

#include "nrf_cli_libuarte.h"



#include "rcea.h"



void rcea_cmd_init(void)
{
    rcea_init();
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

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
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

    rcea_send_packet();

    nrf_cli_fprintf(p_cli,NRF_CLI_NORMAL,
                    "Packet sent... I hope \n\n");
}

static void cmd_send_config(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    rcea_set_channel(11);
/*
    rcea_send_packet();
*/
    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Config sent... I hope \n\n");
}


static void cmd_set_channel(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    //UNUSED_PARAMETER(argc);
    //UNUSED_PARAMETER(argv);
/*
    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }
*/
    if ((argc == 1) || nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc > 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: bad parameters count.\r\n", argv[0]);
        return;
    }


    uint32_t channel;

    channel = atoi(argv[1]);

    rcea_set_channel(channel); //check if channel valid


    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Channel set to: %d.\n\n", channel);
}

static void cmd_set_mode(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if ((argc == 1) || nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc > 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: bad parameters count.\r\n", argv[0]);
        return;
    }

    if (argc == 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Uknown argument: %s.\r\n", argv[1]);
    }
}

static void cmd_set_txpower(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if ((argc == 1) || nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc > 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: bad parameters count.\r\n", argv[0]);
        return;
    }

    if (argc == 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Uknown argument: %s.\r\n", argv[1]);
    }
}


static void cmd_set_message(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    //UNUSED_PARAMETER(argc);
    //UNUSED_PARAMETER(argv);

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }


    if (argc > 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: bad parameters count.\r\n", argv[0]);
        return;
    }



    if (argc == 2) {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Message set to: %s \n\n", argv[0]);
    }

    if (argc == 1) {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Message would be randomized. \n\n");
    }
    
}


static void cmd_transmit_message(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    rcea_send_packet();

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Channel sent... I hope \n\n");
}


NRF_CLI_CMD_REGISTER(welcome, NULL, "Print Hello", cmd_hello);
NRF_CLI_CMD_REGISTER(send, NULL, "Send packet", cmd_send_packet); //remove only for test
NRF_CLI_CMD_REGISTER(config, NULL, "Send config", cmd_send_config); //remove only for test


//TODO subcommands for mode
//TODO subcommands for txpower


NRF_CLI_CMD_REGISTER(channel, NULL, "Set channel \n channel VALUE", cmd_set_channel);
NRF_CLI_CMD_REGISTER(txpower, NULL, "Set txpower \n txpower VALUE", cmd_set_txpower);
NRF_CLI_CMD_REGISTER(mode, NULL, "Set mode \n mode VALUE", cmd_set_mode);
NRF_CLI_CMD_REGISTER(transmit, NULL, 
                     "Transmit message and analyze received packet \n transmit [NUMBER_OF_TRIES]", 
                     cmd_transmit_message);
NRF_CLI_CMD_REGISTER(set_message, NULL, 
                     "Set message to transmit. Length must be less than 256. If there is no message specified payload would be randomized. \n set_message [MESSAGE]", 
                     cmd_set_message);