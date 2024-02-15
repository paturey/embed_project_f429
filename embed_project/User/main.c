/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*******************************************************************************
 * 包含的头文件
 ******************************************************************************/
#include <stdio.h>

/* FreeRTOS头文件 */
#include "main.h"
#define MAIN_TASK_PRIO   ( tskIDLE_PRIORITY + 1 )
#define DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 4 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Main_task(void *pvParameters);

static void     AppTaskCreate(void);
TaskHandle_t    AppTaskCreate_Handle = NULL;
TaskHandle_t    Touch_Task_Handle = NULL;
TaskHandle_t    DemoRedraw = NULL;
TaskHandle_t    FatFs_test_Handle = NULL;
TaskHandle_t    RFID_Handle = NULL;
TaskHandle_t    COM_JETSON_Handle = NULL;
TaskHandle_t    INFO_PROCESS_Handle = NULL;



uint8_t         log_in_mode = 0x00;



int main(void)
{
	// init led
	LED_GPIO_Config();
	LED1(LED_OFF);
	LED2(LED_OFF);
	LED3(LED_OFF);

	// init uart 
	Debug_USART_Config();
	printf("starting up!!!\r\n");
	
	// init systick
	//SysTick_Init();

	xTaskCreate(Main_task,"Main", configMINIMAL_STACK_SIZE * 2, NULL, MAIN_TASK_PRIO, NULL);

	/* Start scheduler */
	vTaskStartScheduler();
	
	/* We should never get here as control is now taken by the scheduler */
	for( ;; );
}

void Main_task(void *pvParameters)
{
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
	ETH_BSP_Config();

	/* Initilaize the LwIP stack */
	LwIP_Init();

#ifdef USE_DHCP
	/* Start DHCP Client */
	xTaskCreate(LwIP_DHCP_task, "DHCP", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);
#endif

	for( ;; )
	{
		vTaskDelete(NULL);
	}	
}