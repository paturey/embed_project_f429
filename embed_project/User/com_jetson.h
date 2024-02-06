#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "./usart/bsp_debug_usart.h"
#include <stdio.h>
#include <string.h>

#define queue_length                3
#define info_length                 5


void                                com_jetson(void* parameter);
