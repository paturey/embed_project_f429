#include "RFID_task.h"

extern uint8_t                 log_in_mode;
extern TaskHandle_t            DemoRedraw;
extern TaskHandle_t            RFID_Handle;
extern WM_HWIN                 hWin_MFRC;

extern void                    CreateFramewin_task(void* parameter);


void RFID_task(void) {
    char      cStr[30];
    char      ucStatusReturn;    //返回状态 
    u8        ucArray_ID [4];                                                                                     
                           
    spi_MFRC_init();
    RC522_Rese();
    RC522_Config_Type('A');
    while (1) { 
        if (!log_in_mode) {
            taskENTER_CRITICAL();
            DemoRedraw = NULL;
            xTaskCreate((TaskFunction_t)CreateFramewin_task,
                        (const char*      )"DemoRedraw1",
                        (uint16_t         )1024,     
                        (void*            )NULL,  
                        (UBaseType_t      )3,       
                        (TaskHandle_t*     )&DemoRedraw);
            WM_DeleteWindow(hWin_MFRC);
						taskEXIT_CRITICAL();
            vTaskDelete(NULL);	
        }
				GUI_Delay(200);
		/* 寻卡（方式：范围内全部），第一次寻卡失败后再进行一次，寻卡成功时卡片序列传入数组ucArray_ID中 */
        taskENTER_CRITICAL();
				if (( ucStatusReturn = PcdRequest( PICC_REQALL, ucArray_ID )) != MI_OK) {
            ucStatusReturn = PcdRequest(PICC_REQALL, ucArray_ID);
        }
        printf("%c", ucStatusReturn);
        if (ucStatusReturn == MI_OK) {
			    /* 防冲突操作，被选中的卡片序列传入数组ucArray_ID中 */
            if (PcdAnticoll ( ucArray_ID ) == MI_OK) {
			    sprintf( cStr, "The Card ID is: %02X%02X%02X%02X", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ]);				
		        printf("%s\r\n", cStr); 
            }
        }
				taskEXIT_CRITICAL();
    }
}
