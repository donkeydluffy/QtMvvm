# QtMvvm Qt Adapter Library
# Bridges pure C++ mvvmcore with Qt applications

cmake_minimum_required(VERSION 3.16)

# Find Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# Source files
set(SOURCES
    # Core adapters
    presenteradapter.cpp
    viewmodeladapter.cpp
    bindingadapter.cpp
    messageadapter.cpp
    serviceregistryadapter.cpp
    
    # Qt MVVM implementations
    binding.cpp
    coreapp.cpp
    ipresenter.cpp
    message.cpp
    serviceregistry.cpp
    viewmodel.cpp
    
    # Settings system
    isettingsaccessor.cpp
    qsettingsaccessor.cpp
    settingsconfigloader.cpp
    settingsentry.cpp
    settingsviewmodel.cpp
    
    # Qt Widget adapters
    qtwidgetadapter.cpp
    nativewidgetspresenter.cpp
    
    # Qt globals
    qtmvvmcore_global.cpp
)

# Header files
set(HEADERS
    # Core adapters
    presenteradapter.h
    viewmodeladapter.h
    bindingadapter.h
    messageadapter.h
    serviceregistryadapter.h
    
    # Qt MVVM implementations
    binding.h
    binding_p.h
    coreapp.h
    coreapp_p.h
    ipresenter.h
    injection.h
    message.h
    message_p.h
    serviceregistry.h
    serviceregistry_p.h
    viewmodel.h
    viewmodel_p.h
    
    # Settings system
    isettingsaccessor.h
    qsettingsaccessor.h
    settingsconfigloader_p.h
    settingsentry.h
    settingssetup.h
    settingsviewmodel.h
    settingsviewmodel_p.h
    
    # Qt Widget adapters
    qtwidgetadapter.h
    nativewidgetspresenter.h
    
    # Qt globals
    qtmvvmcore_global.h
    qtmvvmcore_helpertypes.h
    qtmvvm_logging_p.h
)

# Android-specific files
if(ANDROID)
    list(APPEND SOURCES
        androidsettingsaccessor.cpp
    )
    list(APPEND HEADERS
        androidsettingsaccessor.h
        androidsettingsaccessor_p.h
    )
endif()

# Create library
add_library(QtMvvmQtAdapter ${SOURCES} ${HEADERS})
add_library(QtMvvm::QtAdapter ALIAS QtMvvmQtAdapter)

# Include directories
target_include_directories(QtMvvmQtAdapter
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/QtMvvm>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(QtMvvmQtAdapter
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        QtMvvm::Core  # Pure C++ mvvmcore
    PRIVATE
        ${CMAKE_DL_LIBS}
)

# Compiler features
target_compile_features(QtMvvmQtAdapter PUBLIC cxx_std_14)

# Compile definitions
target_compile_definitions(QtMvvmQtAdapter
    PRIVATE
        QT_BUILD_QTMVVMCORE_LIB
    PUBLIC
        QT_MVVMCORE_LIB
)

# Installation
install(TARGETS QtMvvmQtAdapter
    EXPORT QtMvvmTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers
install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtMvvm/QtAdapter
)

# Export target
export(TARGETS QtMvvmQtAdapter
    NAMESPACE QtMvvm::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/QtMvvmQtAdapterTargets.cmake"
)
