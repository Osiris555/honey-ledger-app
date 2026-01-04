# -------------------------------------------------
# Compiler (MANDATORY for modern Ledger SDK)
# -------------------------------------------------
CC      := clang
CXX     := clang++
LD      := clang
AR      := llvm-ar
OBJCOPY := llvm-objcopy

# -------------------------------------------------
# Application metadata
# -------------------------------------------------
APPNAME = Honey
APPVERSION = 1.0.0
APP_LOAD_PARAMS = --curve secp256k1

# -------------------------------------------------
# Source files
# -------------------------------------------------
APP_SOURCE_PATH += src
APP_SOURCE_FILES += main.c

# -------------------------------------------------
# Include paths
# -------------------------------------------------
INCLUDES_PATH += include

# -------------------------------------------------
# Ledger SDK
# -------------------------------------------------
BOLOS_SDK ?= /opt/nanos-secure-sdk
include $(BOLOS_SDK)/Makefile.rules
