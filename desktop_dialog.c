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

static int menu_sel = 0;
static int game_sel = 0;
static int line_sel = 0;
static int batt = 0;

#define MENU_NUM        6
#define MENU_ICON_NUM   2

#define GAME_NUM        6
#define GAME_ICON_NUM   1
#define GAME_ICON_NUM_PERPAGE  4

static BITMAP menu_bmap[MENU_NUM][MENU_ICON_NUM];
static BITMAP game_bmap[GAME_NUM][GAME_ICON_NUM];

static RECT msg_rcGame = {GAME_PINT_X, GAME_PINT_Y, GAME_PINT_X + GAME_PINT_W, GAME_PINT_Y + GAME_PINT_H};
static RECT msg_rcMusic = {MUSIC_PINT_X, MUSIC_PINT_Y, MUSIC_PINT_X + MUSIC_PINT_W, MUSIC_PINT_Y + MUSIC_PINT_H};
static RECT msg_rcPhoto = {PHOTO_PINT_X, PHOTO_PINT_Y, PHOTO_PINT_X + PHOTO_PINT_W, PHOTO_PINT_Y + PHOTO_PINT_H};
static RECT msg_rcFolde = {FOLDE_PINT_X, FOLDE_PINT_Y, FOLDE_PINT_X + FOLDE_PINT_W, FOLDE_PINT_Y + FOLDE_PINT_H};
static RECT msg_rcSetting = {SETTING_PINT_X, SETTING_PINT_Y, SETTING_PINT_X + SETTING_PINT_W, SETTING_PINT_Y + SETTING_PINT_H};

const GAL_Rect msg_galrcMenu[] = {
	{GAME_PINT_X, GAME_PINT_Y, GAME_PINT_W, GAME_PINT_H},
	{MUSIC_PINT_X, MUSIC_PINT_Y, MUSIC_PINT_W, MUSIC_PINT_H},
	{PHOTO_PINT_X, PHOTO_PINT_Y, PHOTO_PINT_W, PHOTO_PINT_H},
	{VIDEO_PINT_X, VIDEO_PINT_Y, VIDEO_PINT_W, VIDEO_PINT_H},
	{FOLDE_PINT_X, FOLDE_PINT_Y, FOLDE_PINT_W, FOLDE_PINT_H},
	{SETTING_PINT_X, SETTING_PINT_Y, SETTING_PINT_W, SETTING_PINT_H}
};

static const char game_name[GAME_NUM][64] = {
	"GAME1",
	"GAME2",
	"GAME3",
	"GAME4",
	"GAME5",
	"GAME6"
};
#define TITLE_GAME        "游戏"
#define TITLE_MUSIC       "音乐"
#define TITLE_PIC         "图片"
#define TITLE_VIDEO       "视频"
#define TITLE_BROWSER     "资源管理器"

static int loadres(void)
{
    int i, j;
    char img[128];
    char respath[] = UI_IMAGE_PATH;

    for (i = 0; i < MENU_ICON_NUM; i++) {
        /* load game bmp */
        snprintf(img, sizeof(img), "%sgame%d.png", respath, i);
        //printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[0][i], img))
            return -1;

        snprintf(img, sizeof(img), "%smusic%d.png", respath, i);
        //printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[1][i], img))
            return -1;

        snprintf(img, sizeof(img), "%sphoto%d.png", respath, i);
        //printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[2][i], img))
            return -1;

        snprintf(img, sizeof(img), "%svideo%d.png", respath, i);
        //printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[3][i], img))
            return -1;  

        snprintf(img, sizeof(img), "%sfolde%d.png", respath, i);
        //printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[4][i], img))
            return -1;  

        snprintf(img, sizeof(img), "%ssetting%d.png", respath, i);
        //printf("%s\n", img);
        if (LoadBitmap(HDC_SCREEN, &menu_bmap[5][i], img))
            return -1;
    }
    for (j = 0; j < GAME_NUM; j++) {
        for (i = 0; i < GAME_ICON_NUM; i++) {
            snprintf(img, sizeof(img), "%sgame_%d_%d.png", respath, j, i);
            //printf("%s\n", img);
            if (LoadBitmap(HDC_SCREEN, &game_bmap[j][i], img))
                return -1;
        }
    }

    return 0;
}

