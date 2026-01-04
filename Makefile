APPNAME = "Honey"
ICONNAME = icon.png

APP_SOURCE_PATH += app
APP_SOURCE_FILES += main.c apdu.c ux.c

# Ledger SDK is injected by the builder container
include Makefile.rules
