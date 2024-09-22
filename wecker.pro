HEADERS += \
    bilder.h \
    einstellenForm.h \
    i2c.h \
    menue.h \
    paint.h \
    pin.h \
    print.h \
    rtc.h \
    segment14Font.h \
    tft.h \
    tft_ST7735.h \
    uart_hard.h \
    uart_print.10.h \
    uart_print.h \
    uart_soft_PlayerMini.h \
    uhr.h \
    wavPlayer.h

SOURCES += \
    einstellenForm.c \
    i2c.c \
    main.c \
    menue.c \
    paint.c \
    print.c \
    rtc.c \
    segment14Font.c \
    tft.c \
    tft_ST7735.c \
    uart_hard.c \
    uart_print.c \
    uart_soft_PlayerMini.c \
    uhr.c \
    wavPlayer.c

DISTFILES += \
    Makefile

INCLUDEPATH += \+
    /usr/lib/avr/include
