# QtMvvm Core - Pure C++ MVVM Framework

**100% Qt-Independent** - Can be used in ANY C++ project!

## Overview

**mvvmcore** is a pure C++ MVVM (Model-View-ViewModel) framework with **ZERO Qt dependencies**.

Use it in:
- Console applications
- Embedded systems  
- Game engines (Unity, Unreal, Godot, etc.)
- Web backends
- Mobile apps (without Qt)
- Any C++ UI framework (wxWidgets, GTK+, ImGui, etc.)
- Cross-platform libraries

## Pure C++ Interfaces

### View Layer
- **INativeView** - Pure virtual view interface using `std::string`
- **INativePresentingView** - Parent view presentation
- **INativeViewFactory** - Factory pattern for view creation

### Presenter Layer  
- **INativePresenter** - Presenter using `std::map<std::string, std::string>`
- **NativePresenterBase** - Base implementation with factory management
- **NativeMessageConfig** - Message dialogs without Qt

### ViewModel Layer
- **INativeViewModel** - ViewModel using `std::function` callbacks
- **NativeViewModelBase** - Base implementation with result callbacks
- `onInit()` and `onResult()` methods with native types

### Data Binding
- **INativeBinding** - Data binding without meta-object system
- **INativeProperty** - Property access and change notification
- **NativeBindingBase** - Two-way binding with callbacks
- Supports SingleInit, OneWayToView, OneWayToViewModel, TwoWay

### Message System
- **INativeMessageResult** - Message result with `std::function` callbacks
- **INativeProgressControl** - Progress tracking (determinate/indeterminate)
- **NativeMessageResultBase** / **NativeProgressControlBase** - Implementations

### Dependency Injection
- **INativeServiceRegistry** - Service registry without Qt meta-object
- **TypedServiceRegistry** - Type-safe registration template
- **NativeServiceRegistryBase** - Implementation with destruction scopes
- **NativeServiceException** hierarchy - Standard C++ exceptions

## No Qt Required!

```cpp
// Pure C++ - compiles without Qt installed
#include <iostream>
#include <memory>
#include "nativeviewmodel.h"
#include "nativeview.h"

class MyViewModel : public NativeViewModelBase {
public:
    void onInit(const std::map<std::string, std::string> &params) override {
        std::cout << "ViewModel initialized!" << std::endl;
    }
};

class ConsoleView : public INativeView {
public:
    void show() override {
        std::cout << "View shown" << std::endl;
    }
    
    void setTitle(const std::string &title) override {
        std::cout << "Title: " << title << std::endl;
    }
    
    // ... other INativeView methods
};
```

## Building

### With CMake (Pure C++)
```bash
mkdir build && cd build
cmake .. -DBUILD_QTMVVM_CORE_ONLY=ON  # NO Qt required!
cmake --build .
```

### Using in Your Project

```cmake
find_package(QtMvvm REQUIRED COMPONENTS Core)
target_link_libraries(myapp PRIVATE QtMvvm::Core)
```

**NO Qt** in your `find_package()` or `target_link_libraries()` calls!

## Components

### Header Files (Pure C++)
- `exception.h` - C++ exception classes
- `nativeview.h` - View interfaces
- `nativepresenter.h` - Presenter interfaces
- `nativeviewmodel.h` - ViewModel interfaces
- `nativebinding.h` - Data binding interfaces
- `nativemessage.h` - Message/Progress interfaces
- `nativeserviceregistry.h` - DI container interfaces

### Source Files
- `nativeview.cpp`
- `nativepresenter.cpp`
- `nativeviewmodel.cpp`
- `nativebinding.cpp`
- `nativemessage.cpp`
- `nativeserviceregistry.cpp`

### Convenience Header
- `NativeInterface` - Includes all native interfaces

## Architecture Benefits

### ✅ Framework Independent
Use with any C++ UI framework - not locked into Qt

### ✅ Testable
Easy to mock interfaces for unit testing

### ✅ Portable
Standard C++14 - works anywhere C++ works

### ✅ Lightweight
~2,500 lines of pure C++ code, no heavy dependencies

### ✅ Performance
Zero-cost abstractions, no meta-object overhead

### ✅ Modern C++
Uses `std::string`, `std::function`, `std::map`, smart pointers

## Example Use Cases

### Console Application
```cpp
#include "nativeviewmodel.h"
#include "nativeview.h"

class DataProcessorVM : public NativeViewModelBase {
    void processData() {
        // Pure C++ business logic
        emitResult("Processing complete");
    }
};
```

### Game Engine Integration
```cpp
class GameUIView : public INativeView {
    void show() override {
        // Show using game engine's UI system
        GameEngine::UI::ShowPanel(this);
    }
};
```

### Embedded System
```cpp
class SensorViewModel : public NativeViewModelBase {
    void readSensor() {
        // Read hardware sensor
        int value = Hardware::ReadTemperature();
        // Update via callback
        propertyChanged("temperature");
    }
};
```

## For Qt Users

If you're using Qt, you want the **mvvmqtadapter** module instead:

```cmake
find_package(QtMvvm REQUIRED COMPONENTS Core QtAdapter)
target_link_libraries(myapp PRIVATE
    QtMvvm::Core       # Pure C++ core
    QtMvvm::QtAdapter  # Qt bridge
)
```

See [mvvmqtadapter](../mvvmqtadapter/README.md) for Qt integration.

## Design Philosophy

This module is **deliberately minimal** and **Qt-free**:
- No `QObject`, `QString`, `QVariant`, etc.
- Only standard C++ types and STL
- Can compile and run without Qt installed
- Business logic completely independent of UI framework

## Documentation

- [Native Interface Guide](../../doc/native_interface_guide.md) - Technical reference
- [Migration Guide](../../doc/MIGRATION_TO_NATIVE.md) - Moving to native interfaces
- [Examples](../../examples/native/) - Working code samples

## Dependencies

**NONE!** Just standard C++14 compiler.

## Module Structure

```
mvvmcore/
├── CMakeLists.txt       # Pure C++ build (NO Qt)
├── mvvmcore.pro         # Pure C++ qmake (NO Qt)
├── README.md            # This file
│
├── Pure C++ Headers
│   ├── exception.h
│   ├── nativeview.h
│   ├── nativepresenter.h
│   ├── nativeviewmodel.h
│   ├── nativebinding.h
│   ├── nativemessage.h
│   └── nativeserviceregistry.h
│
├── Pure C++ Sources
│   ├── nativeview.cpp
│   ├── nativepresenter.cpp
│   ├── nativeviewmodel.cpp
│   ├── nativebinding.cpp
│   ├── nativemessage.cpp
│   └── nativeserviceregistry.cpp
│
└── Convenience Header
    └── NativeInterface
```

## See Also

- [mvvmqtadapter](../mvvmqtadapter/README.md) - Qt bridge layer
- [mvvmwidgets](../mvvmwidgets/README.md) - Qt Widgets integration
- [Complete Native Interface Summary](../../doc/COMPLETE_NATIVE_INTERFACE_SUMMARY.md)
