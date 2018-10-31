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
#include "desktop_dialog.h"

char timebuff[100];
static RECT msg_rcTime = {TIME_PINT_X, TIME_PINT_Y, TIME_PINT_X + TIME_PINT_W, TIME_PINT_Y + TIME_PINT_H};
static RECT msg_rcBatt = {BATT_PINT_X, BATT_PINT_Y, BATT_PINT_X + BATT_PINT_W, BATT_PINT_Y + BATT_PINT_H};
static RECT msg_rcBg = {BG_PINT_X, BG_PINT_Y, BG_PINT_X + BG_PINT_W, BG_PINT_Y + BG_PINT_H};

static BITMAP batt_bmap;
BITMAP background_bmap;

static char* mk_time(char* buff)
{
     time_t t;
     struct tm * tm;
 
     time (&t);
     tm = localtime (&t);
     sprintf (buff, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
     
     return buff;
}

void sysUsecTime(char* buff) 
{ 
    struct timeval    tv; 
    struct timezone tz; 
       
    struct tm         *p; 
       
    gettimeofday(&tv, &tz); 

    p = localtime(&tv.tv_sec); 
    sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d %03d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec / 1000); 
}

static int loadres(void)
{
    int i;
    char img[128];
    char respath[] = "/usr/local/share/minigui/res/images/";

    char batt_img[] = "batt.bmp";
    char background_img[] = "background.jpg";

    /* load batt bmp */
    snprintf(img, sizeof(img), "%s%s", respath, batt_img);
    if (LoadBitmap(HDC_SCREEN, &batt_bmap, img))
        return -1;

    /* load bt bmp */
    snprintf(img, sizeof(img), "%s%s", respath, background_img);
    if (LoadBitmap(HDC_SCREEN, &background_bmap, img))
        return -1;

    return 0;
}

static void unloadres(void)
{
    /* unload batt bmp */
    UnloadBitmap(&batt_bmap);

    /* unload bt bmp */
    UnloadBitmap(&background_bmap);
}

static LRESULT MainWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (message) {
        case MSG_CREATE:
        	 loadres();
           SetTimer(hWnd, _ID_TIMER, 100);
           InvalidateRect(hWnd, &msg_rcBg, TRUE);
           creat_desktop_dialog(hWnd);
        break;
        case MSG_TIMER:
           if (wParam == _ID_TIMER) {
               //InvalidateRect(hWnd, &msg_rcTime, FALSE);
               //InvalidateRect(hWnd, &msg_rcBatt, FALSE);
               //InvalidateRect(hWnd, &msg_rcBt, FALSE);
               //InvalidateRect(hWnd, &msg_rcWifi, FALSE);
           }
        break;
        case MSG_PAINT:
            hdc = BeginPaint(hWnd);
            /*
            mk_time(timebuff);
            TextOut(hdc, TIME_PINT_X, TIME_PINT_Y, timebuff);
            FillBoxWithBitmap(hdc, BATT_PINT_X,
                              BATT_PINT_Y, BATT_PINT_W,
                              BATT_PINT_H, &batt_bmap);
            */
            FillBoxWithBitmap(hdc, BG_PINT_X,
                              BG_PINT_Y, BG_PINT_W,
                              BG_PINT_H, &background_bmap);
            EndPaint(hWnd, hdc);
        break;
        case MSG_KEYDOWN:
            printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
            break;
        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            PostQuitMessage(hWnd);
            unloadres();
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_AUTOSECONDARYDC;
    pCreateInfo->spCaption = "Main" ;
    pCreateInfo->hMenu = 0;
    //pCreateInfo->hCursor = GetSystemCursor (0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = MainWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0; 
    pCreateInfo->rx = 480;
    pCreateInfo->by = 320;
    pCreateInfo->iBkColor = PIXEL_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain(int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, arg[0], 0, 0);
#endif

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    while (GetMessage (&Msg, hMainWnd)) {
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}


