# App metadata (MANDATORY)
APPNAME = "Honey"
APPVERSION = 0.1.0
ICONNAME = icon.png

# Force Ledger-required compiler
CC := clang

# Source files
APP_SOURCE_PATH += app
APP_SOURCE_FILES += main.c apdu.c ux.c

# Ledger SDK (injected by Docker)
include $(BOLOS_SDK)/Makefile.rules
