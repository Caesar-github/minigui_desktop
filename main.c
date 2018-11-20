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
extern WINDOW_ELEMENT_RENDERER * __mg_def_renderer;
extern MG_EXPORT PLOGFONT g_SysLogFont [];

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
//need save
int language = 0;
int screenoff_val = 0;
int eq_val = 0;
int backlight_val = 0;
int gamedisp_val = 0;
//
char *res_str[RES_STR_MAX] = {0};
LOGFONT  *logfont_cej;
LOGFONT  *logfont_k;
LOGFONT  *logfont;

#define maxlabelsize 35
static int  __getline(char **lineptr, ssize_t *n, FILE *stream)
{
    int count=0;
    int buf;

    if(*lineptr == NULL) {
        *n=maxlabelsize;
        *lineptr = (char*)malloc(*n);
    }

    if(( buf = fgetc(stream) ) == EOF ) {
        return -1;
    }

    do {
        if(buf=='\n') {    
            count += 1;
            break;
        }

        count++;

        *(*lineptr+count-1) = buf;
        *(*lineptr+count) = '\0';

        if(*n <= count)
            *lineptr = (char*)realloc(*lineptr,count*2);
        buf = fgetc(stream);
    } while( buf != EOF);

    return count;
}

void updatesysfont(LOGFONT  *font)
{
    int i;

    for (i = 0; i <= WE_DESKTOP; i++) {
        __mg_def_renderer->we_fonts[i] = font;
    }
    for (i = 0; i < NR_SYSLOGFONTS; i++) {
        g_SysLogFont[i] = font;
    }
}

int loadversion(char **model, char **version)
{
    FILE *fp;
    ssize_t len = 0;
    int pos = 0;
    const char* versionFile;

    versionFile = VERSION_FILE;

    fp = fopen(versionFile, "r");
    if (fp == NULL) {
        printf("open file %s failed: %s\n", versionFile, strerror(errno));
        return -1;
    }

    //fgetc(fp);
    //fgetc(fp);
    //fgetc(fp);

    if ((__getline(model, &len, fp)) == -1)
        return -1;

    if ((__getline(version, &len, fp)) == -1)
        return -1;

    fclose(fp);

    return 0;	
}

int loadstringres(void)
{
    FILE *fp;
    ssize_t len = 0;
    int pos = 0;
    const char* langFile;
    
    switch (language) {
        case LANGUAGE_CH:
            langFile = REC_FILE_CN;
            logfont = logfont_cej;
            break;
        case LANGUAGE_EN:
            langFile = REC_FILE_EN;
            logfont = logfont_cej;
            break;
        case LANGUAGE_JA:
            langFile = REC_FILE_JA;
            logfont = logfont_cej;
            break;
        case LANGUAGE_KO:
            langFile = REC_FILE_KO;
            logfont = logfont_k;
            break;
    }
    fp = fopen(langFile, "r");
    if (fp == NULL) {
        printf("open file %s failed: %s\n", langFile, strerror(errno));
        return -1;
    }

    fgetc(fp);
    fgetc(fp);
    fgetc(fp);

    while ((__getline(&res_str[pos], &len, fp)) != -1) {
        //printf("load line Label %d------%s\n", pos, res_str[pos]);
        pos++;
        if (pos >= RES_STR_MAX)
            break;
    }

    fclose(fp);
    updatesysfont(logfont);
}

void unloadstringres(void)
{
    int i;

    for (i = 0; i < RES_STR_MAX; i++) {
        if (res_str[i]) {
            free(res_str[i]);
            res_str[i] = 0;
        }
    }
}

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
        	 logfont_cej = CreateLogFont("ttf", "msyh", "UTF-8", 'k', 'r', 'n', 'c', 'n', 'n', TTF_FONT_SIZE, 0);
        	 logfont_k = CreateLogFont("ttf", "msn", "UTF-8", 'k', 'r', 'n', 'c', 'n', 'n', TTF_FONT_SIZE, 0);
           loadstringres();
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
            unloadstringres();
            DestroyLogFont(logfont_cej);
            DestroyLogFont(logfont_k);
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


