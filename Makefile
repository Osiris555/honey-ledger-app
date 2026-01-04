APPNAME = "Honey"
ICONNAME = icon.png

APP_SOURCE_PATH += app
APP_SOURCE_FILES += main.c apdu.c ux.c

# Ledger SDK (injected by Docker)
include $(BOLOS_SDK)/Makefile.rules
