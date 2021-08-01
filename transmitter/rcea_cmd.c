#include "rcea_cmd.h"

#include <stdlib.h>
#include "nrf_cli.h"

#include "nrf_cli_libuarte.h"



#include "rcea.h"


/**@brief Function for initializing modules, needed for work of cmd.
 */
void rcea_cmd_init(void)
{
    rcea_init();
}


/**@brief Welcome command.
 */
static void cmd_welcome(nrf_cli_t const * p_cli, size_t argc, char **argv)
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


/**@brief Radio test debug command.
 */
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


/**@brief SPI test debug command.
 */
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

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Config sent... I hope \n\n");
}


/**@brief Function for setting radio channel.
 */
static void cmd_set_channel(nrf_cli_t const * p_cli, size_t argc, char **argv)
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

    uint32_t channel;

    channel = atoi(argv[1]);

    if (rcea_check_channel(channel)) {
        rcea_set_channel(channel);

        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Channel set to: %d.\n\n", channel);
    } else {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                        "Invalid channel.\r\n");
    }
}


/**@brief Function for setting radio mode.
 */
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


//TODO NEED MORE MODES
#if EXTENDED_SUPPORT

static void cmd_mode_ieee802154_250kbit(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_mode(NRF_RADIO_MODE_IEEE802154_250KBIT);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Radio mode set to: %s.\r\n", STRINGIFY_(NRF_RADIO_MODE_IEEE802154_250KBIT));
}

#endif

static void cmd_mode_nrf_250kbit(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_mode(NRF_RADIO_MODE_NRF_250KBIT);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Radio mode set to: %s.\r\n", STRINGIFY_(NRF_RADIO_MODE_NRF_250KBIT));
}

/**@brief Function for setting radio output power.
 */
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


#if EXTENDED_SUPPORT

static void cmd_txpower_pos8dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS8DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 8 dBm.\n\n");
}

static void cmd_txpower_pos7dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS7DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 7 dBm.\n\n");
}

static void cmd_txpower_pos6dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS6DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 6 dBm.\n\n");
}

static void cmd_txpower_pos5dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS5DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 5 dBm.\n\n");
}

static void cmd_txpower_pos2dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS2DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 2 dBm.\n\n");
}

#endif

static void cmd_txpower_pos4dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS4DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 4 dBm.\n\n");
}

static void cmd_txpower_pos3dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS3DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 4 dBm.\n\n");
}

static void cmd_txpower_0dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_0DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 0 dBm.\n\n");
}

static void cmd_txpower_neg4dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG4DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -4 dBm.\n\n");    
}

static void cmd_txpower_neg8dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG8DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -8 dBm.\n\n");    
}

static void cmd_txpower_neg12dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG12DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -12 dBm.\n\n");    
}

static void cmd_txpower_neg16dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG16DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -16 dBm.\n\n");    
}

static void cmd_txpower_neg20dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG20DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -20 dBm.\n\n");    
}

static void cmd_txpower_neg30dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG30DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -30 dBm.\n\n");    
}

static void cmd_txpower_neg40dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG40DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -40 dBm.\n\n");    
}


/**@brief Function for setting transmit message.
 */
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


    //TODO check length and set message
    if (argc == 2) {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Message set to: %s \n\n", argv[0]);
    }

    if (argc == 1) {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Message would be randomized. \n\n");
    }
    
}


/**@brief Function for sending message N times. If number is unspecified it will be sent once.
 */
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


NRF_CLI_CMD_REGISTER(welcome, NULL, "Print welcome message", cmd_welcome);
NRF_CLI_CMD_REGISTER(send, NULL, "Send packet", cmd_send_packet); //TODO remove later only for test
NRF_CLI_CMD_REGISTER(config, NULL, "Send config", cmd_send_config); //TODO remove later only for test


//TODO subcommands for mode
NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_mode)
{
#if EXTENDED_SUPPORT
    NRF_CLI_CMD(ieee802154_250Kbit, NULL, "IEEE 802.15.4-2006 250 kbit/s", cmd_ble_ieee),
#endif
    NRF_CLI_CMD(nrf_250Kbit, NULL, "250 kbit/s Nordic proprietary radio mode", cmd_mode_nrf_250kbit),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_tx_power)
{
#if EXTENDED_SUPPORT
    NRF_CLI_CMD(pos8dBm, NULL, "TX power: +8 dBm", cmd_txpower_pos8dBm),
    NRF_CLI_CMD(pos7dBm, NULL, "TX power: +7 dBm", cmd_txpower_pos7dBm),
    NRF_CLI_CMD(pos6dBm, NULL, "TX power: +6 dBm", cmd_txpower_pos6dBm),
    NRF_CLI_CMD(pos5dBm, NULL, "TX power: +5 dBm", cmd_txpower_pos5dBm),
    NRF_CLI_CMD(pos2dBm, NULL, "TX power: +2 dBm", cmd_txpower_pos2dBm),
#endif
    NRF_CLI_CMD(pos4dBm, NULL, "TX power: +4 dBm", cmd_txpower_pos4dBm),
    NRF_CLI_CMD(pos3dBm, NULL, "TX power: +3 dBm", cmd_txpower_pos3dBm),
    NRF_CLI_CMD(pos0dBm, NULL, "TX power: 0 dBm", cmd_txpower_0dBm),
    NRF_CLI_CMD(neg4dBm, NULL, "TX power: -4 dBm", cmd_txpower_neg4dBm),
    NRF_CLI_CMD(neg8dBm, NULL, "TX power: -8 dBm", cmd_txpower_neg8dBm),
    NRF_CLI_CMD(neg12dBm, NULL, "TX power: -12 dBm", cmd_txpower_neg12dBm),
    NRF_CLI_CMD(neg16dBm, NULL, "TX power: -16 dBm", cmd_txpower_neg16dBm),
    NRF_CLI_CMD(neg20dBm, NULL, "TX power: -20 dBm", cmd_txpower_neg20dBm),
    NRF_CLI_CMD(neg30dBm, NULL, "TX power: -30 dBm", cmd_txpower_neg30dBm),
    NRF_CLI_CMD(neg40dBm, NULL, "TX power: -40 dBm", cmd_txpower_neg40dBm),
    NRF_CLI_SUBCMD_SET_END
};


NRF_CLI_CMD_REGISTER(channel, NULL, "Set channel \n channel VALUE", cmd_set_channel);
NRF_CLI_CMD_REGISTER(txpower, &m_sub_tx_power, "Set txpower \n txpower VALUE", cmd_set_txpower);
NRF_CLI_CMD_REGISTER(mode, &m_sub_mode, "Set mode \n mode VALUE", cmd_set_mode);
NRF_CLI_CMD_REGISTER(transmit, NULL, 
                     "Transmit message and analyze received packet \n transmit [NUMBER_OF_TRIES]", 
                     cmd_transmit_message);
NRF_CLI_CMD_REGISTER(set_message, NULL, 
                     "Set message to transmit. Length must be less than 256. If there is no message specified payload would be randomized. \n set_message [MESSAGE]", 
                     cmd_set_message);