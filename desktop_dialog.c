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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"

#if 0
#define ICON_SPAC      90
#define GAME_PINT_X    25
#define GAME_PINT_Y    LCD_H - 96
#define GAME_PINT_W    76
#define GAME_PINT_H    96
#else
#define ICON_SPAC      90
#define GAME_PINT_X    25
#define GAME_PINT_Y    LCD_H - 80
#define GAME_PINT_W    64
#define GAME_PINT_H    64
#endif

#define MUSIC_PINT_X    GAME_PINT_X + ICON_SPAC
#define MUSIC_PINT_Y    GAME_PINT_Y
#define MUSIC_PINT_W    GAME_PINT_W
#define MUSIC_PINT_H    GAME_PINT_H

#define PHOTO_PINT_X    MUSIC_PINT_X + ICON_SPAC
#define PHOTO_PINT_Y    GAME_PINT_Y
#define PHOTO_PINT_W    GAME_PINT_W
#define PHOTO_PINT_H    GAME_PINT_H

#define FOLDE_PINT_X    PHOTO_PINT_X + ICON_SPAC
#define FOLDE_PINT_Y    GAME_PINT_Y
#define FOLDE_PINT_W    GAME_PINT_W
#define FOLDE_PINT_H    GAME_PINT_H

#define SETTING_PINT_X    FOLDE_PINT_X + ICON_SPAC
#define SETTING_PINT_Y    GAME_PINT_Y
#define SETTING_PINT_W    GAME_PINT_W
#define SETTING_PINT_H    GAME_PINT_H

#define GAME_ICON_PINT_X    50
#define GAME_ICON_PINT_Y    90
#define GAME_ICON_PINT_W    160
#define GAME_ICON_PINT_H    90
#define GAME_ICON_SPAC      220
#define GAME_ICON_ZOOM_W    (GAME_ICON_PINT_W / 5)
#define GAME_ICON_ZOOM_H    (GAME_ICON_PINT_H / 5)

static int menu_sel = 0;
static int game_sel = 0;
static int line_sel = 0;
#define MENU_ICON_NUM   2
#define GAME_ICON_NUM   1
#define GAME_NUM        6

static BITMAP menu_bmap[5][MENU_ICON_NUM];
static BITMAP game_bmap[GAME_NUM][GAME_ICON_NUM];

static RECT msg_rcGame = {GAME_PINT_X, GAME_PINT_Y, GAME_PINT_X + GAME_PINT_W, GAME_PINT_Y + GAME_PINT_H};
static RECT msg_rcMusic = {MUSIC_PINT_X, MUSIC_PINT_Y, MUSIC_PINT_X + MUSIC_PINT_W, MUSIC_PINT_Y + MUSIC_PINT_H};
static RECT msg_rcPhoto = {PHOTO_PINT_X, PHOTO_PINT_Y, PHOTO_PINT_X + PHOTO_PINT_W, PHOTO_PINT_Y + PHOTO_PINT_H};
static RECT msg_rcFolde = {FOLDE_PINT_X, FOLDE_PINT_Y, FOLDE_PINT_X + FOLDE_PINT_W, FOLDE_PINT_Y + FOLDE_PINT_H};
static RECT msg_rcSetting = {SETTING_PINT_X, SETTING_PINT_Y, SETTING_PINT_X + SETTING_PINT_W, SETTING_PINT_Y + SETTING_PINT_H};
static RECT msg_rcBg = {BG_PINT_X, BG_PINT_Y, BG_PINT_X + BG_PINT_W, BG_PINT_Y + BG_PINT_H};

static GAL_Rect msg_galrcMenu[] = {
	{GAME_PINT_X, GAME_PINT_Y, GAME_PINT_W, GAME_PINT_H},
	{MUSIC_PINT_X, MUSIC_PINT_Y, MUSIC_PINT_W, MUSIC_PINT_H},
	{PHOTO_PINT_X, PHOTO_PINT_Y, PHOTO_PINT_W, PHOTO_PINT_H},
	{FOLDE_PINT_X, FOLDE_PINT_Y, FOLDE_PINT_W, FOLDE_PINT_H},
	{SETTING_PINT_X, SETTING_PINT_Y, SETTING_PINT_W, SETTING_PINT_H}
};

