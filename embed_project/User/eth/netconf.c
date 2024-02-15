/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Network connection configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "main.h"
#include "netconf.h"
#include <stdio.h>
#include "LAN8742A.h"
#include "./led/bsp_led.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"

/* Private typedef -----------------------------------------------------------*/
#define MAX_DHCP_TRIES        4

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif gnetif;
uint32_t TCPTimer = 0;
uint32_t ARPTimer = 0;
uint32_t IPaddress = 0;

#ifdef USE_DHCP
uint32_t DHCPfineTimer = 0;
uint32_t DHCPcoarseTimer = 0;
__IO uint8_t DHCP_state;
#endif
extern __IO uint32_t  EthStatus;

/* Private functions ---------------------------------------------------------*/
void LwIP_DHCP_Process_Handle(void);
/**
* @brief  Initializes the lwIP stack
* @param  None
* @retval None
*/
void LwIP_Init(void)
{
  ip4_addr_t ipaddr;
  ip4_addr_t netmask;
  ip4_addr_t gw;

  /* Create tcp_ip stack thread */
  tcpip_init( NULL, NULL );	

#ifdef USE_DHCP
  ip4_addr_set_zero(&gw);
  ip4_addr_set_zero(&ipaddr);
  ip4_addr_set_zero(&netmask);
#else
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif  

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
  struct ip_addr *netmask, struct ip_addr *gw,
  void *state, err_t (* init)(struct netif *netif),
  err_t (* input)(struct pbuf *p, struct netif *netif))

  Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface.*/
  netif_set_default(&gnetif);

  if (EthStatus == (ETH_INIT_FLAG | ETH_LINK_FLAG))
  { 
    /* Set Ethernet link flag */
    gnetif.flags |= NETIF_FLAG_LINK_UP;

    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);
#ifdef USE_DHCP
    DHCP_state = DHCP_START;
#else
	printf("\r\n	Static IP address	\r\n");
	printf("IP: %d.%d.%d.%d\r\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	printf("NETMASK: %d.%d.%d.%d\r\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
	printf("Gateway: %d.%d.%d.%d\r\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */
  }
  else
  {
    /*  When the netif link is down this function must be called.*/
    netif_set_down(&gnetif);
#ifdef USE_DHCP
    DHCP_state = DHCP_LINK_DOWN;
#endif /* USE_DHCP */
	printf("\r\n	Network Cable is  \r\n");
	printf("	not connected	\r\n");
  }

  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ETH_link_callback);
}

#ifdef USE_DHCP
/**
* @brief  LwIP_DHCP_Process_Handle
* @param  None
* @retval None
*/
void LwIP_DHCP_task(void * pvParameters)
{
  ip4_addr_t ipaddr;
  ip4_addr_t netmask;
  ip4_addr_t gw;
  uint8_t iptab[4] = {0};
  uint8_t iptxt[20];
  uint8_t mactxt[40];

  for (;;)
  {

	  switch (DHCP_state)
	  {
	  case DHCP_START:
	    {
	      DHCP_state = DHCP_WAIT_ADDRESS;
	      dhcp_start(&gnetif);
	      /* IP address should be set to 0 
	         every time we want to assign a new DHCP address */
	      IPaddress = 0;

	      printf("     Looking for    \r\n");
	      printf("     DHCP server    \r\n");
	      printf("     please wait... \r\n");	  
	    }
	    break;
	    
	  case DHCP_WAIT_ADDRESS:
	    {
	      /* Read the new IP address */
	      IPaddress = gnetif.ip_addr.addr;

	      if (IPaddress!=0) 
	      {
	        DHCP_state = DHCP_ADDRESS_ASSIGNED;	

	        /* Stop DHCP */
	        //dhcp_stop(&gnetif);
	     
	        iptab[0] = (uint8_t)(IPaddress >> 24);
	        iptab[1] = (uint8_t)(IPaddress >> 16);
	        iptab[2] = (uint8_t)(IPaddress >> 8);
	        iptab[3] = (uint8_t)(IPaddress);

	        sprintf((char*)iptxt, " %d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]);

	        /* Display the IP address */
	        printf("IP address assigned by a DHCP server:\r\n");
			sprintf((char*)iptxt, " %d.%d.%d.%d", (uint8_t)(gnetif.ip_addr.addr), (uint8_t)(gnetif.ip_addr.addr>>8), (uint8_t)(gnetif.ip_addr.addr>>16), (uint8_t)(gnetif.ip_addr.addr>>24));
			printf("IP addr: %s\r\n", iptxt);
			
			sprintf((char*)iptxt, " %d.%d.%d.%d", (uint8_t)(gnetif.netmask.addr), (uint8_t)(gnetif.netmask.addr>>8), (uint8_t)(gnetif.netmask.addr>>16), (uint8_t)(gnetif.netmask.addr>>24));
			printf("Netmask addr: %s\r\n", iptxt);
			
			sprintf((char*)iptxt, " %d.%d.%d.%d", (uint8_t)(gnetif.gw.addr), (uint8_t)(gnetif.gw.addr>>8), (uint8_t)(gnetif.gw.addr>>16), (uint8_t)(gnetif.gw.addr>>24));
			printf("GW addr: %s\r\n", iptxt);
			
			sprintf((char*)mactxt, " %02x-%02x-%02x-%02x-%02x-%02x", gnetif.hwaddr[0], gnetif.hwaddr[1], gnetif.hwaddr[2], gnetif.hwaddr[3], gnetif.hwaddr[4], gnetif.hwaddr[5]);
			printf("MAC addr: %s\r\n", mactxt);

	        LED1(LED_ON);
	      }
	      else
	      {
		    struct dhcp *dhcp = netif_dhcp_data(&gnetif);
	        /* DHCP timeout */
	        if (dhcp->tries > MAX_DHCP_TRIES)
	        {
	          DHCP_state = DHCP_TIMEOUT;

	          /* Stop DHCP */
	          dhcp_stop(&gnetif);

	          /* Static address used */
	          IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
	          IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
	          IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
	          netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);
	  
	          printf("    DHCP timeout    \r\n");

	          iptab[0] = IP_ADDR3;
	          iptab[1] = IP_ADDR2;
	          iptab[2] = IP_ADDR1;
	          iptab[3] = IP_ADDR0;

	          sprintf((char*)iptxt, "  %d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]);

	          printf("  Static IP address   \r\n");
	          printf("%s\r\n", iptxt);

	          LED1(LED_ON);
	        }
	      }
	    }
	    break;
	  
	  default: break;
	  }

	  /* wait 250 ms */
	  vTaskDelay(250);
   }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
