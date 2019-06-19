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
#define WHOLE_BUTTON_NUM 1

static int batt = 0;
static int autopoweroff_cnt = 0;
static int runlowpower = 0;

static touch_pos touch_pos_down,touch_pos_up,touch_pos_old;

static int check_button(int x,int y)
{
    if ((x <= BACK_PINT_X + BACK_PINT_W ) &&
        (x >= BACK_PINT_X) &&
        (y <= BACK_PINT_Y + BACK_PINT_H ) &&
        (y >= BACK_PINT_Y))
        return 0;
    return -1;
}

static int loadres(void)
{
    return 0;
}

static void unloadres(void)
{
}

static void poweroff(void)
{
    printf("%s\n", __func__);
    system("halt");
}

static void lowpower_enter(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
    //todo
}

static void menu_back(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
    EndDialog(hWnd, wParam);
}

static LRESULT dialog_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    //printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
    switch (message)
    {
        case MSG_INITDIALOG:
        {
            DWORD bkcolor;
            HWND hFocus = GetDlgDefPushButton(hWnd);
            loadres();
            bkcolor = GetWindowElementPixel(hWnd, WE_BGC_WINDOW);
            SetWindowBkColor(hWnd, bkcolor);
            if (hFocus)
                SetFocus(hFocus);
            batt = battery;
            autopoweroff_cnt = 0;
            SetTimer(hWnd, _ID_TIMER_LOWPOWER, TIMER_LOWPOWER);
            return 0;
        }
        case MSG_TIMER:
        {
            static int dialog_last_time = 60;
            if (now_time->tm_min != dialog_last_time)
            {
                dialog_last_time = now_time->tm_min;
                InvalidateRect(hWnd, &msg_rcBg, FALSE);
            }
            if (wParam == _ID_TIMER_LOWPOWER)
            {
#ifdef ENABLE_BATT
                if (batt != battery)
                {
                    batt = battery;
                    InvalidateRect(hWnd, &msg_rcBatt, TRUE);
                }
#endif
                if (autopoweroff_cnt < 4)
                {
                    autopoweroff_cnt ++;
                    printf("autopoweroff_cnt = %d\n", autopoweroff_cnt);
                    if (autopoweroff_cnt >= 4)
                        poweroff();
                }
            }
            break;
        }
        case MSG_PAINT:
        {
            int i;
            int page;
            struct file_node *file_node_temp;
            gal_pixel old_brush;
            RECT msg_rcInfo;
            gal_pixel pixle = 0xffffffff;

            hdc = BeginPaint(hWnd);
            old_brush = SetBrushColor(hdc, pixle);
            FillBoxWithBitmap(hdc, BG_PINT_X,
                                   BG_PINT_Y, BG_PINT_W,
                                   BG_PINT_H, &background_bmap);
#ifdef ENABLE_BATT
            FillBoxWithBitmap(hdc, BATT_PINT_X - status_bar_offset, BATT_PINT_Y,
                                   BATT_PINT_W, BATT_PINT_H,
                                   &batt_bmap[batt]);
#endif
#ifdef ENABLE_WIFI
            if(get_wifi()==RK_WIFI_State_IDLE)
            {
                FillBoxWithBitmap(hdc, WIFI_PINT_X - status_bar_offset, WIFI_PINT_Y,
                                       WIFI_PINT_W, WIFI_PINT_H,
                                       &wifi_disabled_bmap);
                }
            else if(get_wifi()==RK_WIFI_State_CONNECTED)
            {
                            FillBoxWithBitmap(hdc, WIFI_PINT_X, WIFI_PINT_Y,
                                       WIFI_PINT_W, WIFI_PINT_H,
                                       &wifi_connected_bmap);
            }
            else
            {
                FillBoxWithBitmap(hdc, WIFI_PINT_X, WIFI_PINT_Y,
                                         WIFI_PINT_W, WIFI_PINT_H,
                                        &wifi_disconnected_bmap);
            }
#endif
            //==================display volume icon============================
            BITMAP *volume_display;
            if(get_volume()==0) volume_display=&volume_0;
            else if ( get_volume()>0  && get_volume()<=32)    volume_display=&volume_1;
            else if ( get_volume()>32  && get_volume()<=66)  volume_display=&volume_2;
            else volume_display=&volume_3;
            FillBoxWithBitmap(hdc, VOLUME_PINT_X, VOLUME_PINT_Y,
                                   VOLUME_PINT_W, VOLUME_PINT_H,
                                           volume_display);

            RECT msg_rcTime;
            time_flush();
            msg_rcTime.left = REALTIME_PINT_X - status_bar_offset;
            msg_rcTime.top = REALTIME_PINT_Y;
            msg_rcTime.right = REALTIME_PINT_X + REALTIME_PINT_W;
            msg_rcTime.bottom = REALTIME_PINT_Y + REALTIME_PINT_H;
            SetBkColor(hdc, COLOR_transparent);
            SetBkMode(hdc,BM_TRANSPARENT);
            SetTextColor(hdc, RGB2Pixel(hdc, 0xff, 0xff, 0xff));
            SelectFont(hdc, logfont_title);
            DrawText(hdc, status_bar_time_str, -1, &msg_rcTime, DT_TOP);
            msg_rcTime.left = REALDATE_PINT_X - status_bar_offset;
            DrawText(hdc, status_bar_date_str, -1, &msg_rcTime, DT_TOP);

            SetBkColor(hdc, COLOR_transparent);
            SetBkMode(hdc,BM_TRANSPARENT);
            SetTextColor(hdc, RGB2Pixel(hdc, 0xff, 0, 0));
            SelectFont(hdc, logfont);
            msg_rcInfo.left = 0;
            msg_rcInfo.top = LCD_H / 2;
            msg_rcInfo.right = LCD_W;
            msg_rcInfo.bottom = msg_rcInfo.top + 24;
            DrawText(hdc, res_str[RES_STR_LOWPOWER], -1, &msg_rcInfo, DT_TOP | DT_CENTER);

            SetBrushColor(hdc, old_brush);
            EndPaint(hWnd, hdc);
            break;
        }
        case MSG_KEYDOWN:
            //printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
            switch (wParam)
            {
                case KEY_EXIT_FUNC:
                    EndDialog(hWnd, wParam);
                    break;
                case KEY_UP_FUNC:
                    break;
                case KEY_DOWN_FUNC:
                    break;
                case KEY_ENTER_FUNC:
                    break;
            }
            break;
        case MSG_COMMAND: {
            break;
        }
        case MSG_DESTROY:
            KillTimer(hWnd, _ID_TIMER_LOWPOWER);
            unloadres();
            break;
        case MSG_LBUTTONDOWN:
            touch_pos_down.x = LOSWORD(lParam);
            touch_pos_down.y = HISWORD(lParam);
            printf("%s MSG_LBUTTONDOWN x %d, y %d\n", __func__,touch_pos_down.x,touch_pos_down.y);
            break;
        case MSG_LBUTTONUP:
        {
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
            if (witch_button == 0) menu_back(hWnd,wParam,lParam);
            if (witch_button > 0 && witch_button < WHOLE_BUTTON_NUM)
            {
                lowpower_enter(hWnd,wParam,witch_button);
            }
            touch_pos_old.x = touch_pos_up.x;
            touch_pos_old.y = touch_pos_up.y;
            EnableScreenAutoOff();
            break;
        }
    }

    return DefaultDialogProc(hWnd, message, wParam, lParam);
}

void creat_lowpower_dialog(HWND hWnd)
{
    DLGTEMPLATE DesktopDlg = {WS_VISIBLE, WS_EX_NONE | WS_EX_AUTOSECONDARYDC,
                                0, 0,
                                LCD_W, LCD_H,
                              DESKTOP_DLG_STRING, 0, 0, 0, NULL, 0};
    //DesktopDlg.controls = DesktopCtrl;
    if (runlowpower)
        return;
    runlowpower = 1;
    DialogBoxIndirectParam(&DesktopDlg, hWnd, dialog_proc, 0L);
}
