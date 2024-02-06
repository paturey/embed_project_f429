/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*******************************************************************************
 * ������ͷ�ļ�
 ******************************************************************************/
#include <stdio.h>

/* FreeRTOSͷ�ļ� */
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	/* ������Ӳ����ʼ�� */
	BSP_Init();
	
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,/* ������ں��� */
											 (const char*    )"AppTaskCreate",/* �������� */
											 (uint16_t       )512,					/* ����ջ��С */
											 (void*          )NULL,					/* ������ں������� */
											 (UBaseType_t    )1,						/* ��������ȼ� */
											 (TaskHandle_t   )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */
	/* ����������� */
	if(pdPASS == xReturn)
		vTaskStartScheduler();/* �������񣬿������� */
	else
		return -1;
	
	while(1);/* ��������ִ�е����� */
}

/**
  * @brief ���񴴽�����
  * @note Ϊ�˷���������е����񴴽������������������
  * @param ��
  * @retval ��
  */
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
    taskENTER_CRITICAL();//�����ٽ���
    /*
    xReturn = xTaskCreate((TaskFunction_t)Touch_Task,
                          (const char*      )"Touch_Task",
                          (uint16_t         )256,     
                          (void*            )NULL,    
                          (UBaseType_t      )4,       
                          (TaskHandle_t     )&Touch_Task_Handle);
    if(pdPASS == xReturn)
        printf("����Touch_Task����ɹ���\r\n");
		
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
        printf("����GUI_Task����ɹ���\r\n");
		}
	
    vTaskDelete(AppTaskCreate_Handle);//ɾ��AppTaskCreate����
	
    taskEXIT_CRITICAL();//�˳��ٽ���
}



/**
  * @brief ���������������
  * @note ��
  * @param ��
  * @retval ��
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
  * @brief �弶�����ʼ��
  * @note ���а����ϵĳ�ʼ�����ɷ��������������
  * @param ��
  * @retval ��
  */
static void BSP_Init(void)
{
	/* CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı���������
   * ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ�
   */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��ͬһ�����ȼ����飬ǧ��Ҫ�ٷ��飬�мǡ�
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	Debug_USART_Config();
	
	/* ��������ʼ�� */
	GTP_Init_Panel();	
	/* SDRAM��ʼ�� */
  SDRAM_Init();
	/* LCD��ʼ�� */
	EXTI_Key_Config();
	
	LCD_Init();
		/* ��ʼ��GUI */
	GUI_Init();
	/* ���������� */
	WM_MULTIBUF_Enable(1);
	
  USART_DMA_Config();
//  /* ����WIFIģ�� */
//  AP6181_PDN_INIT();
//  /* �����ļ�ϵͳ������ʱ���SD����ʼ�� */
//  result = f_mount(&fs,"0:",1);
//	if(result != FR_OK)
//	{
//		printf("SD����ʼ��ʧ�ܣ���ȷ��SD������ȷ���뿪���壬��һ��SD�����ԣ�\n");
//		while(1);
//	}
}

///**
//  * @brief AP6181_PDN_INIT
//  * @note ��ֹWIFIģ��
//  * @param ��
//  * @retval ��
//  */
//static void AP6181_PDN_INIT(void)
//{
//  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
//  GPIO_InitTypeDef GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
//  GPIO_Init(GPIOB, &GPIO_InitStructure);	
//  
//  GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //����WiFiģ��
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
