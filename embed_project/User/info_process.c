#include "info_process.h"

extern xQueueHandle                bbox_handle;
static int                         rx_info[5];



void info_process(void* parameter)
{
    while (1) {  
        if (xQueueReceive(bbox_handle, rx_info, 250)) {
				    for (int i = 0; i < info_length; i++) {
                printf("%d", rx_info[i]);
		        }
				}
        
    }
}
