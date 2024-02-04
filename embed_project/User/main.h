#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "./led/bsp_led.h" 
#include "./beep/bsp_beep.h" 
#include "./usart/bsp_debug_usart.h"
#include "./TouchPad/bsp_touchpad.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/gt9xx.h"
#include "./key/bsp_exti.h"
#include "./sdram/bsp_sdram.h"

#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"

#include "ff.h"
#include "diskio.h"
#include "integer.h"

#include "WIDGET_ButtonSimple.h"

#define    _key      "1234567890"