#include "stm32f4xx.h"
#include "./MFRC/RC522.h"
#define            	MFRC_SPI_GPIO_CLK                       RCC_APB2Periph_GPIOB

#define             MFRC_SPI_SDA_PIN		                   GPIO_Pin_5
#define             MFRC_SPI_SDA_PORT		                   GPIOB

#define             MFRC_SPI_SCK_PIN		                   GPIO_Pin_11
#define             MFRC_SPI_SCK_PORT		                   GPIOB

#define	            MFRC_SPI_MOSI_PIN	                    GPIO_Pin_12
#define	            MFRC_SPI_MOSI_PORT	                    GPIOB

#define	            MFRC_SPI_MISO_PIN	                    GPIO_Pin_13
#define	            MFRC_SPI_MISO_PORT	                    GPIOB

#define	            MFRC_RST_PIN	                        GPIO_Pin_14
#define	            MFRC_RST_PORT	                        GPIOB

#define             MFRC_SS_ENABLE()                        GPIO_ResetBits(MFRC_SPI_SDA_PORT, MFRC_SPI_SDA_PIN)    
#define             MFRC_SS_DISABLE()                       GPIO_SetBits (MFRC_SPI_SDA_PORT, MFRC_SPI_SDA_PIN)  

#define             MFRC_CLK_HIGH()                         GPIO_SetBits(MFRC_SPI_SCK_PORT, MFRC_SPI_SCK_PIN)    
#define             MFRC_CLK_LOW()                          GPIO_ResetBits(MFRC_SPI_SCK_PORT, MFRC_SPI_SCK_PIN) 

#define             MFRC_MOSI_1()                           GPIO_SetBits(MFRC_SPI_MOSI_PORT, MFRC_SPI_MOSI_PIN) 
#define             MFRC_MOSI_0()                           GPIO_ResetBits(MFRC_SPI_MOSI_PORT, MFRC_SPI_MOSI_PIN)

#define             MFRC_MISO()                             GPIO_ReadInputDataBit(MFRC_SPI_MISO_PORT, MFRC_SPI_MISO_PIN)

#define             MFRC_RST_ENABLE()                       GPIO_ResetBits(MFRC_RST_PORT, MFRC_RST_PIN)
#define             MFRC_RST_DISABLE()                      GPIO_SetBits(MFRC_RST_PORT, MFRC_RST_PIN)

static void         delay_MFRC( __IO uint32_t ulCount );
void                MFRC_write_byte(uint8_t byte);
void                spi_MFRC_init(void);

void                RC522_Write_Register(uint8_t Address, uint8_t Data);
void                RC522_SetBit_Register(uint8_t Address, uint8_t Mask);
void                RC522_ClearBit_Register( uint8_t Address, uint8_t mask );

void                RC522_Antenna_On(void);
void                RC522_Antenna_Off(void);
void                RC522_Rese(void);
void                RC522_Config_Type(char Type);

char PcdComMF522 (uint8_t ucCommand, uint8_t * pInData, uint8_t ucInLenByte, uint8_t * pOutData, uint32_t * pOutLenBit);
char PcdRequest(uint8_t ucReq_code, uint8_t * pTagType);
char PcdAnticoll(uint8_t * pSnr);
void CalulateCRC(uint8_t * pIndata, u8 ucLen, uint8_t * pOutData);
char PcdSelect(uint8_t * pSnr);
char PcdAuthState(uint8_t ucAuth_mode, uint8_t ucAddr, uint8_t * pKey, uint8_t * pSnr);
char PcdWrite(uint8_t ucAddr, uint8_t * pData);
char PcdRead(uint8_t ucAddr, uint8_t * pData);
char PcdHalt(void);


uint8_t             MFRC_read_byte(void);
uint8_t             RC522_Read_Register(uint8_t Address);

