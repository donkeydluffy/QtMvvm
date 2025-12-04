TARGET = QtMvvmQtAdapter
TEMPLATE = lib

QT = core gui widgets

CONFIG += c++14

DEFINES += QT_BUILD_QTMVVMCORE_LIB

# Link to pure C++ mvvmcore
LIBS += -L$$OUT_PWD/../mvvmcore -lQtMvvmCore
INCLUDEPATH += $$PWD/../mvvmcore
DEPENDPATH += $$PWD/../mvvmcore

HEADERS += \
    # Core adapters
    presenteradapter.h \
    viewmodeladapter.h \
    bindingadapter.h \
    messageadapter.h \
    serviceregistryadapter.h \
    # Qt MVVM implementations
    binding.h \
    binding_p.h \
    coreapp.h \
    coreapp_p.h \
    ipresenter.h \
    injection.h \
    message.h \
    message_p.h \
    serviceregistry.h \
    serviceregistry_p.h \
    viewmodel.h \
    viewmodel_p.h \
    # Settings system
    isettingsaccessor.h \
    qsettingsaccessor.h \
    settingsconfigloader_p.h \
    settingsentry.h \
    settingssetup.h \
    settingsviewmodel.h \
    settingsviewmodel_p.h \
    # Qt Widget adapters
    qtwidgetadapter.h \
    nativewidgetspresenter.h \
    # Qt globals
    qtmvvmcore_global.h \
    qtmvvmcore_helpertypes.h \
    qtmvvm_logging_p.h

SOURCES += \
    # Core adapters
    presenteradapter.cpp \
    viewmodeladapter.cpp \
    bindingadapter.cpp \
    messageadapter.cpp \
    serviceregistryadapter.cpp \
    # Qt MVVM implementations
    binding.cpp \
    coreapp.cpp \
    ipresenter.cpp \
    message.cpp \
    serviceregistry.cpp \
    viewmodel.cpp \
    # Settings system
    isettingsaccessor.cpp \
    qsettingsaccessor.cpp \
    settingsconfigloader.cpp \
    settingsentry.cpp \
    settingsviewmodel.cpp \
    # Qt Widget adapters
    qtwidgetadapter.cpp \
    nativewidgetspresenter.cpp \
    # Qt globals
    qtmvvmcore_global.cpp

android {
    HEADERS += \
        androidsettingsaccessor.h \
        androidsettingsaccessor_p.h
    
    SOURCES += \
        androidsettingsaccessor.cpp
}

load(qt_module)
