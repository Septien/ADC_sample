APPLICATION = main

BOARD ?= native

RIOTBASE ?= /home/phantom/CP_Systems/RTOS/RIOT

DEVHELP ?= 1

DEVELHELP ?= 1

USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += analog_util
USEMODULE += hd44780

FEATURES_REQUIRED += arch_esp32
FEATURES_REQUIRED += periph_adc
FEATURES_REQUIRED += periph_gpio

include $(RIOTBASE)/Makefile.include
