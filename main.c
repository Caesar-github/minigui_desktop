/*
 * This is a every simple sample for MiniGUI.
 * It will create a main window and display a string of "Hello, world!" in it.
 */
#include <signal.h>
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
#include "hardware.h"

char timebuff[100];
static RECT msg_rcTime = {TIME_PINT_X, TIME_PINT_Y, TIME_PINT_X + TIME_PINT_W, TIME_PINT_Y + TIME_PINT_H};
RECT msg_rcBatt = {BATT_PINT_X, BATT_PINT_Y, BATT_PINT_X + BATT_PINT_W, BATT_PINT_Y + BATT_PINT_H};
RECT msg_rcBg = {BG_PINT_X, BG_PINT_Y, BG_PINT_X + BG_PINT_W, BG_PINT_Y + BG_PINT_H};
RECT msg_rcTitle = {TITLE_PINT_X, TITLE_PINT_Y, TITLE_PINT_X + TITLE_PINT_W, TITLE_PINT_Y + TITLE_PINT_H};
RECT msg_rcDialog = {0, 0, LCD_W, LCD_H};

BITMAP batt_bmap[6];
BITMAP background_bmap;
int battery = 0;

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
    char respath[] = UI_IMAGE_PATH;

    for (i = 0; i < 6; i++) {
        snprintf(img, sizeof(img), "%sbattery%d.png", respath, i);
        if (LoadBitmap(HDC_SCREEN, &batt_bmap[i], img))
            return -1;
    }

    snprintf(img, sizeof(img), "%sbackground.jpg", respath);
    if (LoadBitmap(HDC_SCREEN, &background_bmap, img))
        return -1;

    return 0;
}

static void unloadres(void)
{
    int i;
    
    for (i = 0; i < 6; i++)
      UnloadBitmap(&batt_bmap[i]);

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
               if (battery < 5)
                   battery++;
               else
                   battery = 0;
           }
        break;
        case MSG_PAINT:
            hdc = BeginPaint(hWnd);

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
    pCreateInfo->rx = LCD_W;
    pCreateInfo->by = LCD_H;
    pCreateInfo->iBkColor = PIXEL_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void signal_func(int signal)
{
    switch (signal){
        case SIGUSR1:
            printf("ac online:%s, battery capacity:%d%%\n",
                   ac_is_online()? "yes": "no",
                   get_battery_capacity());
            break;
        default:
            break;
    }
}

int MiniGUIMain(int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;
    struct sigaction sa;

#ifdef _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, arg[0], 0, 0);
#endif

    InitCreateInfo (&CreateInfo);

    sa.sa_sigaction = NULL;
    sa.sa_handler   = signal_func;
    sa.sa_flags     = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    while (GetMessage (&Msg, hMainWnd)) {
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}


