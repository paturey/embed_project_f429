#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "stm32f4xx.h"


/* ����õ���4.3��/5���RGB�����䴥��оƬ������GT1151QM������I2C��ַ��ͬ����Ҫ����������л�һ�µ�ַ */
#define LCD_TOUCH_IC_GT1151QM


#ifndef LCD_TOUCH_IC_GT1151QM
#define GTP_ADDRESS                      0xBA
#else
#define GTP_ADDRESS                      0x28
/* 4.3���5����������� GT1151QM ����´���оƬ��
   ֻ��ѡ����������Ļ���е�һ����Ĭ��ѡ5��������������Ϊ1ѡ�У�*/
  #define TOUCH_GT1151QM_LCD_4_3    0   //ʹ�õ���4.3�������(GT1151QM)
  #define TOUCH_GT1151QM_LCD_5      1   //ʹ�õ���5�������(GT1151QM)
#endif



#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


#define GTP_I2C                          I2C2
#define GTP_I2C_CLK                      RCC_APB1Periph_I2C2

#define GTP_I2C_SCL_PIN                  GPIO_Pin_4                  
#define GTP_I2C_SCL_GPIO_PORT            GPIOH                       
#define GTP_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define GTP_I2C_SCL_SOURCE               GPIO_PinSource4
#define GTP_I2C_SCL_AF                   GPIO_AF_I2C2

#define GTP_I2C_SDA_PIN                  GPIO_Pin_5                  
#define GTP_I2C_SDA_GPIO_PORT            GPIOH                    
#define GTP_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define GTP_I2C_SDA_SOURCE               GPIO_PinSource5
#define GTP_I2C_SDA_AF                   GPIO_AF_I2C2

#define GTP_RST_GPIO_PORT                GPIOI
#define GTP_RST_GPIO_CLK                 RCC_AHB1Periph_GPIOI
#define GTP_RST_GPIO_PIN                 GPIO_Pin_8

#define GTP_INT_GPIO_PORT                GPIOD
#define GTP_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define GTP_INT_GPIO_PIN                 GPIO_Pin_13
#define GTP_INT_READ()                   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)

//#define GTP_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOD
//#define GTP_INT_EXTI_PINSOURCE           EXTI_PinSource12


static uint32_t I2C_TIMEOUT_UserCallback(void);


uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);

void I2C_Touch_Init(void);
void I2C_ResetChip(void);


#endif /* __I2C_TOUCH_H */