static char game_name[GAME_NUM][64] = {
	"GAME1",
	"GAME2",
	"GAME3",
	"GAME4",
	"GAME5",
	"GAME6"
};

extern BITMAP background_bmap;
static int loadres(void)
{
    int i, j;
    char img[128];
    char respath[] = "/usr/local/share/minigui/res/images/";

    for (i = 0; i < MENU_ICON_NUM; i++) {
        /* load game bmp */
        snprintf(img, sizeof(img), "%sgame%d.png", respath, i);
        printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[0][i], img))
            return -1;

        snprintf(img, sizeof(img), "%smusic%d.png", respath, i);
        printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[1][i], img))
            return -1;

        snprintf(img, sizeof(img), "%sphoto%d.png", respath, i);
        printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[2][i], img))
            return -1;

        snprintf(img, sizeof(img), "%sfolde%d.png", respath, i);
        printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[3][i], img))
            return -1;  

        snprintf(img, sizeof(img), "%ssetting%d.png", respath, i);
        printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[4][i], img))
            return -1;
    }
    for (j = 0; j < GAME_NUM; j++) {
        for (i = 0; i < GAME_ICON_NUM; i++) {
            snprintf(img, sizeof(img), "%sgame_%d_%d.png", respath, j, i);
            printf("%s\n", img);
            if (LoadBitmap(HDC_SCREEN, &game_bmap[j][i], img))
                return -1;
        }
    }
    return 0;
}

static void unloadres(void)
{
    int i, j;

    for (j = 0; j < 5; j++)
        for (i = 0; i < MENU_ICON_NUM; i ++)
            UnloadBitmap(&menu_bmap[j][i]);

    for (j = 0; j < GAME_NUM; j++)
        for (i = 0; i < GAME_ICON_NUM; i ++)
            UnloadBitmap(&game_bmap[j][i]); 
}

