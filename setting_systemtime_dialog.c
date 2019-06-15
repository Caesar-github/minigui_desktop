/*
 * This is a every simple sample for MiniGUI.
 * It will create a main window and display a string of "Hello, world!" in it.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h> 
#include <math.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>

#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include <unistd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"

#define SLIDE_DISTANCE 100

#define SYSTEMTIME_WHOLE_BUTTON_NUM 10

static BITMAP list_sel_bmap;
static BITMAP seldot_bmap[2];
static int list_sel = 0;
static int batt = 0;
#define SYSTEMTIME_NUM    (RES_STR_SYSTEMTIME_OFF3 - RES_STR_SYSTEMTIME_DATA + 1)

static int on_1 = 0;
static int off_1 = 0;
static int on_2 = 0;
static int off_2 = 0;
static int on_3 = 0;
static int off_3 = 0;


static touch_pos touch_pos_down,touch_pos_up,touch_pos_old;

static int check_button(int x,int y)
{
    if((x <= BACK_PINT_X + BACK_PINT_W ) &&
        (x >= BACK_PINT_X) &&
        (y <= BACK_PINT_Y + BACK_PINT_H ) &&
        (y >= BACK_PINT_Y))
        return 0;
    if(y > SETTING_LIST_STR_PINT_Y)
        return (((y - SETTING_LIST_STR_PINT_Y) / SETTING_LIST_STR_PINT_SPAC)+1);
    return -1;

}

static int loadres(void)
{
    int i;
    char img[128];
    char *respath = get_ui_image_path();

    snprintf(img, sizeof(img), "%slist_sel.png", respath);
    if (LoadBitmap(HDC_SCREEN, &list_sel_bmap, img))
        return -1;

    for (i = 0; i < 2; i++) {
        snprintf(img, sizeof(img), "%sdot%d.png", respath, i);
        if (LoadBitmap(HDC_SCREEN, &seldot_bmap[i], img))
            return -1;
    }
    return 0;
}

static void unloadres(void)
{
    int i;

    UnloadBitmap(&list_sel_bmap);
    for (i = 0; i < 2; i++) {
        UnloadBitmap(&seldot_bmap[i]);
    }
}

static void menu_back(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
    EndDialog(hWnd, wParam);
}

static LRESULT setting_systemtime_dialog_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    //printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
    switch (message) {
    case MSG_INITDIALOG: {
    	  DWORD bkcolor;
        HWND hFocus = GetDlgDefPushButton(hWnd);
        loadres();
        bkcolor = GetWindowElementPixel(hWnd, WE_BGC_WINDOW);
        SetWindowBkColor(hWnd, bkcolor);
        if (hFocus)
            SetFocus(hFocus);
        batt = battery;
        list_sel = 0;
        SetTimer(hWnd, _ID_TIMER_SETTING_SYSTEMTIME, TIMER_SETTING_SYSTEMTIME);
        return 0;
    }
    case MSG_TIMER: {
        if (wParam == TIMER_SETTING_SYSTEMTIME) {
#ifdef ENABLE_BATT
            if (batt != battery) {
                batt = battery;
                InvalidateRect(hWnd, &msg_rcBatt, TRUE);
            }
#endif
#ifdef ENABLE_WIFI
				InvalidateRect(hWnd, &msg_rcWifi, TRUE);
#endif

        }
        break;
    }
    case MSG_PAINT:
    {
        int i;
        int page;
        int cur_page;
        struct file_node *file_node_temp;
        gal_pixel old_brush;
        gal_pixel pixle = 0xffffffff;

        hdc = BeginPaint(hWnd);
        old_brush = SetBrushColor(hdc, pixle);
        FillBoxWithBitmap(hdc, BG_PINT_X,
                               BG_PINT_Y, BG_PINT_W,
                               BG_PINT_H, &background_bmap);
        FillBoxWithBitmap(hdc, BACK_PINT_X, BACK_PINT_Y,
                               BACK_PINT_W, BACK_PINT_H,
                               &back_bmap);
#ifdef ENABLE_BATT
        FillBoxWithBitmap(hdc, BATT_PINT_X, BATT_PINT_Y,
                               BATT_PINT_W, BATT_PINT_H,
                               &batt_bmap[batt]);
#endif
#ifdef ENABLE_WIFI
		if(get_wifi()==RK_WIFI_State_IDLE) 
		{
        	FillBoxWithBitmap(hdc, WIFI_PINT_X, WIFI_PINT_Y,
                              	 WIFI_PINT_W, WIFI_PINT_H,
                               	&wifi_disabled_bmap);
			}
		else if(get_wifi()==RK_WIFI_State_CONNECTED){
			        	FillBoxWithBitmap(hdc, WIFI_PINT_X, WIFI_PINT_Y,
                              	 WIFI_PINT_W, WIFI_PINT_H,
                               	&wifi_connected_bmap);
		}
		else{
			FillBoxWithBitmap(hdc, WIFI_PINT_X, WIFI_PINT_Y,
									 WIFI_PINT_W, WIFI_PINT_H,
									&wifi_disconnected_bmap);
		}
		
#endif

		RECT msg_rcTime;
		char *sys_time_str[6];
		snprintf(sys_time_str, sizeof(sys_time_str), "%02d:%02d", time_hour / 60, time_hour % 60, time_min / 60, time_min % 60);
		msg_rcTime.left = REALTIME_PINT_X;
		msg_rcTime.top = REALTIME_PINT_Y;
		msg_rcTime.right = REALTIME_PINT_X + REALTIME_PINT_W;
		msg_rcTime.bottom = REALTIME_PINT_Y + REALTIME_PINT_H;
		SetBkColor(hdc, COLOR_transparent);
		SetBkMode(hdc,BM_TRANSPARENT);
		SetTextColor(hdc, RGB2Pixel(hdc, 0xff, 0xff, 0xff));
		SelectFont(hdc, logfont_title);
		DrawText(hdc, sys_time_str, -1, &msg_rcTime, DT_TOP);

//==================display volume icon============================

		BITMAP *volume_display;

		
		if(get_volume()==0) volume_display=&volume_0;
		else if ( get_volume()>0  && get_volume()<=32)	volume_display=&volume_1;
		else if ( get_volume()>32  && get_volume()<=66)  volume_display=&volume_2;
		else volume_display=&volume_3;

		FillBoxWithBitmap(hdc, VOLUME_PINT_X, VOLUME_PINT_Y,
							   VOLUME_PINT_W, VOLUME_PINT_H,
							   volume_display);



        SetBkColor(hdc, COLOR_transparent);
        SetBkMode(hdc,BM_TRANSPARENT);
        SetTextColor(hdc, RGB2Pixel(hdc, 0xff, 0xff, 0xff));
        SelectFont(hdc, logfont);
        DrawText(hdc, res_str[RES_STR_TITLE_SYSTEMTIME], -1, &msg_rcTitle, DT_TOP);
        FillBox(hdc, TITLE_LINE_PINT_X, TITLE_LINE_PINT_Y, TITLE_LINE_PINT_W, TITLE_LINE_PINT_H);

        page = (SYSTEMTIME_NUM + SETTING_NUM_PERPAGE - 1) / SETTING_NUM_PERPAGE;
        cur_page = list_sel / SETTING_NUM_PERPAGE;

		RECT msg_temp;
		char *sys_data_str[11];
			
        for (i = 0; i < SETTING_NUM_PERPAGE; i++) {
            RECT msg_rcFilename;

            if ((cur_page * SETTING_NUM_PERPAGE + i) >= SYSTEMTIME_NUM)
                break;

            msg_rcFilename.left = SETTING_LIST_STR_PINT_X;
            msg_rcFilename.top = SETTING_LIST_STR_PINT_Y + SETTING_LIST_STR_PINT_SPAC * i;
            msg_rcFilename.right = LCD_W - msg_rcFilename.left;
            msg_rcFilename.bottom = msg_rcFilename.top + SETTING_LIST_STR_PINT_H;

            if (i == list_sel % SETTING_NUM_PERPAGE)
               FillBoxWithBitmap(hdc, 0, msg_rcFilename.top - 9, LCD_W, SETTING_LIST_SEL_PINT_H, &list_sel_bmap);

  //         if ((cur_page * SETTING_NUM_PERPAGE + i) == get_themestyle())
  //             FillBoxWithBitmap(hdc, SETTING_LIST_DOT_PINT_X, msg_rcFilename.top, SETTING_LIST_DOT_PINT_W, SETTING_LIST_DOT_PINT_H, &seldot_bmap[1]);
   //        else
   //           FillBoxWithBitmap(hdc, SETTING_LIST_DOT_PINT_X, msg_rcFilename.top, SETTING_LIST_DOT_PINT_W, SETTING_LIST_DOT_PINT_H, &seldot_bmap[0]);
            SelectFont(hdc, logfont);
            DrawText(hdc, res_str[RES_STR_SYSTEMTIME_DATA + cur_page * SETTING_NUM_PERPAGE + i], -1, &msg_rcFilename, DT_TOP);

		//==========================================//
		
		    snprintf(sys_data_str, sizeof(sys_data_str), "%04d-%02d-%02d", systemtime_year, systemtime_month,systemtime_day);
			msg_temp = msg_rcFilename;
			
			switch (RES_STR_SYSTEMTIME_DATA + cur_page * SETTING_NUM_PERPAGE + i)
				{
					case RES_STR_SYSTEMTIME_DATA:
						msg_temp.left = msg_rcFilename.left+825;
						DrawText(hdc, sys_data_str, -1, &msg_temp, DT_TOP);
					break;


					case RES_STR_SYSTEMTIME_TIME:
						msg_temp.left = msg_rcFilename.left+900;
						DrawText(hdc, sys_time_str, -1, &msg_temp, DT_TOP);						
					break;

					
					case RES_STR_SYSTEMTIME_FORMAT:
						msg_temp.left = msg_rcFilename.left+900;
						SelectFont(hdc, logfont);
						DrawText(hdc, "24", -1, &msg_temp, DT_TOP);	
					break;

					
					case RES_STR_SYSTEMTIME_ON1:
						msg_temp.left = msg_rcFilename.left+500;
						SelectFont(hdc, logfont);
						if(on_1) DrawText(hdc, res_str[RES_STR_ENABLE], -1, &msg_temp, DT_TOP);
						else DrawText(hdc, res_str[RES_STR_DISABLE], -1, &msg_temp, DT_TOP);
						
						msg_temp.left = msg_rcFilename.left+900;
						DrawText(hdc, "12:00", -1, &msg_temp, DT_TOP);	
					break;


					case RES_STR_SYSTEMTIME_ON2:
						SelectFont(hdc, logfont);
						msg_temp.left = msg_rcFilename.left+500;
						
						if(on_2) DrawText(hdc, res_str[RES_STR_ENABLE], -1, &msg_temp, DT_TOP);
						else DrawText(hdc, res_str[RES_STR_DISABLE], -1, &msg_temp, DT_TOP);
						
						msg_temp.left = msg_rcFilename.left+900;
						DrawText(hdc, "12:00", -1, &msg_temp, DT_TOP);							
					break;


					case RES_STR_SYSTEMTIME_ON3:
						SelectFont(hdc, logfont);
						msg_temp.left = msg_rcFilename.left+500;
						
						if(on_3) DrawText(hdc, res_str[RES_STR_ENABLE], -1, &msg_temp, DT_TOP);
						else DrawText(hdc, res_str[RES_STR_DISABLE], -1, &msg_temp, DT_TOP);
					
						msg_temp.left = msg_rcFilename.left+900;
						DrawText(hdc, "12:00", -1, &msg_temp, DT_TOP);								
					break;

					
					case RES_STR_SYSTEMTIME_OFF1:
						SelectFont(hdc, logfont);
						msg_temp.left = msg_rcFilename.left+500;
						
						if(off_1) DrawText(hdc, res_str[RES_STR_ENABLE], -1, &msg_temp, DT_TOP);
						else DrawText(hdc, res_str[RES_STR_DISABLE], -1, &msg_temp, DT_TOP);
					
						msg_temp.left = msg_rcFilename.left+900;
						DrawText(hdc, "12:00", -1, &msg_temp, DT_TOP);						
					break;

					
					case RES_STR_SYSTEMTIME_OFF2:
						SelectFont(hdc, logfont);
						msg_temp.left = msg_rcFilename.left+500;
						
						if(off_2) DrawText(hdc, res_str[RES_STR_ENABLE], -1, &msg_temp, DT_TOP);
						else DrawText(hdc, res_str[RES_STR_DISABLE], -1, &msg_temp, DT_TOP);
					

						msg_temp.left = msg_rcFilename.left+900;
						DrawText(hdc, "12:00", -1, &msg_temp, DT_TOP);						
					break;

					
					case RES_STR_SYSTEMTIME_OFF3:
						SelectFont(hdc, logfont);
						msg_temp.left = msg_rcFilename.left+500;
						
						if(off_3) DrawText(hdc, res_str[RES_STR_ENABLE], -1, &msg_temp, DT_TOP);
						else DrawText(hdc, res_str[RES_STR_DISABLE], -1, &msg_temp, DT_TOP);
					
						msg_temp.left = msg_rcFilename.left+900;
						DrawText(hdc, "12:00", -1, &msg_temp, DT_TOP);						
					break;
					
						
				}
			
        }

        if (page > 1) {
            for (i = 0; i < page; i++) {
                int x;
                if (page == 1)
                    x =  SETTING_PAGE_DOT_X;
                else if (page % 2)
           	        x =  SETTING_PAGE_DOT_X - page / 2 * SETTING_PAGE_DOT_SPAC;
                else
                    x =  SETTING_PAGE_DOT_X - page / 2 * SETTING_PAGE_DOT_SPAC + SETTING_PAGE_DOT_SPAC / 2;

                if (i == cur_page)
                    FillCircle(hdc, x + i * SETTING_PAGE_DOT_SPAC, SETTING_PAGE_DOT_Y, SETTING_PAGE_DOT_DIA);
                else
                    Circle(hdc, x + i * SETTING_PAGE_DOT_SPAC, SETTING_PAGE_DOT_Y, SETTING_PAGE_DOT_DIA);    
            }
        }
        SetBrushColor(hdc, old_brush);
        EndPaint(hWnd, hdc);
        break;
    }
    case MSG_KEYDOWN:
        //printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
        switch (wParam) {
            case KEY_EXIT_FUNC:
                EndDialog(hWnd, wParam);
                break;
            case KEY_DOWN_FUNC:
                if (list_sel < (SYSTEMTIME_NUM - 1))
                    list_sel++;
                else
                    list_sel = 0;
                InvalidateRect(hWnd, &msg_rcBg, TRUE);
                break;
            case KEY_UP_FUNC:
                 if (list_sel > 0)
                    list_sel--;
                else
                    list_sel = SYSTEMTIME_NUM - 1;
                InvalidateRect(hWnd, &msg_rcBg, TRUE);
                break;
            case KEY_ENTER_FUNC:
				

                break;
        }
        break;
    case MSG_COMMAND: {
        break;
    }
    case MSG_DESTROY:
        KillTimer(hWnd, _ID_TIMER_SETTING_SYSTEMTIME);
        unloadres();
        break;
    case MSG_LBUTTONDOWN:
        touch_pos_down.x = LOSWORD(lParam);
        touch_pos_down.y = HISWORD(lParam);
        printf("%s MSG_LBUTTONDOWN x %d, y %d\n", __func__,touch_pos_down.x,touch_pos_down.y);
        break;
    case MSG_LBUTTONUP:
        if (get_bl_brightness() == 0)
        {
            screenon();
            break;
        }
        DisableScreenAutoOff();
        touch_pos_up.x = LOSWORD(lParam);
        touch_pos_up.y = HISWORD(lParam);
        printf("%s MSG_LBUTTONUP x %d, y %d\n", __func__, touch_pos_up.x, touch_pos_up.y);
        int witch_button = check_button(touch_pos_up.x,touch_pos_up.y);
        if(witch_button == 0) menu_back(hWnd,wParam,lParam);
        if(witch_button > 0 && witch_button < SYSTEMTIME_WHOLE_BUTTON_NUM)
        {
            list_sel = witch_button - 1;
            switch (list_sel) {
					case 0:
						break;
					
					
					case 1:
						break;
						
						
					case 2:
						break;
						

					case 3:
						on_1 = !on_1;
						printf("on_1 = %d",on_1);
						printf("on_1 = %d",on_1);
						printf("on_1 = %d",on_1);
						break;

					
					case 4:
						off_1 = !off_1;
						break;

					
					case 5:
						on_2 = !on_2;
						break;

					
					case 6:
						off_2 = !off_2;
						break;

					
					case 7:						
						on_3 = !on_3;
						break;

					case 8:						
						off_3 = !off_3;
						break;
				}
			
            InvalidateRect(hWnd, &msg_rcBg, TRUE);
        }
        touch_pos_old.x = touch_pos_up.x;
        touch_pos_old.y = touch_pos_up.y;
        EnableScreenAutoOff();
        break;
    }

    return DefaultDialogProc(hWnd, message, wParam, lParam);
}

void creat_setting_systemtime_dialog(HWND hWnd)
{
    DLGTEMPLATE DesktopDlg = {WS_VISIBLE, WS_EX_NONE | WS_EX_AUTOSECONDARYDC,
    	                        0, 0,
    	                        LCD_W, LCD_H,
                              DESKTOP_DLG_STRING, 0, 0, 0, NULL, 0};
    //DesktopDlg.controls = DesktopCtrl;

    DialogBoxIndirectParam(&DesktopDlg, hWnd, setting_systemtime_dialog_proc, 0L);
}
