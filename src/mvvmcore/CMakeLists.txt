project(QtMvvmCore)

# Library sources
set(SOURCES
    viewmodel.cpp
    coreapp.cpp
    serviceregistry.cpp
    qtmvvmcore_global.cpp
    binding.cpp
    message.cpp
    ipresenter.cpp
    settingsviewmodel.cpp
    isettingsaccessor.cpp
    qsettingsaccessor.cpp
    settingsentry.cpp
    settingsconfigloader.cpp
    nativeview.cpp
    nativepresenter.cpp
    presenteradapter.cpp
    nativeviewmodel.cpp
    viewmodeladapter.cpp
    nativebinding.cpp
    bindingadapter.cpp
    nativemessage.cpp
    messageadapter.cpp
    nativeserviceregistry.cpp
    serviceregistryadapter.cpp
)

set(HEADERS
    viewmodel.h
    qtmvvmcore_global.h
    viewmodel_p.h
    coreapp.h
    coreapp_p.h
    serviceregistry.h
    serviceregistry_p.h
    qtmvvmcore_helpertypes.h
    ipresenter.h
    qtmvvm_logging_p.h
    binding.h
    binding_p.h
    message.h
    message_p.h
    settingssetup.h
    settingsviewmodel_p.h
    settingsviewmodel.h
    injection.h
    isettingsaccessor.h
    qsettingsaccessor.h
    settingsentry.h
    settingsconfigloader_p.h
    exception.h
    nativeview.h
    nativepresenter.h
    presenteradapter.h
    nativeviewmodel.h
    viewmodeladapter.h
    nativebinding.h
    bindingadapter.h
    nativemessage.h
    messageadapter.h
    nativeserviceregistry.h
    serviceregistryadapter.h
)

# Android-specific sources
if(ANDROID AND Qt${QT_VERSION_MAJOR}AndroidExtras_FOUND)
    list(APPEND SOURCES
        androidsettingsaccessor.cpp
    )
    list(APPEND HEADERS
        androidsettingsaccessor.h
        androidsettingsaccessor_p.h
    )
endif()

# Create library
add_library(QtMvvmCore SHARED ${SOURCES} ${HEADERS})
add_library(QtMvvm::Core ALIAS QtMvvmCore)

# Target properties
target_compile_definitions(QtMvvmCore PRIVATE
    QT_BUILD_MVVMCORE_LIB
)

# Qt dependencies
target_link_libraries(QtMvvmCore PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

if(ANDROID AND Qt${QT_VERSION_MAJOR}AndroidExtras_FOUND)
    target_link_libraries(QtMvvmCore PRIVATE
        Qt${QT_VERSION_MAJOR}::AndroidExtras
    )
endif()

# Include directories
target_include_directories(QtMvvmCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QtMvvmCore>
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

# Translations
set(TS_FILES
    translations/qtmvvmcore_de.ts
    translations/qtmvvmcore_template.ts
)

if(Qt${QT_VERSION_MAJOR}_VERSION VERSION_GREATER_EQUAL "5.15.0")
    qt_add_translation(QM_FILES ${TS_FILES})
    target_sources(QtMvvmCore PRIVATE ${QM_FILES})
    
    install(FILES ${QM_FILES}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/translations
    )
endif()
