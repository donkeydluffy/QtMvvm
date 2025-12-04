project(NativeWidgetIntegration)

set(SOURCES
    main.cpp
    nativeexampleview.cpp
    nativeexampleviewmodel.cpp
)

set(HEADERS
    nativeexampleview.h
    nativeexampleviewmodel.h
)

set(FORMS
    nativeexampleview.ui
)

# Create executable
add_executable(NativeWidgetIntegration ${SOURCES} ${HEADERS} ${FORMS})

# Link libraries
target_link_libraries(NativeWidgetIntegration PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    QtMvvm::Core
    QtMvvm::Widgets
)

# Set C++ standard
target_compile_features(NativeWidgetIntegration PRIVATE cxx_std_14)

# Installation (optional)
install(TARGETS NativeWidgetIntegration
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples
)
