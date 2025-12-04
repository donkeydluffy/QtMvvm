# QtMvvm Core - Pure C++ MVVM Framework
# NO Qt dependencies - can be used in any C++ project

cmake_minimum_required(VERSION 3.16)
project(QtMvvmCore LANGUAGES CXX)

# Pure C++ source files (NO Qt)
set(SOURCES
    nativeview.cpp
    nativepresenter.cpp
    nativeviewmodel.cpp
    nativebinding.cpp
    nativemessage.cpp
    nativeserviceregistry.cpp
)

# Pure C++ header files (NO Qt)
set(HEADERS
    exception.h
    nativeview.h
    nativepresenter.h
    nativeviewmodel.h
    nativebinding.h
    nativemessage.h
    nativeserviceregistry.h
)

# Create pure C++ library (NO Qt linkage)
add_library(QtMvvmCore ${SOURCES} ${HEADERS})
add_library(QtMvvm::Core ALIAS QtMvvmCore)

# C++14 standard required
target_compile_features(QtMvvmCore PUBLIC cxx_std_14)

# Include directories
target_include_directories(QtMvvmCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QtMvvmCore>
)

# NO Qt dependencies - pure C++ only!
# This library can be used in any C++ project without Qt

# Compile definitions
target_compile_definitions(QtMvvmCore PUBLIC
    QTMVVM_PURE_CPP
)

# Installation
install(TARGETS QtMvvmCore
    EXPORT QtMvvmTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtMvvmCore
)

# Install convenience header
install(FILES NativeInterface
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtMvvmCore
)

# Export for find_package()
export(TARGETS QtMvvmCore
    NAMESPACE QtMvvm::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/QtMvvmCoreTargets.cmake"
)

message(STATUS "QtMvvmCore: Pure C++ MVVM framework (NO Qt dependencies)")
