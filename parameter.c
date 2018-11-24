/*
 * Copyright (c) 2018 rockchip
 *
 */

#include "parameter.h"

#define LANGUAGE_DEF     0
#define SCREENOFF_DEF    0
#define EQ_DEF           0
#define BACKLIGHT_DEF    0
#define GAMEDISP_DEF     0
#define THEMESTYLE_DEF   0

static int language = 0;
static int screenoff_val = 0;
static int eq_val = 0;
static int backlight_val = 0;
static int gamedisp_val = 0;
static int themestyle_val = 0;

#define UI_IMAGE_PATH_0         "/usr/local/share/minigui/res/images/"
#define UI_IMAGE_PATH_1         "/usr/local/share/minigui/res/images1/"

int parameter_recovery(void)
{
    set_language(LANGUAGE_DEF);
    set_screenoff(SCREENOFF_DEF);
    set_eq(EQ_DEF);
    set_backlight(BACKLIGHT_DEF);
    set_gamedisp(GAMEDISP_DEF);
    set_themestyle(THEMESTYLE_DEF);

    return 0;
}

int get_language(void)
{
    return language;
}

void set_language(int val)
{
    language = val;
}

int get_screenoff(void)
{
    return screenoff_val;
}

void set_screenoff(int val)
{
    screenoff_val = val;
}

int get_eq(void)
{
    return eq_val;
}

void set_eq(int val)
{
    eq_val = val;
}

int get_backlight(void)
{
    return backlight_val;
}

void set_backlight(int val)
{
    backlight_val = val;
}

int get_gamedisp(void)
{
    return gamedisp_val;
}

void set_gamedisp(int val)
{
    gamedisp_val = val;
}

int get_themestyle(void)
{
    return themestyle_val;
}

void set_themestyle(int val)
{
    themestyle_val = val;
}

char *get_ui_image_path(void)
{
    if (get_themestyle() == 0)
        return UI_IMAGE_PATH_0;
    else if (get_themestyle() == 1)
        return UI_IMAGE_PATH_1;
    else
        return UI_IMAGE_PATH_0;
}