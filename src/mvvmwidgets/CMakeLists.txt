project(QtMvvmWidgets)

# Library sources
set(SOURCES
    widgetspresenter.cpp
    fontcombobox.cpp
    selectcombobox.cpp
    inputwidgetfactory.cpp
    settingsdialog.cpp
    tooltipslider.cpp
    coloredit.cpp
    progressdialog.cpp
    qtwidgetadapter.cpp
    nativewidgetspresenter.cpp
)

set(HEADERS
    qtmvvmwidgets_global.h
    widgetspresenter.h
    ipresentingview.h
    widgetspresenter_p.h
    fontcombobox_p.h
    selectcombobox_p.h
    inputwidgetfactory.h
    inputwidgetfactory_p.h
    settingsdialog_p.h
    settingsdialog.h
    tooltipslider_p.h
    coloredit_p.h
    progressdialog_p.h
    qtwidgetadapter.h
    nativewidgetspresenter.h
)

set(FORMS
    settingsdialog.ui
)

set(RESOURCES
    qtmvvmwidgets_module.qrc
)

# Create library
add_library(QtMvvmWidgets SHARED ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})
add_library(QtMvvm::Widgets ALIAS QtMvvmWidgets)

# Target properties
target_compile_definitions(QtMvvmWidgets PRIVATE
    QT_BUILD_MVVMWIDGETS_LIB
)

# Qt dependencies
target_link_libraries(QtMvvmWidgets PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    QtMvvm::Core
)

# Include directories
target_include_directories(QtMvvmWidgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QtMvvmWidgets>
)

# Installation
install(TARGETS QtMvvmWidgets
    EXPORT QtMvvmTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtMvvmWidgets
)

# Install convenience header
install(FILES QtNativeWidgets
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtMvvmWidgets
)

# Translations
set(TS_FILES
    translations/qtmvvmwidgets_de.ts
    translations/qtmvvmwidgets_template.ts
)

if(Qt${QT_VERSION_MAJOR}_VERSION VERSION_GREATER_EQUAL "5.15.0")
    qt_add_translation(QM_FILES ${TS_FILES})
    target_sources(QtMvvmWidgets PRIVATE ${QM_FILES})
    
    install(FILES ${QM_FILES}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/translations
    )
endif()
