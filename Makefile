APPNAME = Honey
APPVERSION = 0.1.0

SDK_PATH ?= /ledger-secure-sdk

CFLAGS += -Iinclude

SOURCES = src/main.c

include $(SDK_PATH)/Makefile.standard_app
