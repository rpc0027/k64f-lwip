/**
 * @file    main.c
 *
 * @brief   Application entry point.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "board.h"

#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "MK64F12.h"

#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_gpio.h"

#include "FreeRTOS.h"
#include "message_buffer.h"

#include "lwip/opt.h"
#include "lwip/dhcp.h"
#include "lwip/ip_addr.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "ethernetif.h"

#include "lcd.h"
#include "led.h"
#include "pwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* MAC address configuration. */
#define configMAC_ADDR {0x02, 0x12, 0x13, 0x10, 0x15, 0x11}
/* Top row of the LCD. */
#define TOP_ROW 0U
/* Bottom row of the LCD. */
#define BOTTOM_ROW 1U
/* Number of bytes used in commands name. */
#define COMMAND_SIZE 4
/* Name of the LED command. */
#define LED_COMMAND "led:"
/* Index of the color argument in the received LED command. */
#define LED_COLOR_INDEX 4
/* Name of the MSG command. */
#define MSG_COMMAND "msg:"
/* Index of the line in the received MSG command. */
#define MSG_ROW_INDEX 4
/* Index of the text in the received MSG command. */
#define MSG_TXT_INDEX 6
/* Name of the PWM command. */
#define PWM_COMMAND "pwm:"
/* Index of the pwm device argument in the received PWM command. */
#define PWM_DEVICE_INDEX 4
/* Index of the percentage argument in the received PWM command. */
#define PWM_PERCENTAGE_INDEX 6

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void stack_init_thread(void * arg);
void tcp_listener_thread(void * arg);
void rx_command_check(char * buffer, uint16_t null_terminator_position);
void led_thread(void * arg);
void msg_thread(void * arg);
void pwm_thread(void * arg);
uint8_t range_adjust(long value);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Handler for the messages for led_thread. */
MessageBufferHandle_t led_handler;
/* Handler for the messages for msg_thread. */
MessageBufferHandle_t msg_handler;
/* Handler for the messages for pwm_thread. */
MessageBufferHandle_t pwm_handler;

/* Number of bytes of the messages. */
#define BUFFER_LENGTH 1400

uint8_t messageBuffer[BUFFER_LENGTH];

TaskHandle_t tcp_listener_handle;


/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * @brief   Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Disable Memory Protection Unit. */
    SYSMPU->CESR &= ~SYSMPU_CESR_VLD_MASK;

    /* LCD initialization. */
    LCD_Init(0x3F, 16, 2, LCD_5x8DOTS);

    /* Creation of the message buffers. */
    led_handler = xMessageBufferCreate(BUFFER_LENGTH);
    msg_handler = xMessageBufferCreate(BUFFER_LENGTH);
    pwm_handler = xMessageBufferCreate(BUFFER_LENGTH);

    /* Tasks creation. */
    if (xTaskCreate(stack_init_thread, "stack_init_thread",
            DEFAULT_THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO, NULL) != pdPASS)
    {
        PRINTF("stack_init_thread creation failed.\n");
    }

    if (xTaskCreate(tcp_listener_thread, "tcp_listener_thread",
            DEFAULT_THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO , &tcp_listener_handle) != pdPASS)
    {
        PRINTF("tcp_listener_thread creation failed.\n");
    }
    /* Task suspended until the TCP/IP stack is initialized. */
    vTaskSuspend(tcp_listener_handle);

    if (xTaskCreate(led_thread, "led_thread",
            DEFAULT_THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO - 1, NULL) != pdPASS)
    {
        PRINTF("led_thread creation failed.\n");
    }

    if (xTaskCreate(msg_thread, "msg_thread",
            DEFAULT_THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO - 1, NULL) != pdPASS)
    {
        PRINTF("msg_thread creation failed.\n");
    }

    if (xTaskCreate(pwm_thread, "pwm_thread",
            DEFAULT_THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO - 1, NULL) != pdPASS)
    {
        PRINTF("pwm_thread creation failed.\n");
    }

    vTaskStartScheduler();

    return 0 ;
}

/*!
 * @brief Initializes lwIP stack.
 */
void stack_init_thread(void * arg)
{
    LCD_clear_row(TOP_ROW);
    LCD_printstr("Obtaining IP");
    LCD_clear_row(BOTTOM_ROW);
    LCD_printstr("Please wait...");

    struct netif netif;
    struct dhcp * dhcp;

    ip4_addr_t ip_address;
    ip4_addr_t netmask;
    ip4_addr_t gateway;

    ethernetif_config_t config =
    {
            .phyAddress = BOARD_ENET0_PHY_ADDRESS,
            .clockName = kCLOCK_CoreSysClk,
            .macAddress = configMAC_ADDR,
    };

    /* Initialization of the TCP/IP stack. */
    tcpip_init(NULL, NULL);

    /* Set up the network interface. */
    netif_add(&netif, &ip_address, &netmask, &gateway, &config,
            ethernetif0_init, tcpip_input);
    netif_set_default(&netif);
    netif_set_up(&netif);

    /* Start DHCP negotiation. */
    dhcp_start(&netif);

    for(;;)
    {
        dhcp = netif_dhcp_data(&netif);

        if (dhcp->state == DHCP_STATE_BOUND)
        {
            PRINTF("IPv4 Address : %s\n", ipaddr_ntoa(&netif.ip_addr));
            PRINTF("IPv4 Netmask : %s\n", ipaddr_ntoa(&netif.netmask));
            PRINTF("IPv4 Gateway : %s\n", ipaddr_ntoa(&netif.gw));

            LCD_clear_row(TOP_ROW);
            LCD_printstr("IPv4 Address:");
            LCD_clear_row(BOTTOM_ROW);
            LCD_printstr(ipaddr_ntoa(&netif.ip_addr));

            vTaskResume(tcp_listener_handle);
            vTaskDelete(NULL);
        }
    }
}

