# Build Configuration for Native Interface Support

## Overview

QtMvvm can be built with various levels of native interface support. This document explains the build configuration options.

## Build Options

### Option 1: Full Build (Default)

Includes everything: Qt Widgets, QML, and Native interfaces.

**qmake:**
```bash
qmake
make
```

**CMake (if available):**
```bash
cmake -B build
cmake --build build
```

This is the recommended configuration for maximum compatibility.

### Option 2: Native Interface Only

Build only the core with native interfaces, no Qt Widgets GUI.

**qmake:**
```bash
qmake CONFIG+=native_only
make
```

**Defines:**
- `QTMVVM_NATIVE_INTERFACE_ONLY` - Excludes Qt Widgets specific code

**Benefits:**
- Smaller binary size
- Faster compilation
- Minimal Qt dependencies (only QtCore)

**Limitations:**
- No `WidgetsPresenter`
- No `QtWidgetAdapter`
- Must implement your own native views

### Option 3: Hybrid Build

Build with both Qt Widgets and Native interfaces (default).

**qmake:**
```bash
qmake CONFIG+=native_interface
make
```

**Defines:**
- `QTMVVM_NATIVE_INTERFACE` - Enables native interface support

**Benefits:**
- Use Qt Widgets where needed
- Use native views where desired
- Maximum flexibility

### Option 4: Qt Only (Legacy)

Build without native interface support.

**qmake:**
```bash
qmake CONFIG+=qt_only
make
```

**Benefits:**
- No new dependencies
- Smallest change from previous versions
- Proven stability

## Conditional Compilation

Use these defines in your code to support multiple configurations:

```cpp
#ifdef QTMVVM_NATIVE_INTERFACE
    // Code using native interfaces
    #include <QtMvvmCore/NativeInterface>
    auto presenter = new QtMvvm::NativeWidgetsPresenter();
#else
    // Traditional Qt-only code
    #include <QtMvvmWidgets/WidgetsPresenter>
    auto presenter = new QtMvvm::WidgetsPresenter();
#endif
```

## Project File Configuration

### Example .pro File

```pro
TARGET = MyApp
QT += core gui widgets

# Enable native interface support
CONFIG += native_interface

# Conditional dependencies
native_interface {
    QT += mvvmcore
    DEFINES += QTMVVM_NATIVE_INTERFACE
} else {
    QT += mvvmcore mvvmwidgets
}

SOURCES += main.cpp
```

### Example CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyApp)

# Option to enable native interface
option(QTMVVM_NATIVE_INTERFACE "Enable native interface support" ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

if(QTMVVM_NATIVE_INTERFACE)
    add_definitions(-DQTMVVM_NATIVE_INTERFACE)
    # Link native interface libraries
endif()

add_executable(MyApp main.cpp)
target_link_libraries(MyApp Qt5::Core Qt5::Gui Qt5::Widgets)
```

## Module Dependencies

### Core Module (`mvvmcore`)
Always required. Now includes:
- Traditional Qt interfaces (`IPresenter`, `ViewModel`)
- Native interfaces (`INativeView`, `INativePresenter`)
- Adapter classes (`PresenterAdapter`)

**Dependencies:**
- Qt Core
- Qt Gui (for `QColor` in messages)

### Widgets Module (`mvvmwidgets`)
Optional. Includes:
- `WidgetsPresenter` (Qt Widgets)
- `QtWidgetAdapter` (Native ↔ Qt bridge)
- `NativeWidgetsPresenter` (Native wrapper)

**Dependencies:**
- Qt Core
- Qt Gui
- Qt Widgets
- mvvmcore

### Quick Module (`mvvmquick`)
Optional. QML support (unchanged).

**Dependencies:**
- Qt Core
- Qt Gui  
- Qt Quick
- Qt QML
- mvvmcore

## Installation Paths

### System-Wide Installation

```bash
# Install to Qt directory
qmake
make install

# Files installed to:
# Headers: $QT_INSTALL_HEADERS/QtMvvmCore/
# Libraries: $QT_INSTALL_LIBS/
# Examples: $QT_INSTALL_EXAMPLES/qtmvvm/
```

### Local Installation

```bash
# Install to custom prefix
qmake PREFIX=/usr/local
make install

# Or with CMake:
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -B build
cmake --build build --target install
```

## Feature Detection

Check for native interface support at runtime:

```cpp
#include <QtMvvmCore/qtmvvmcore_global.h>

bool hasNativeInterface() {
#ifdef QTMVVM_NATIVE_INTERFACE
    return true;
#else
    return false;
#endif
}
```

## Compatibility Matrix

| Configuration | Qt Widgets | Native Interface | Binary Size | Use Case |
|---------------|-----------|------------------|-------------|----------|
| Full (Default) | ✓ | ✓ | Large | Maximum compatibility |
| Native Only | ✗ | ✓ | Small | Custom UI frameworks |
| Hybrid | ✓ | ✓ | Medium | Gradual migration |
| Qt Only | ✓ | ✗ | Medium | Legacy applications |

## Platform-Specific Notes

### Windows
- Use MSVC or MinGW compiler
- Qt 5.9+ recommended
- Native interfaces work with Win32 or custom UI

### Linux
- GCC 7+ or Clang 5+
- Qt 5.9+ from distribution
- Native interfaces work with GTK+, Qt, or console

### macOS
- Xcode 9+ 
- Qt 5.9+ from Homebrew or official installer
- Native interfaces work with Cocoa or Qt

### Android
- Android NDK r19+
- Qt 5.12+ for Android
- Limited native interface support (use Qt for UI)

### iOS
- Xcode 11+
- Qt 5.12+ for iOS
- Limited native interface support (use Qt for UI)

## Troubleshooting

### Issue: Native interface headers not found

**Solution:**
```bash
# Ensure native interface is enabled
qmake CONFIG+=native_interface
# Or check that QTMVVM_NATIVE_INTERFACE is defined
```

### Issue: Linker error with native classes

**Solution:**
```pro
# In your .pro file, ensure proper module loading
QT += mvvmcore mvvmwidgets
# Check that libraries are in linker path
```

### Issue: Runtime crashes with adapters

**Solution:**
- Ensure adapter lifetime matches adapted object
- Don't delete adapted objects while adapter exists
- Use smart pointers for automatic lifetime management

## Performance Tips

### Optimization Flags

```pro
# For native-only builds
CONFIG += native_only
QMAKE_CXXFLAGS += -O3

# Reduce binary size
CONFIG += strip
QMAKE_LFLAGS += -s
```

### Link-Time Optimization

```pro
# Enable LTO
CONFIG += ltcg
```

### Static Linking

```pro
# Static Qt libraries
CONFIG += static

# Results in larger binary but no dependencies
```

## Testing Configuration

```bash
# Build with tests
qmake CONFIG+=tests
make
make check

# Run specific tests
./tests/tst_nativeinterface
./tests/tst_presenteradapter
```

## Documentation Generation

```bash
# Generate API documentation
make docs

# Opens in browser
xdg-open doc/html/index.html
```

## Summary

The native interface can be enabled/disabled at build time with minimal impact on existing code. The default configuration includes everything for maximum compatibility, but you can optimize builds for your specific use case.

**Recommended Approach:**
1. Start with default full build
2. Test your application
3. Optimize build flags as needed
4. Consider native-only for production if not using Qt UI

## References

- [Native Interface Guide](native_interface_guide.md)
- [Migration Guide](MIGRATION_TO_NATIVE.md)
- [Qt Build System Documentation](https://doc.qt.io/qt-5/qmake-manual.html)
