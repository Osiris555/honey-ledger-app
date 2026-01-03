APPNAME = Honey
APPVERSION = 1.0.0
APP_LOAD_PARAMS = --appFlags 0x240 --path "44'/118'/0'/0/0"

ifeq ($(TARGET_NAME),)
TARGET_NAME = $(TARGET)
endif

include sdk/Makefile.defines
include sdk/Makefile.rules
