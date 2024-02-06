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


static void     AppTaskCreate(void);
TaskHandle_t    AppTaskCreate_Handle = NULL;
TaskHandle_t    Touch_Task_Handle = NULL;
TaskHandle_t    DemoRedraw = NULL;
TaskHandle_t    FatFs_test_Handle = NULL;
TaskHandle_t    RFID_Handle = NULL;
TaskHandle_t    COM_JETSON_Handle = NULL;
TaskHandle_t    INFO_PROCESS_Handle = NULL;



uint8_t         log_in_mode = 0x00;



static void Touch_Task(void* parameter);
static void GUI_Task(void* parameter);

static void BSP_Init(void);


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	/* 开发板硬件初始化 */
	BSP_Init();
	
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,/* 任务入口函数 */
											 (const char*    )"AppTaskCreate",/* 任务名称 */
											 (uint16_t       )512,					/* 任务栈大小 */
											 (void*          )NULL,					/* 任务入口函数参数 */
											 (UBaseType_t    )1,						/* 任务的优先级 */
											 (TaskHandle_t   )&AppTaskCreate_Handle);/* 任务控制块指针 */
	/* 启动任务调度 */
	if(pdPASS == xReturn)
		vTaskStartScheduler();/* 启动任务，开启调度 */
	else
		return -1;
	
	while(1);/* 正常不会执行到这里 */
}

/**
  * @brief 任务创建函数
  * @note 为了方便管理，所有的任务创建都放在这个函数里面
  * @param 无
  * @retval 无
  */
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
    taskENTER_CRITICAL();//进入临界区
    /*
    xReturn = xTaskCreate((TaskFunction_t)Touch_Task,
                          (const char*      )"Touch_Task",
                          (uint16_t         )256,     
                          (void*            )NULL,    
                          (UBaseType_t      )4,       
                          (TaskHandle_t     )&Touch_Task_Handle);
    if(pdPASS == xReturn)
        printf("创建Touch_Task任务成功！\r\n");
		
    xReturn = xTaskCreate((TaskFunction_t)CreateFramewin_task,
                          (const char*      )"DemoRedraw",
                          (uint16_t         )2048,     
                          (void*            )NULL,    
                          (UBaseType_t      )3,       
                          (TaskHandle_t*     )&DemoRedraw);	
    */
    xReturn = xTaskCreate((TaskFunction_t)com_jetson,
                          (const char*      )"com_jetson",
                          (uint16_t         )2048,     
                          (void*            )NULL,    
                          (UBaseType_t      )3,       
                          (TaskHandle_t*     )&COM_JETSON_Handle);
													
    xReturn = xTaskCreate((TaskFunction_t)info_process,
                          (const char*      )"info_process",
                          (uint16_t         )2048,     
                          (void*            )NULL,    
                          (UBaseType_t      )2,       
                          (TaskHandle_t*     )&INFO_PROCESS_Handle);
    

													
    if(pdPASS == xReturn) { 
        printf("创建GUI_Task任务成功！\r\n");
		}
	
    vTaskDelete(AppTaskCreate_Handle);//删除AppTaskCreate任务
	
    taskEXIT_CRITICAL();//退出临界区
}



/**
  * @brief 触摸检测任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void Touch_Task(void* parameter)
{
	while(1)
	{
		GT9xx_GetOnePiont();
		vTaskDelay(20);
	}
}


/**
  * @brief 板级外设初始化
  * @note 所有板子上的初始化均可放在这个函数里面
  * @param 无
  * @retval 无
  */
static void BSP_Init(void)
{
	/* CRC和emWin没有关系，只是他们为了库的保护而做的
   * 这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。
   */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用同一个优先级分组，千万不要再分组，切记。
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	Debug_USART_Config();
	
	/* 触摸屏初始化 */
	GTP_Init_Panel();	
	/* SDRAM初始化 */
  SDRAM_Init();
	/* LCD初始化 */
	EXTI_Key_Config();
	
	LCD_Init();
		/* 初始化GUI */
	GUI_Init();
	/* 开启三缓冲 */
	WM_MULTIBUF_Enable(1);
	
  USART_DMA_Config();
//  /* 禁用WIFI模块 */
//  AP6181_PDN_INIT();
//  /* 挂载文件系统，挂载时会对SD卡初始化 */
//  result = f_mount(&fs,"0:",1);
//	if(result != FR_OK)
//	{
//		printf("SD卡初始化失败，请确保SD卡已正确接入开发板，或换一张SD卡测试！\n");
//		while(1);
//	}
}

///**
//  * @brief AP6181_PDN_INIT
//  * @note 禁止WIFI模块
//  * @param 无
//  * @retval 无
//  */
//static void AP6181_PDN_INIT(void)
//{
//  /*定义一个GPIO_InitTypeDef类型的结构体*/
//  GPIO_InitTypeDef GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
//  GPIO_Init(GPIOB, &GPIO_InitStructure);	
//  
//  GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //禁用WiFi模块
//}

/*********************************************END OF FILE**********************/
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) {
        LED1_TOGGLE;
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
     		log_in_mode ^= 0x01;
    }
}