/*!
 * @brief Listens TCP packages.
 *
 */
void tcp_listener_thread(void * arg)
{
    struct netbuf  * netbuf;
    struct netconn * netconn;
    struct netconn * newconn;
    char buffer[BUFFER_LENGTH];

    netconn = netconn_new(NETCONN_TCP);
    netconn_bind(netconn, NULL, 1234);
    netconn_listen(netconn);

    for (;;)
    {
        netconn_accept(netconn, &newconn);

        while (ERR_OK == netconn_recv(newconn, &netbuf))
        {
            do
            {
                netbuf_copy(netbuf, buffer, sizeof(buffer));
                buffer[netbuf->p->tot_len] = '\0';

                rx_command_check(buffer, netbuf->p->tot_len);
            }
            while (netbuf_next(netbuf) >= 0);

            netbuf_delete(netbuf);
        }

        netconn_delete(newconn);
    }
}

void rx_command_check(char * buffer, uint16_t null_terminator_position)
{
    if (COMMAND_SIZE > null_terminator_position)
    {
        PRINTF("Invalid command: too short.\n");
    }
    else
    {
        if (strncmp(buffer, LED_COMMAND, COMMAND_SIZE) == 0)
        {
            xMessageBufferSend(led_handler, (void *) buffer, strlen(buffer), 0);
        }
        else if (strncmp(buffer, MSG_COMMAND, COMMAND_SIZE) == 0)
        {
            xMessageBufferSend(msg_handler, (void *) buffer, strlen(buffer) + 1, 0);
        }
        else if (strncmp(buffer, PWM_COMMAND, COMMAND_SIZE) == 0)
        {
            xMessageBufferSend(pwm_handler, (void *) buffer, strlen(buffer), 0);
        }
        else
        {
            PRINTF("Invalid command.\n");
        }
    }
}

void led_thread(void *arg)
{
    char buffer[BUFFER_LENGTH];
    size_t xReceivedBytes;
    const TickType_t xBlockTime = pdMS_TO_TICKS(20);

    for (;;)
    {
        xReceivedBytes = xMessageBufferReceive(led_handler, (void *) buffer,
                sizeof(buffer), xBlockTime);

        if (xReceivedBytes)
        {
            switch (buffer[LED_COLOR_INDEX])
            {
            case 'r':
                turn_on_red();
                break;

            case 'g':
                turn_on_green();
                break;

            case 'b':
                turn_on_blue();
                break;

            case 'y':
                turn_on_yellow();
                break;

            case 'm':
                turn_on_magenta();
                break;

            case 'c':
                turn_on_cyan();
                break;

            case 'w':
                turn_on_white();
                break;

            case 'o':
                turn_off_leds();
                break;

            default:
                PRINTF("Invalid LED color.\n");
            }
        }
    }
}

void msg_thread(void *arg)
{
    char buffer[BUFFER_LENGTH];
    size_t xReceivedBytes;
    const TickType_t xBlockTime = pdMS_TO_TICKS(20);

    for (;;)
    {
        xReceivedBytes = xMessageBufferReceive(msg_handler, (void *) buffer,
                sizeof(buffer), xBlockTime);

        if (xReceivedBytes)
        {
            if (buffer[MSG_ROW_INDEX] == '0')
            {
                LCD_clear_row(TOP_ROW);
                LCD_printstr(buffer + MSG_TXT_INDEX);
            }
            else if (buffer[MSG_ROW_INDEX == '1'])
            {
                LCD_clear_row(BOTTOM_ROW);
                LCD_printstr(buffer + MSG_TXT_INDEX);
            }
            else
            {
                PRINTF("Invalid LCD line.\n");
            }
        }
    }
}

void pwm_thread(void *arg)
{
    char buffer[BUFFER_LENGTH];
    size_t xReceivedBytes;
    const TickType_t xBlockTime = pdMS_TO_TICKS(20);
    uint8_t percentage;

    for (;;)
    {
        xReceivedBytes = xMessageBufferReceive(pwm_handler, (void *) buffer,
                sizeof(buffer), xBlockTime);

        if (xReceivedBytes)
        {
            percentage = range_adjust(
                    strtol(buffer + PWM_PERCENTAGE_INDEX, NULL, 10));

            switch (buffer[PWM_DEVICE_INDEX])
            {
            case 'w':
                update_pwm_dutycyle(WHITE_PWM, WHITE_CHANNEL, percentage);
                break;

            case 'g':
                update_pwm_dutycyle(GREEN_PWM, GREEN_CHANNEL,  percentage);
                break;

            case 'y':
                update_pwm_dutycyle(YELLOW_PWM, YELLOW_CHANNEL, percentage);
                break;

            case 'r':
                update_pwm_dutycyle(RED_PWM, RED_CHANNEL, percentage);
                break;

            default:
                PRINTF("Invalid PWM device.\n");
            }
        }
    }
}

/*!
 * @brief Truncate values lower than 0 and higher than 100.
 * @param[in] value The value to adjust.
 * @return The adjusted value.
 */
uint8_t range_adjust(long value)
{
    uint8_t corrected_value;

    if (value < 0)
    {
        corrected_value = 0U;
    }
    else if (value > 100)
    {
        corrected_value = 100U;
    }
    else
    {
        corrected_value = (uint8_t) value;
    }

    return corrected_value;
}

/*** end of file ***/
