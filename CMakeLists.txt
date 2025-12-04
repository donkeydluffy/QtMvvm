cmake_minimum_required(VERSION 3.16)

project(QtMvvm VERSION 1.1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# Optional Qt modules
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick DataSync AndroidExtras QUIET)

# Build options
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# Installation directories
include(GNUInstallDirs)

# Add subdirectories
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_DOCS)
    add_subdirectory(doc)
endif()

# Export targets
install(EXPORT QtMvvmTargets
    FILE QtMvvmTargets.cmake
    NAMESPACE QtMvvm::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtMvvm
)

# Create config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/QtMvvmConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QtMvvmConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtMvvm
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/QtMvvmConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QtMvvmConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/QtMvvmConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtMvvm
)
