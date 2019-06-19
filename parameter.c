/*
 * Copyright (c) 2018 rockchip
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parameter.h"
#include <DeviceIo/Rk_wifi.h>

#define PARAMETER_FILE    "/data/parameter"
#define VERSION          1002

#define LANGUAGE_DEF     0
#define SCREENOFF_DEF    3     //list value
#define SCREENOFF_VAL_DEF    30     //wait time
#define EQ_DEF           0
#define BACKLIGHT_DEF    0
#define GAMEDISP_DEF     0
#define THEMESTYLE_DEF   0
#define VOLUME_DEF       20

struct parameter_data
{
    int version;
    int language;
    int screenoff;
    int screenoff_val;
    int eq_val;
    int backlight_val;
    int gamedisp_val;
    int themestyle_val;
	int volume_val;
	RK_WIFI_RUNNING_State_e wifi_state;
	char wifi_ssid[128];
	char wifi_psk[128];
};

static struct parameter_data para_data;

#define UI_IMAGE_PATH_0         "/usr/local/share/minigui/res/images/"
#define UI_IMAGE_PATH_1         "/usr/local/share/minigui/res/images1/"

static void set_version(int val)
{
    para_data.version = val;
}

static int parameter_save(void)
{
    FILE * fpFile = 0;

    fpFile = fopen(PARAMETER_FILE, "wb+");
    if (fpFile <= 0) {
        printf("create parameter file fail\n");
        return -1;
    }
    fwrite(&para_data, 1, sizeof(struct parameter_data), fpFile);
    fflush(fpFile);
    fsync(fpFile);
    fclose(fpFile);
    system("sync &");

    return 0;
}

int parameter_init(void)
{
    FILE * fpFile = 0;

    memset(&para_data, 0, sizeof(struct parameter_data));
    fpFile = fopen(PARAMETER_FILE, "r");
    if (fpFile <= 0) {
        parameter_recovery();
        return 0;
    }
    if (fpFile > 0)
        fclose(fpFile);

    fpFile = fopen(PARAMETER_FILE, "rb+");
    if (fpFile <= 0) {
        printf("open parameter file fail\n");
        return -1;
    }
    fread(&para_data, 1, sizeof(struct parameter_data), fpFile);
    fclose(fpFile);

    if (para_data.version != VERSION) {
        parameter_recovery();
    }
	
    return 0;
}

void parameter_deinit(void)
{


}

int parameter_recovery(void)
{
    FILE * fpFile = 0;

    set_version(VERSION);
    set_language(LANGUAGE_DEF);
    set_screenoff(SCREENOFF_DEF);
    set_screenoff_val(SCREENOFF_VAL_DEF);
    set_eq(EQ_DEF);
    set_backlight(BACKLIGHT_DEF);
    set_gamedisp(GAMEDISP_DEF);
    set_themestyle(THEMESTYLE_DEF);

	set_volume(VOLUME_DEF);
	set_wifi(0);
	set_wifi_date("  ", "  ");

    fpFile = fopen(PARAMETER_FILE, "wb+");
    if (fpFile <= 0) {
        printf("create parameter file fail\n");
        return -1;
    }
    fwrite(&para_data, 1, sizeof(struct parameter_data), fpFile);
    fclose(fpFile);

    return 0;
}

int get_language(void)
{
    return para_data.language;
}

void set_language(int val)
{
    para_data.language = val;
    parameter_save();
}

int get_screenoff(void)
{
    return para_data.screenoff;
}

void set_screenoff(int val)
{
    para_data.screenoff = val;
    parameter_save();
}

int get_screenoff_val(void)
{
    return para_data.screenoff_val;
}

void set_screenoff_val(int val)
{
    para_data.screenoff_val = val;
    parameter_save();
}

int get_eq(void)
{
    return para_data.eq_val;
}

void set_eq(int val)
{
    para_data.eq_val = val;
    parameter_save();
}

int get_backlight(void)
{
    return para_data.backlight_val;
}

void set_backlight(int val)
{
    para_data.backlight_val = val;
    parameter_save();
}

int get_gamedisp(void)
{
    return para_data.gamedisp_val;
}

void set_gamedisp(int val)
{
    para_data.gamedisp_val = val;
    parameter_save();
}

int get_themestyle(void)
{
    return para_data.themestyle_val;
}

void set_themestyle(int val)
{
    para_data.themestyle_val = val;
    parameter_save();
}

int get_volume(void)
{
    return para_data.volume_val;
}

void set_volume(int val)
{
    para_data.volume_val = val;
    parameter_save();
}

int get_wifi(void)
{
    return para_data.wifi_state;
}

void set_wifi(RK_WIFI_RUNNING_State_e val)
{

	para_data.wifi_state = val;
    parameter_save();
}


void set_wifi_date(char *ssid, char *psk)
{

	snprintf(para_data.wifi_ssid, 128, "%s", ssid);
	snprintf(para_data.wifi_psk, 128, "%s", psk);
    parameter_save();
}


char* get_wifi_ssid(void)
{
    return para_data.wifi_ssid;
}

char* get_wifi_psk(void)
{
    return para_data.wifi_psk;
}


int test_wifi_pwd(void) // exist retuen 1 ,esle 0 
{

	if (strcmp(para_data.wifi_psk, "  ") == 0)
		return 0;
	return 1;

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

