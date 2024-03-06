QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/llftranslator.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/llftranslator.h \
    src/mainwindow.h

FORMS += \
    src/mainwindow.ui

INCLUDEPATH += "C:/MSFS SDK/SimConnect SDK/include"
INCLUDEPATH += "./src"

LIBS += -L"C:/MSFS SDK/SimConnect SDK/lib" -lSimConnect

RESOURCES += ./src/config.csv

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
