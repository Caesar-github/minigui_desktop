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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"

static RECT msg_rcDialog = {0, 0, 480, 320};

#define PIC_NUM   9
static int list_select = 0;
static BITMAP pic_bmap[PIC_NUM];
static int move_mode = 0;
static int move_cnt = 0;
static int move_pic_pre = 0;
static int move_pic_cur = 0;
static int move_auto = 1;
static int move_auto_cnt = 0;

static int loadres(void)
{
    int i;
    char img[128];
    char respath[] = "/usr/local/share/minigui/res/images/";

    for (i = 0; i < PIC_NUM; i++) {
        /* load pic bmp */
        printf("%s i = %d\n", __func__, i);
        snprintf(img, sizeof(img), "%spic%d.jpg", respath, i);
        printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &pic_bmap[i], img))
            return -1;
    }
    return 0;
}

static void unloadres(void)
{
    int i;
    for (i = 0; i < PIC_NUM; i++) {
        /* unload pic bmp */
        UnloadBitmap(&pic_bmap[i]);
    }
}

static LRESULT picpreview_dialog_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    //printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
    switch (message) {
    case MSG_INITDIALOG: {
    	  DWORD bkcolor;
        HWND hFocus = GetDlgDefPushButton(hWnd);
        bkcolor = GetWindowElementPixel(hWnd, WE_BGC_WINDOW);
        SetWindowBkColor(hWnd, bkcolor);
        if (hFocus)
            SetFocus(hFocus);
        loadres();
        SetTimer(hWnd, _ID_TIMER_1, 5);
        return 0;
    }
    case MSG_TIMER:
        if (wParam == _ID_TIMER_1) {
            if (move_mode != 0) {
                //printf("%s MSG_TIMER\n", __func__);
                move_cnt ++;
                if (move_cnt >= 10) {
                    move_mode = 0;
                    move_cnt = 0;
                }
                InvalidateRect(hWnd, &msg_rcDialog, TRUE);
            } else {
                if (move_auto) {
               	    move_auto_cnt++;
               	    if (move_auto_cnt > 20) {
               	        move_auto_cnt = 0;
                        move_pic_pre = list_select;
                        if (list_select < (sizeof(pic_bmap) / sizeof(pic_bmap[0]) - 1))
                            list_select++;
                        else
                            list_select = 0;
                        move_pic_cur = list_select;
                        InvalidateRect(hWnd, &msg_rcDialog, TRUE);
                        move_mode = 2;
               	    }
                }
            }
        }
        break;
    case MSG_KEYDOWN:
        switch (wParam) {
            case SCANCODE_VOLUP:
                if (move_mode != 0)
                    break;
                move_pic_pre = list_select;
                if (list_select == 0)
                    list_select = sizeof(pic_bmap) / sizeof(pic_bmap[0]) - 1;
                else
                    list_select--;
                move_pic_cur = list_select;
                InvalidateRect(hWnd, &msg_rcDialog, TRUE);
                move_mode = 1;
                break;
            case SCANCODE_VOLDOWN:
                if (move_mode != 0)
                    break;
                move_pic_pre = list_select;
                if (list_select < (sizeof(pic_bmap) / sizeof(pic_bmap[0]) - 1))
                    list_select++;
                else
                    list_select = 0;
                move_pic_cur = list_select;
                InvalidateRect(hWnd, &msg_rcDialog, TRUE);
                move_mode = 2;
                break;
            case SCANCODE_PLAY:
                EndDialog(hWnd, wParam);
                creat_desktop_dialog(hWnd);
                break;
        }
        break;
    case MSG_COMMAND: {
        break;
    }
    case MSG_PAINT: {
        int i;
        hdc = BeginPaint(hWnd);
        if (move_mode == 1) {
            FillBoxWithBitmap(hdc, 480 - (480 * move_cnt / 10), 0, 480, 320, &pic_bmap[move_pic_cur]);
            FillBoxWithBitmap(hdc, -(480 * move_cnt / 10), 0, 480, 320, &pic_bmap[move_pic_pre]);
        } else if (move_mode == 2) {
            FillBoxWithBitmap(hdc, (480 * move_cnt / 10), 0, 480, 320, &pic_bmap[move_pic_pre]);
            FillBoxWithBitmap(hdc, (480 * move_cnt / 10) - 480, 0, 480, 320, &pic_bmap[move_pic_cur]);
        } else {
            FillBoxWithBitmap(hdc, 0, 0, 480, 320, &pic_bmap[move_pic_cur]);
        }
        EndPaint(hWnd, hdc);
        break;
    }
    case MSG_CLOSE:
        unloadres();
        return 0;
    }

    return DefaultDialogProc(hWnd, message, wParam, lParam);
}

void creat_picpreview_dialog(HWND hWnd)
{
    DLGTEMPLATE PicPreViewDlg = {WS_VISIBLE, WS_EX_NONE | WS_EX_AUTOSECONDARYDC,
    	                        0, 0,
    	                        480, 320,
                              DESKTOP_DLG_STRING, 0, 0, 0, NULL, 0};
    list_select = 0;
    DialogBoxIndirectParam(&PicPreViewDlg, HWND_DESKTOP, picpreview_dialog_proc, 0L);
}