static LRESULT desktop_dialog_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        SetTimer(hWnd, _ID_TIMER_1, 100);
        if (hFocus)
            SetFocus(hFocus);
        return 0;
    }
    case MSG_TIMER: {
        if (wParam == _ID_TIMER_1) {
            InvalidateRect(hWnd, &msg_rcBg, TRUE);
        }
        break;
    }
    case MSG_PAINT:
    {
        int i;
        int page;
        gal_pixel old_brush;
        gal_pixel pixle = 0xff000000;

        hdc = BeginPaint(hWnd);
        old_brush = SetBrushColor(hdc, pixle);
        FillBoxWithBitmap(hdc, BG_PINT_X,
                               BG_PINT_Y, BG_PINT_W,
                               BG_PINT_H, &background_bmap);
        for (i = 0; i < 4; i++) {
            RECT msg_rcName;

            if (((game_sel / 4) * 4 + i) >= GAME_NUM)
                break;
            if ((i == game_sel % 4) && (line_sel == 0))
                FillBoxWithBitmap(hdc, GAME_ICON_PINT_X + GAME_ICON_SPAC * i - (GAME_ICON_ZOOM_W / 2),
                                       GAME_ICON_PINT_Y - GAME_ICON_ZOOM_H, GAME_ICON_PINT_W + GAME_ICON_ZOOM_W,
                                       GAME_ICON_PINT_H + GAME_ICON_ZOOM_H, &game_bmap[(game_sel / 4) * 4 + i][0]);
            else
                FillBoxWithBitmap(hdc, GAME_ICON_PINT_X + GAME_ICON_SPAC * i,
                                       GAME_ICON_PINT_Y, GAME_ICON_PINT_W,
                                       GAME_ICON_PINT_H, &game_bmap[(game_sel / 4) * 4 + i][0]);
           msg_rcName.left = GAME_ICON_PINT_X + GAME_ICON_SPAC * i;
           msg_rcName.top = GAME_ICON_PINT_Y + GAME_ICON_PINT_H + 20;
           msg_rcName.right = msg_rcName.left + GAME_ICON_PINT_W;
           msg_rcName.bottom = msg_rcName.top + 24;
           SetBkColor(hdc, COLOR_transparent);
           SetBkMode(hdc,BM_TRANSPARENT);
           DrawText (hdc, game_name[(game_sel / 4) * 4 + i], -1, &msg_rcName, DT_TOP | DT_CENTER);
        }
        page = (GAME_NUM + 3) / 4;
            
        for (i = 0; i < page; i++) {
            int x;
            if (page == 1)
                x =  LCD_W / 2;
            else if (page % 2)
           	    x =  LCD_W / 2 - page / 2 * 40;
            else
                x =  LCD_W / 2 - page / 2 * 40 + 20;

            if (i == game_sel / 4)
                FillCircle(hdc, x + i * 40, GAME_ICON_PINT_Y + GAME_ICON_PINT_H + 90, 4);
            else
                Circle(hdc, x + i * 40, GAME_ICON_PINT_Y + GAME_ICON_PINT_H + 90, 4);    
        }
        for (i = 0; i < 5; i++) {
            if ((i == menu_sel) && (line_sel == 1))
                FillBoxWithBitmap(hdc, msg_galrcMenu[i].x,
                                  msg_galrcMenu[i].y, msg_galrcMenu[i].w,
                                  msg_galrcMenu[i].h, &menu_bmap[i][1]);
            else
                FillBoxWithBitmap(hdc, msg_galrcMenu[i].x,
                                  msg_galrcMenu[i].y, msg_galrcMenu[i].w,
                                  msg_galrcMenu[i].h, &menu_bmap[i][0]);
        }
        SetBrushColor(hdc, old_brush);
        EndPaint(hWnd, hdc);
        break;
    }
    case MSG_KEYDOWN:
        printf("%s message = 0x%x, 0x%x, 0x%x\n", __func__, message, wParam, lParam);
        switch (wParam) {
            case SCANCODE_MODE:
            case SCANCODE_MUTE:
                line_sel = line_sel?0:1;
                InvalidateRect(hWnd, &msg_rcBg, TRUE);
                break;
            case SCANCODE_VOLUP:
                if (line_sel) {
                    if (menu_sel < 4)
                        menu_sel++;
                    else
                        menu_sel = 0;
                } else {
                    if (game_sel < (GAME_NUM - 1))
                        game_sel++;
                    else
                        game_sel = 0;
                }
                InvalidateRect(hWnd, &msg_rcBg, TRUE);
                break;
            case SCANCODE_VOLDOWN:
                if (line_sel) {
                    if (menu_sel > 0)
                        menu_sel--;
                    else
                        menu_sel = 4;
                } else {
                    if (game_sel > 0)
                        game_sel--;
                    else
                        game_sel = GAME_NUM - 1;
                }
                InvalidateRect(hWnd, &msg_rcBg, TRUE);
                break;
            case 0xb:
                if (line_sel == 0) {
			char cmd[128];
			sprintf(cmd, "/data/start.sh %d", game_sel);
                	system_fd_closexec(cmd);
                	system_fd_closexec("killall retroarch weston");
		} else {
                    switch (menu_sel) {
                        case 0:
                            break;
                        case 1:
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                        case 4:
                            break;
                    }
                }
                break;
        }
        break;
    case MSG_COMMAND: {

        break;
    }
    case MSG_DESTROY:
        unloadres();
        break;
    }

    return DefaultDialogProc(hWnd, message, wParam, lParam);
}

void creat_desktop_dialog(HWND hWnd)
{
    DLGTEMPLATE DesktopDlg = {WS_VISIBLE, WS_EX_NONE | WS_EX_AUTOSECONDARYDC,
    	                        0, 0,
    	                        LCD_W, LCD_H,
                              DESKTOP_DLG_STRING, 0, 0, 0, NULL, 0};
    //DesktopDlg.controls = DesktopCtrl;
    DialogBoxIndirectParam(&DesktopDlg, HWND_DESKTOP, desktop_dialog_proc, 0L);
}
