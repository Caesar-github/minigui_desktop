PROJECT_DIR := $(shell pwd)
CC ?= ../../buildroot/output/rockchip_rk3128_game/host/bin/arm-buildroot-linux-gnueabihf-gcc
BIN = game

STAGING_DIR ?= ../../buildroot/output/rockchip_rk3128_game/staging/

OBJ = desktop_dialog.o  main.o  pic_preview_dialog.o  system.o
CFLAGS ?= -I./include \
	  -I$(STAGING_DIR)/usr/include \
          -I$(STAGING_DIR)/usr/include/libdrm \
          -I$(STAGING_DIR)/usr/include \
	  -L$(STAGING_DIR)/usr/lib \
	  -lpthread  -ldrm -lminigui_ths -ljpeg -lpng -lm

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ) $(BIN)
