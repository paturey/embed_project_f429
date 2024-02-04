#include "WIDGET_ButtonSimple.h"
#include "main.h"

extern uint8_t                 log_in_mode;
char                           _key_in[20] = "";
static char                    _key_show[10] = "0";
static char                    _key_[20] = "KEY:";
static int                     flag = 0;

extern TaskHandle_t            DemoRedraw;
extern TaskHandle_t            Touch_Task_Handle;
extern TaskHandle_t            RFID_Handle;

WM_HWIN                        hWin_key;
WM_HWIN                        hWin_MFRC;


void CreateFramewin_task(void* parameter)
{
    
		hWin_key = CreateFramewin();
    while(1) {
        if (log_in_mode == 1) {
        taskENTER_CRITICAL();	
        RFID_Handle = NULL;
        xTaskCreate((TaskFunction_t)RFID_task,
                    (const char*      )"RFID_task",
                    (uint16_t         )1024,     
                    (void*            )NULL,  
                    (UBaseType_t      )2,       
                    (TaskHandle_t*     )&RFID_Handle);
        WM_DeleteWindow(hWin_key);
        hWin_MFRC = CreateSwitch();
				taskEXIT_CRITICAL();
        vTaskDelete(NULL);
		    }
			GUI_Delay(200);
		}
}

WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}

WM_HWIN CreateSwitch(void) 
{
	WM_HWIN hWin = GUI_CreateDialogBox(_aDialogSwitch, GUI_COUNTOF(_aDialogSwitch), _cbDialogSwitch, WM_HBKWIN, 0, 0);
	return hWin;
}

WM_HWIN log_result(void) 
{
	WM_HWIN hWin = GUI_CreateDialogBox(_aDialogResult, GUI_COUNTOF(_aDialogResult), _cbDialogResult, WM_HBKWIN, 0, 0);
	return hWin;
}

static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int NCode;
    int Id;
    static U8 button_3_flag = 0, button_4_flag = 0;

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        hItem = pMsg->hWin;
        FRAMEWIN_SetText(hItem, "STemWIN@EmbedFire STM32F429");
        FRAMEWIN_SetFont(hItem, GUI_FONT_13HB_1);
        FRAMEWIN_SetTitleHeight(hItem, 16);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        BUTTON_SetFont(hItem, GUI_FONT_13HB_1);
        WM_SetCallback(hItem, _cbButton0);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
        WM_SetCallback(hItem, _cbButton1);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
        WM_SetCallback(hItem, _cbButton2);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
        WM_SetCallback(hItem, _cbButton3);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
        WM_SetCallback(hItem, _cbButton4);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
        WM_SetCallback(hItem, _cbButton5);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
        WM_SetCallback(hItem, _cbButton6);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_7);
        WM_SetCallback(hItem, _cbButton7);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_8);
        WM_SetCallback(hItem, _cbButton8);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_9);
        WM_SetCallback(hItem, _cbButton9);
        hItem = WM_GetDialogItem(pMsg->hWin, ENTER);
        WM_SetCallback(hItem, _ENTER);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(hItem, GUI_FONT_COMIC24B_ASCII);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        TEXT_SetText(hItem, "KEY:");
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id) {
        case ID_BUTTON_0: // Notifications sent by 'Button0'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "0");
                printf("%s\n",_key_in);
                break;
			default:
			    break;
            }
			break;
        case ID_BUTTON_1: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "1");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
			break;
		case ID_BUTTON_2: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "2");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
		    break;
		case ID_BUTTON_3: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "3");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
						break;
        case ID_BUTTON_4: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "4");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
						break;
        case ID_BUTTON_5: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "5");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
						break;
        case ID_BUTTON_6: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "6");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
						break;
        case ID_BUTTON_7: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "7");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
						break;
        case ID_BUTTON_8: // Notifications sent by 'Button1'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "8");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
						break;
        case ID_BUTTON_9: 
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                strcat(_key_in, "9");
                printf("%s\n",_key_in);
                break;
			default:
                break;
            }
						break;
        case ENTER: 
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
                show_key(hItem);
                flag = compare_key();
                if (flag) {
									log_result();
                }
								
                else {
                    log_result();
                }
                break;
			default:
                break;
            }
						break;
    default:
        WM_DefaultProc(pMsg);
        break;
				}
		}
}


static void _cbDialogSwitch(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
		    WM_HWIN hItem;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(hItem, GUI_FONT_COMIC24B_ASCII);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        TEXT_SetText(hItem, "Log with RFID");
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

static void _cbDialogResult(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        WM_HWIN hItem;
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(hItem, GUI_FONT_COMIC24B_ASCII);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        if (flag) {
            TEXT_SetText(hItem, "Log Success");
        }
        else {
            TEXT_SetText(hItem, "Log Fail");
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

static void _cbButton0(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("0", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}


static void _cbButton1(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("1", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton2(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("2", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton3(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("3", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton4(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("4", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton5(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("5", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton6(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("6", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton7(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("7", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton8(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("8", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton9(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("9", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

static void _ENTER(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(pMsg->hWin)) {
            GUI_SetColor(GUI_GRAY_C8);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_GRAY_C8);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        } else {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(0xA0ECECEC);
            GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
        }
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(GUI_FONT_13HB_1);
        GUI_DispStringInRect("ENTER", &Rect, GUI_TA_HCENTER |
                            GUI_TA_VCENTER);
        break;
    default:
        BUTTON_Callback(pMsg);
    }
}

void show_key(TREEVIEW_ITEM_Handle hItem)
{
	TEXT_SetFont(hItem, GUI_FONT_COMIC24B_ASCII);
	TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	memset(_key_show, '*', 1);
	strcat(_key_ ,_key_show);
	TEXT_SetText(hItem,_key_);
}

int compare_key()
{
	if (strcmp(_key_in, _key) == 0) {
        return 1;
    } else {
        return 0;
    }
	
}