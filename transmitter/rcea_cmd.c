#include "rcea_cmd.h"

#include <stdlib.h>
#include "nrf_cli.h"

#include "nrf_cli_libuarte.h"

#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "nrf_delay.h"

#include "boards.h"

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


/**@brief Function for printing current parameters.
 */
static void cmd_info(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Parameters:\r\n");


    switch (rcea_get_mode()) {
#if EXTENDED_SUPPORT    
        case NRF_RADIO_MODE_IEEE802154_250KBIT:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "Mode: %s\r\n",
                            STRINGIFY_(NRF_RADIO_MODE_IEEE802154_250KBIT));
            break;
#endif
        case NRF_RADIO_MODE_NRF_250KBIT:
        default:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "Mode: %s\r\n",
                            STRINGIFY_(NRF_RADIO_MODE_NRF_250KBIT));
            break;
    }

    switch (rcea_get_txpower()) {
#if EXTENDED_SUPPORT    
        case NRF_RADIO_TXPOWER_POS8DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_POS8DBM));
            break;
        case NRF_RADIO_TXPOWER_POS7DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_POS7DBM));
            break;
        case NRF_RADIO_TXPOWER_POS6DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_POS6DBM));
            break;
        case NRF_RADIO_TXPOWER_POS5DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_POS5DBM));
            break;
        case NRF_RADIO_TXPOWER_POS2DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_POS2DBM));
            break;

#endif
        case NRF_RADIO_TXPOWER_POS4DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_POS4DBM));
            break;
        case NRF_RADIO_TXPOWER_POS3DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_POS3DBM));
            break;
        case NRF_RADIO_TXPOWER_0DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_0DBM));
            break;
        case NRF_RADIO_TXPOWER_NEG4DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_NEG4DBM));
            break;
        case NRF_RADIO_TXPOWER_NEG8DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_NEG8DBM));
            break;
        case NRF_RADIO_TXPOWER_NEG12DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_NEG12DBM));
            break;
        case NRF_RADIO_TXPOWER_NEG16DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_NEG16DBM));
            break;
        case NRF_RADIO_TXPOWER_NEG20DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_NEG20DBM));
            break;
        case NRF_RADIO_TXPOWER_NEG30DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_NEG30DBM));
            break;
        case NRF_RADIO_TXPOWER_NEG40DBM:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: %s\r\n",
                            STRINGIFY_(NRF_RADIO_TXPOWER_NEG40DBM));
            break;
        default:
            nrf_cli_fprintf(p_cli, 
                            NRF_CLI_INFO, 
                            "TX power: unknown\r\n");
            break;
    }    

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Channel: %d\r\n", rcea_get_channel());
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
                    "Channel set to: %d.\r\n", channel);
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

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 8 dBm.\r\n");
}

static void cmd_txpower_pos7dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS7DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 7 dBm.\r\n");
}

static void cmd_txpower_pos6dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS6DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 6 dBm.\r\n");
}

static void cmd_txpower_pos5dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS5DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 5 dBm.\r\n");
}

static void cmd_txpower_pos2dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS2DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 2 dBm.\r\n");
}

#endif

static void cmd_txpower_pos4dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS4DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 4 dBm.\r\n");
}

static void cmd_txpower_pos3dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_POS3DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 4 dBm.\r\n");
}

static void cmd_txpower_0dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_0DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: 0 dBm.\r\n");
}

static void cmd_txpower_neg4dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG4DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -4 dBm.\r\n");    
}

static void cmd_txpower_neg8dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG8DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -8 dBm.\r\n");    
}

static void cmd_txpower_neg12dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG12DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -12 dBm.\r\n");    
}

static void cmd_txpower_neg16dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG16DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -16 dBm.\r\n");    
}

static void cmd_txpower_neg20dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG20DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -20 dBm.\r\n");    
}

static void cmd_txpower_neg30dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG30DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -30 dBm.\r\n");    
}

static void cmd_txpower_neg40dBm(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    rcea_set_tx_power(NRF_RADIO_TXPOWER_NEG40DBM);

    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Output power set to: -40 dBm.\r\n");    
}


/**@brief Function for setting transmit message.
 */
static void cmd_set_message(nrf_cli_t const * p_cli, size_t argc, char **argv)
{

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

        if (rcea_check_length(strlen(argv[1]))) {
            rcea_set_message(argv[1], strlen(argv[1]));
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Message set to: %s.\r\n", argv[1]);
        } else {
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Failed to set message. Invalid length.\r\n", argv[1]);

        }
        
    }

    if (argc == 1) {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,
                    "Message would be randomized. \r\n");
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

    if (argc > 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: bad parameters count.\r\n", argv[0]);
        return;
    }

    if (argc == 2)
    {
        uint32_t repeats;

        repeats = atoi(argv[1]);

        

        if (repeats > 0) {
            int delivered = 0;

            for (uint32_t i = 0; i < repeats; i++) {
                bool success = rcea_send_packet();

                bsp_board_led_invert(BSP_BOARD_LED_0);
                nrf_delay_ms(100);

                if (success) {
                    delivered++;
                }
            }

            float loss = ((float)(atoi(argv[1]) - delivered))/atoi(argv[1]) * 100;
 
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "%s packets transmitted, %d received, %3.1f%% packet loss.\r\n", argv[1], delivered, loss);


        }
    }

    if (argc == 1)
    {
        bool success = rcea_send_packet();
        bsp_board_led_invert(BSP_BOARD_LED_0);

        if (success) {
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Successfully delivered packet.\r\n");
        } else {
            nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Failed to return packet.\r\n");
        }

    }

}


NRF_CLI_CMD_REGISTER(welcome, NULL, "Print welcome message", cmd_welcome);


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

NRF_CLI_CMD_REGISTER(info, NULL, "Print current settings", cmd_info);
NRF_CLI_CMD_REGISTER(channel, NULL, "Set channel \n channel VALUE", cmd_set_channel);
NRF_CLI_CMD_REGISTER(txpower, &m_sub_tx_power, "Set txpower \n txpower VALUE", cmd_set_txpower);
NRF_CLI_CMD_REGISTER(mode, &m_sub_mode, "Set mode \n mode VALUE", cmd_set_mode);
NRF_CLI_CMD_REGISTER(transmit, NULL, 
                     "Transmit message and analyze received packet \n transmit [NUMBER_OF_TRIES]", 
                     cmd_transmit_message);
NRF_CLI_CMD_REGISTER(set_message, NULL, 
                     "Set message to transmit. Length must be less than 256. If there is no message specified payload would be randomized. \n set_message [MESSAGE]", 
                     cmd_set_message);