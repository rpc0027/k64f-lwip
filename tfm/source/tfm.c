/**
 * @file    tfm.c
 * @brief   Application entry point.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>

#include "board.h"

#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "MK64F12.h"

#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_gpio.h"

#include "lwip/opt.h"
#include "lwip/dhcp.h"
#include "lwip/ip_addr.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "ethernetif.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* MAC address configuration. */
#define configMAC_ADDR {0x02, 0x12, 0x13, 0x10, 0x15, 0x11}

/*! @brief Stack size of the temporary lwIP initialization thread. */
#define STACK_INIT_THREAD_STACKSIZE 1024

/*! @brief Priority of the temporary lwIP initialization thread. */
#define STACK_INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void stack_init_thread(void *);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * @brief   Application entry point.
 */
int main(void)
{
	/* Memory protection unit. */
	SYSMPU_Type *base = SYSMPU;

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	/* Disable SYSMPU. */
	base->CESR &= ~SYSMPU_CESR_VLD_MASK;

	/* Initialize lwIP from thread */
	if (sys_thread_new("main", stack_init_thread, NULL, STACK_INIT_THREAD_STACKSIZE, STACK_INIT_THREAD_PRIO) == NULL)
	{
		LWIP_ASSERT("main(): Stack init thread creation failed.", 0);
	}

	vTaskStartScheduler();

	return 0 ;
}

/*!
 * @brief Initializes lwIP stack.
 */
static void stack_init_thread(void *arg)
{
	static struct netif fsl_netif0;
	struct netif *netif = (struct netif *)&fsl_netif0;
	struct dhcp *dhcp;
	ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
	ethernetif_config_t fsl_enet_config0 = {
			.phyAddress = BOARD_ENET0_PHY_ADDRESS, .clockName = kCLOCK_CoreSysClk, .macAddress = configMAC_ADDR,
	};

	/* Default IP configuration. */
	IP4_ADDR(&fsl_netif0_ipaddr, 0U, 0U, 0U, 0U);
	IP4_ADDR(&fsl_netif0_netmask, 0U, 0U, 0U, 0U);
	IP4_ADDR(&fsl_netif0_gw, 0U, 0U, 0U, 0U);

	/* Initialization of the IP stack. */
	tcpip_init(NULL, NULL);

	/* Set up the network interface. */
	netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0, ethernetif0_init,
			tcpip_input);
	netif_set_default(&fsl_netif0);
	netif_set_up(&fsl_netif0);

	/* Query of IP configuration to a local DHCP server. */
	dhcp_start(&fsl_netif0);

	/* Check DHCP state. */
	while (netif_is_up(netif))
	{
		dhcp = netif_dhcp_data(netif);

		if (dhcp != NULL && dhcp->state == DHCP_STATE_BOUND)
		{
			PRINTF("\r\n IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
			PRINTF(" IPv4 Subnet mask : %s\r\n",     ipaddr_ntoa(&netif->netmask));
			PRINTF(" IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));

			vTaskDelete(NULL);
		}

		sys_msleep(20U);
	}
}
