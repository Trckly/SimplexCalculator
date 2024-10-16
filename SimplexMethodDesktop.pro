QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dualsimplexclass.cpp \
    gomoryclass.cpp \
    lpmethods.cpp \
    main.cpp \
    mainwindow.cpp \
    simplexclass.cpp \
    TableBuilders/simplextablebuilder.cpp \
    TableBuilders/transportationtablebuilder.cpp \
    transportpotentialmethod.cpp

HEADERS += \
    Dialogs/TransportationDialog.h \
    dualsimplexclass.h \
    gomoryclass.h \
    lpmethods.h \
    mainwindow.h \
    simplexclass.h \
    TableBuilders/simplextablebuilder.h \
    TableBuilders/transportationtablebuilder.h \
    transportpotentialmethod.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /opt/homebrew/Cellar/boost/1.86.0_1/include

LIBS += -L/opt/homebrew/Cellar/boost/1.86.0_1/lib -lboost_system -lboost_filesystem


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
