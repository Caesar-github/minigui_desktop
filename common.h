/*
 * Rockchip App
 *
 * Copyright (C) 2017 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#define _ID_TIMER                       100
#define _ID_TIMER_DESKTOP               101
#define _ID_TIMER_AUDIOPLAY             102
#define _ID_TIMER_BROWSER               103
#define _ID_TIMER_PICPREVIEW            104
#define _ID_TIMER_VIDEOPLAY             105
#define _ID_TIMER_SETTING               106
#define _ID_TIMER_SETTING_VERSION       107
#define _ID_TIMER_SETTING_LANGUAGE      108
#define _ID_TIMER_SETTING_EQ            109
#define _ID_TIMER_SETTING_BACKLIGHT     110
#define _ID_TIMER_SETTING_SCREENOFF     111

#define MSG_VIDEOPLAY_END         (MSG_USER + 1)

enum filter_filetype {
    FILTER_FILE_NO = 0,
    FILTER_FILE_MUSIC = 1,
    FILTER_FILE_GAME = 2,
    FILTER_FILE_PIC = 3,
    FILTER_FILE_ZIP = 4,
    FILTER_FILE_VIDEO = 5
};

enum filetype {
    FILE_FOLDER = 0,
    FILE_MUSIC = 1,
    FILE_GAME = 2,
    FILE_PIC = 3,
    FILE_ZIP = 4,
    FILE_VIDEO = 5,
    FILE_OTHER = 6,
    FILE_TYPE_MAX
};

struct file_node
{
    struct file_node *pre_node;
    struct file_node *next_node;
    char *name;
    int type;
};

struct directory_node
{
    struct directory_node *pre_node;
    struct directory_node *next_node;
    struct file_node *file_node_list;
    char *patch;
    int total;
    int file_sel;
};

#define UI_IMAGE_PATH         "/usr/local/share/minigui/res/images/"

#define BROWSER_PATH_ROOT     "/oem/file"
#define BROWSER_PATH_PIC      "/oem/file/pic"
#define BROWSER_PATH_MUSIC    "/oem/file/music"
#define BROWSER_PATH_GAME     "/oem/file/game"
#define BROWSER_PATH_VIDEO    "/oem/file/video"
        
//#include "ui_480x320.h"
#include "ui_480x272.h"

#include "desktop_dialog.h"
#include "pic_preview_dialog.h"
#include "browser_dialog.h"
#include "videoplay_dialog.h"
#include "audioplay_dialog.h"
#include "setting_dialog.h"
#include "setting_language_dialog.h"
#include "setting_eq_dialog.h"
#include "setting_screenoff_dialog.h"
#include "setting_backlight_dialog.h"
#include "setting_version_dialog.h"
#include "system.h"

extern BITMAP batt_bmap[6];
extern int battery;
extern BITMAP background_bmap;
extern RECT msg_rcBg;
extern RECT msg_rcBatt;
extern RECT msg_rcTitle;
extern RECT msg_rcDialog;
#endif
