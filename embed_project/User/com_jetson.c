#include "com_jetson.h"


extern uint8_t              ReceiveBuff[RECEIVEBUFF_SIZE];
xQueueHandle                bbox_handle;
int                         jetson_info[info_length];

void com_jetson(void* parameter)
{
    bbox_handle = xQueueCreate(queue_length, (info_length * 4));
		while(1) {
				USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE);
				DMA_Cmd(DMA2_Stream2, ENABLE);
				vTaskDelay(200);
				//int t = DMA_GetCurrDataCounter(DMA2_Stream2);
			  //printf("%d",t);
				if (DMA_GetCurrDataCounter(DMA2_Stream2) != RECEIVEBUFF_SIZE) {
            for (int i = 1; i < RECEIVEBUFF_SIZE; ++i) {
        				if (ReceiveBuff[i - 1] == 192) {
        						memcpy(jetson_info, (ReceiveBuff + 4 * i), info_length * sizeof(int));
										xQueueSend(bbox_handle, jetson_info, 0);
				    				break;
								}
				    }
				}
				DMA_SetCurrDataCounter(DMA2_Stream2, RECEIVEBUFF_SIZE);
				DMA_Cmd(DMA2_Stream2, DISABLE);
				/*
            for (int i = 0; i < info_length; ++i) {
                printf("%d", jetson_info[i]);					
				    }
				*/
			}
}
