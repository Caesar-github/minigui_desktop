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

#include "desktop_dialog.h"
#include "pic_preview_dialog.h"
#include "system.h"

#define _ID_TIMER 100
#define _ID_TIMER_1 101

#define IDC_FUN1     110
#define IDC_FUN2     111
#define IDC_FUN3     112
#define IDC_FUN4     113
#define IDC_FUN5     114

#define IDC_WIFI_SET_BT_MIN   120
#define IDC_WIFI_SET_BT_MAX   164

#define IDC_UI_WAIT     170

#define MSG_WIFI_SCAN_FINISH      (MSG_USER + 1)

#define LCD_W    1024
#define LCD_H    600

#define TIME_PINT_X    LCD_W - 96
#define TIME_PINT_Y    30
#define TIME_PINT_W    64
#define TIME_PINT_H    32

#define BATT_PINT_X    32
#define BATT_PINT_Y    16
#define BATT_PINT_W    64
#define BATT_PINT_H    64

#define BG_PINT_X    0
#define BG_PINT_Y    0
#define BG_PINT_W    LCD_W
#define BG_PINT_H    LCD_H

#define DESKTOP_DLG_X    0
#define DESKTOP_DLG_Y    80
#define DESKTOP_DLG_W    LCD_W - DESKTOP_DLG_X
#define DESKTOP_DLG_H    LCD_H - DESKTOP_DLG_Y
#define DESKTOP_DLG_STRING    "desktop"

#endif