static void unloadres(void)
{
    int i, j;

    for (j = 0; j < MENU_NUM; j++)
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
        SetTimer(hWnd, _ID_TIMER_DESKTOP, 100);
        if (hFocus)
            SetFocus(hFocus);
        return 0;
    }
    case MSG_TIMER: {
        if (wParam == _ID_TIMER_DESKTOP) {
            if (batt != battery) {
                batt = battery;
                InvalidateRect(hWnd, &msg_rcBatt, TRUE);
            }
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
        FillBoxWithBitmap(hdc, BATT_PINT_X, BATT_PINT_Y,
                               BATT_PINT_W, BATT_PINT_H,
                               &batt_bmap[batt]);
        for (i = 0; i < GAME_ICON_NUM_PERPAGE; i++) {
            RECT msg_rcName;

            if (((game_sel / GAME_ICON_NUM_PERPAGE) * GAME_ICON_NUM_PERPAGE + i) >= GAME_NUM)
                break;
            if ((i == game_sel % GAME_ICON_NUM_PERPAGE) && (line_sel == 0))
                FillBoxWithBitmap(hdc, GAME_ICON_PINT_X + GAME_ICON_SPAC * i - (GAME_ICON_ZOOM_W / 2),
                                       GAME_ICON_PINT_Y - GAME_ICON_ZOOM_H, GAME_ICON_PINT_W + GAME_ICON_ZOOM_W,
                                       GAME_ICON_PINT_H + GAME_ICON_ZOOM_H, &game_bmap[(game_sel / GAME_ICON_NUM_PERPAGE) * GAME_ICON_NUM_PERPAGE + i][0]);
            else
                FillBoxWithBitmap(hdc, GAME_ICON_PINT_X + GAME_ICON_SPAC * i,
                                       GAME_ICON_PINT_Y, GAME_ICON_PINT_W,
                                       GAME_ICON_PINT_H, &game_bmap[(game_sel / GAME_ICON_NUM_PERPAGE) * GAME_ICON_NUM_PERPAGE + i][0]);
           msg_rcName.left = GAME_ICON_PINT_X + GAME_ICON_SPAC * i;
           msg_rcName.top = GAME_ICON_PINT_Y + GAME_ICON_PINT_H + 20;
           msg_rcName.right = msg_rcName.left + GAME_ICON_PINT_W;
           msg_rcName.bottom = msg_rcName.top + 24;
           SetBkColor(hdc, COLOR_transparent);
           SetBkMode(hdc,BM_TRANSPARENT);
           SetTextColor(hdc, RGB2Pixel(hdc, 0xff, 0xff, 0xff));
           DrawText (hdc, game_name[(game_sel / GAME_ICON_NUM_PERPAGE) * GAME_ICON_NUM_PERPAGE + i], -1, &msg_rcName, DT_TOP | DT_CENTER);
        }
        page = (GAME_NUM + GAME_ICON_NUM_PERPAGE - 1) / GAME_ICON_NUM_PERPAGE;

        for (i = 0; i < page; i++) {
            int x;
            if (page == 1)
                x =  DESKTOP_PAGE_DOT_X;
            else if (page % 2)
           	    x =  DESKTOP_PAGE_DOT_X - page / 2 * DESKTOP_PAGE_DOT_SPAC;
            else
                x =  DESKTOP_PAGE_DOT_X - page / 2 * DESKTOP_PAGE_DOT_SPAC + DESKTOP_PAGE_DOT_SPAC / 2;

            if (i == game_sel / GAME_ICON_NUM_PERPAGE)
                FillCircle(hdc, x + i * DESKTOP_PAGE_DOT_SPAC, DESKTOP_PAGE_DOT_Y, DESKTOP_PAGE_DOT_DIA);
            else
                Circle(hdc, x + i * DESKTOP_PAGE_DOT_SPAC, DESKTOP_PAGE_DOT_Y, DESKTOP_PAGE_DOT_DIA);    
        }
        for (i = 0; i < MENU_NUM; i++) {
            if ((i == menu_sel) && (line_sel == 1))
                FillBoxWithBitmap(hdc, msg_galrcMenu[i].x,
                                  msg_galrcMenu[i].y, msg_galrcMenu[i].w,
                                  msg_galrcMenu[i].h, &menu_bmap[i][1]);
            else
                FillBoxWithBitmap(hdc, msg_galrcMenu[i].x + MENU_ICON_ZOOM_W / 2,
                                  msg_galrcMenu[i].y + MENU_ICON_ZOOM_H, msg_galrcMenu[i].w - MENU_ICON_ZOOM_W,
                                  msg_galrcMenu[i].h - MENU_ICON_ZOOM_H, &menu_bmap[i][0]);
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
            case SCANCODE_CURSORBLOCKDOWN:
            case SCANCODE_CURSORBLOCKUP:
                line_sel = line_sel ? 0 : 1;
                batt = battery;
                InvalidateRect(hWnd, &msg_rcBg, TRUE);
                break;
            case SCANCODE_VOLUP:
            case SCANCODE_CURSORBLOCKRIGHT:
                if (line_sel) {
                    if (menu_sel < MENU_NUM - 1)
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
            case SCANCODE_CURSORBLOCKLEFT:
                if (line_sel) {
                    if (menu_sel > 0)
                        menu_sel--;
                    else
                        menu_sel = MENU_NUM - 1;
                } else {
                    if (game_sel > 0)
                        game_sel--;
                    else
                        game_sel = GAME_NUM - 1;
                }
                InvalidateRect(hWnd, &msg_rcBg, TRUE);
                break;
            case SCANCODE_PLAY:
            case SCANCODE_ENTER:
                if (line_sel == 0) {
                    char cmd[128];
                    sprintf(cmd, "/data/start.sh %d", game_sel);
                    system_fd_closexec(cmd);
                    system_fd_closexec("killall retroarch weston");
                } else {
                    switch (menu_sel) {
                        case 0:
                            creat_browser_dialog(hWnd, FILTER_FILE_GAME, TITLE_GAME);
                            break;
                        case 1:
                            creat_browser_dialog(hWnd, FILTER_FILE_MUSIC, TITLE_MUSIC);
                            break;
                        case 2:
                            creat_browser_dialog(hWnd, FILTER_FILE_PIC, TITLE_PIC);
                            break;
                        case 3:
                            creat_browser_dialog(hWnd, FILTER_FILE_VIDEO, TITLE_VIDEO);
                            break;
                        case 4:
                            creat_browser_dialog(hWnd, FILTER_FILE_NO, TITLE_BROWSER);
                            break;
                        case 5:
                            creat_setting_dialog(hWnd);
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
