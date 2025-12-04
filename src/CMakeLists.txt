# Add library subdirectories
add_subdirectory(mvvmcore)
add_subdirectory(mvvmwidgets)

# Optional modules
if(Qt${QT_VERSION_MAJOR}Quick_FOUND)
    add_subdirectory(mvvmquick)
    add_subdirectory(imports)
endif()

if(Qt${QT_VERSION_MAJOR}DataSync_FOUND)
    add_subdirectory(mvvmdatasynccore)
    add_subdirectory(mvvmdatasyncwidgets)
    
    if(Qt${QT_VERSION_MAJOR}Quick_FOUND)
        add_subdirectory(mvvmdatasyncquick)
    endif()
endif()

if(ANDROID AND Qt${QT_VERSION_MAJOR}AndroidExtras_FOUND)
    add_subdirectory(jar)
endif()
