# QtMvvm Core - Pure C++ MVVM Framework
# NO Qt dependencies

TARGET = QtMvvmCore
TEMPLATE = lib
CONFIG += c++14

# NO Qt modules - pure C++ only
QT =

DEFINES += QTMVVM_PURE_CPP

# Pure C++ headers (NO Qt)
HEADERS += \
    exception.h \
    nativeview.h \
    nativepresenter.h \
    nativeviewmodel.h \
    nativebinding.h \
    nativemessage.h \
    nativeserviceregistry.h

# Pure C++ sources (NO Qt)
SOURCES += \
    nativeview.cpp \
    nativepresenter.cpp \
    nativeviewmodel.cpp \
    nativebinding.cpp \
    nativemessage.cpp \
    nativeserviceregistry.cpp

# This is a pure C++ library
# No Qt dependencies, can be used in any C++ project

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

headers.files = $$HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/QtMvvmCore
INSTALLS += headers
