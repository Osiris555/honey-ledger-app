APPNAME = "Honey"
ICONNAME = icon.png

APP_SOURCE_PATH += app
APP_SOURCE_FILES += main.c apdu.c ux.c

SDK_PATH ?= sdk

include $(SDK_PATH)/Makefile.rules
