TEMPLATE = app

QT += widgets
CONFIG += c++14

TARGET = NativeWidgetIntegration

SOURCES += \
    main.cpp \
    nativeexampleview.cpp \
    nativeexampleviewmodel.cpp

HEADERS += \
    nativeexampleview.h \
    nativeexampleviewmodel.h

FORMS += \
    nativeexampleview.ui

# Link to QtMvvm libraries
LIBS += -L$$OUT_PWD/../../src/mvvmcore/
LIBS += -L$$OUT_PWD/../../src/mvvmwidgets/

LIBS += -lQtMvvmCore -lQtMvvmWidgets

INCLUDEPATH += $$PWD/../../src/mvvmcore \
               $$PWD/../../src/mvvmwidgets

DEPENDPATH += $$PWD/../../src/mvvmcore \
              $$PWD/../../src/mvvmwidgets
