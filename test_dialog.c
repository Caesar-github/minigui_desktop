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

//编码
#include <libavcodec/avcodec.h>
//封装格式处理
#include <libavformat/avformat.h>
//像素处理
#include <libswscale/swscale.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"

static BITMAP mBitMap;
static unsigned char rgb_buff[LCD_W*LCD_H*4];
static HWND hMainWnd;
static int file_total;
static int list_select = 0;
static struct directory_node *dir_node = 0;

static int loadres(void)
{
    return 0;
}

static void unloadres(void)
{

}

static LRESULT test_dialog_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    //printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
    switch (message) {
    case MSG_INITDIALOG: {
    	  DWORD bkcolor;
    	  HDC sndHdc;
        HWND hFocus = GetDlgDefPushButton(hWnd);
        bkcolor = GetWindowElementPixel(hWnd, WE_BGC_WINDOW);
        SetWindowBkColor(hWnd, bkcolor);
        if (hFocus)
            SetFocus(hFocus);
        loadres();
        memset(rgb_buff, 0x00, LCD_W * LCD_H*4);
        //SetTimer(hWnd, _ID_TIMER_VIDEOPLAY, 10);
        hMainWnd = hWnd;
        sndHdc = GetSecondaryDC((HWND)hWnd);
        SetMemDCAlpha(sndHdc, MEMDC_FLAG_SWSURFACE, 0);
        InvalidateRect(hWnd, &msg_rcDialog, TRUE);
        return 0;
    }
    case MSG_TIMER:
        if (wParam == _ID_TIMER_VIDEOPLAY) {
            //InvalidateRect(hWnd, &msg_rcDialog, TRUE);
        }
        break;
    case MSG_KEYDOWN:
        switch (wParam) {
            case KEY_UP_FUNC:
                break;
            case KEY_DOWN_FUNC:
                break;
            case KEY_ENTER_FUNC:
                break;
            case KEY_EXIT_FUNC:
                break;
        }
        break;
    case MSG_COMMAND: {
        break;
    }
    case MSG_PAINT: {
        int i;
        hdc = BeginPaint(hWnd);
        InitBitmap(HDC_SCREEN, LCD_W, LCD_H, LCD_W*4, rgb_buff, &mBitMap);
        FillBoxWithBitmap(hdc, 0, 0, LCD_W, LCD_H, &mBitMap);
        EndPaint(hWnd, hdc);
        break;
    }
    case MSG_VIDEOPLAY_END:
        EndDialog(hWnd, wParam);
        break;
    case MSG_CLOSE:
        unloadres();
        return 0;
    }

    return DefaultDialogProc(hWnd, message, wParam, lParam);
}

void creat_test_dialog(HWND hWnd, struct directory_node *node)
{
    DLGTEMPLATE PicPreViewDlg = {WS_VISIBLE, WS_EX_NONE | WS_EX_AUTOSECONDARYDC,
    	                        0, 0,
    	                        LCD_W, LCD_H,
                              DESKTOP_DLG_STRING, 0, 0, 0, NULL, 0};

    DialogBoxIndirectParam(&PicPreViewDlg, hWnd, test_dialog_proc, 0L);
}