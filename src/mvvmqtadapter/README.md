# QtMvvm Qt Adapter

This module provides the Qt bridge layer for the pure C++ mvvmcore framework.

## Purpose

**mvvmqtadapter** bridges the gap between:
- **mvvmcore** - Pure C++ MVVM framework (NO Qt dependencies)
- **Qt applications** - QWidget, QML, and other Qt-based UIs

## Architecture

```
Qt Application
      ↓
mvvmqtadapter (This module)
      ↓
  mvvmcore (Pure C++)
```

## Components

### Qt MVVM Implementations
- `ViewModel` - Qt-based ViewModel with QObject signals/slots
- `Binding` - Data binding using Qt meta-object system
- `Message` - Message dialogs with Qt integration
- `ServiceRegistry` - Dependency injection with Qt meta-object
- `IPresenter` - Presenter interface using Qt types
- `CoreApp` - Qt application lifecycle management

### Adapters (Qt ↔ Native)
- `PresenterAdapter` - Converts between IPresenter and INativePresenter
- `ViewModelAdapter` - Converts between ViewModel and INativeViewModel
- `BindingAdapter` - Converts between Binding and INativeBinding
- `MessageAdapter` - Converts between Message and INativeMessage
- `ServiceRegistryAdapter` - Converts between ServiceRegistry and INativeServiceRegistry

### Qt Widget Adapters
- `QtWidgetAdapter` - Makes QWidget work with INativeView
- `NativeWidgetsPresenter` - Widgets presenter using native interfaces

### Settings System
- `ISettingsAccessor` / `QSettingsAccessor` - Settings access
- `SettingsViewModel` - Settings UI management
- `SettingsEntry` - Individual setting entries

## Usage

### For Qt Projects

Link against mvvmqtadapter to get Qt MVVM functionality:

```cmake
find_package(QtMvvm REQUIRED COMPONENTS Core QtAdapter)
target_link_libraries(myapp PRIVATE
    QtMvvm::Core       # Pure C++ core
    QtMvvm::QtAdapter  # Qt bridge
)
```

### Code Example

```cpp
#include <QtMvvmCore/ViewModel>
#include <QtMvvmCore/ServiceRegistry>

// Use Qt-based ViewModel
class MyViewModel : public QtMvvm::ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    
public:
    QString name() const;
    void setName(const QString &name);
    
signals:
    void nameChanged(const QString &name);
};

// Register in Qt service registry
QtMvvm::ServiceRegistry::instance()->registerObject<MyViewModel>();
```

## Design Philosophy

This module contains **all Qt-dependent code** that was previously in mvvmcore:
- Allows mvvmcore to be pure C++
- Provides seamless Qt integration
- Maintains 100% backward compatibility
- No changes required for existing Qt applications

## Backward Compatibility

All existing Qt applications continue to work unchanged by using mvvmqtadapter.
The API surface is identical to the previous mvvmcore.

## Building

### With CMake
```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt
cmake --build .
```

### With qmake
```bash
qmake
make
```

## Dependencies

- **Qt5 or Qt6** (Core, Gui, Widgets)
- **mvvmcore** (Pure C++ MVVM framework)

## Module Structure

```
mvvmqtadapter/
├── CMakeLists.txt          # CMake build configuration
├── mvvmqtadapter.pro       # qmake build configuration
├── README.md               # This file
│
├── Core Qt MVVM Classes
│   ├── binding.h/cpp
│   ├── coreapp.h/cpp
│   ├── ipresenter.h/cpp
│   ├── message.h/cpp
│   ├── serviceregistry.h/cpp
│   └── viewmodel.h/cpp
│
├── Adapters
│   ├── presenteradapter.h/cpp
│   ├── viewmodeladapter.h/cpp
│   ├── bindingadapter.h/cpp
│   ├── messageadapter.h/cpp
│   └── serviceregistryadapter.h/cpp
│
├── Qt Widget Integration
│   ├── qtwidgetadapter.h/cpp
│   └── nativewidgetspresenter.h/cpp
│
├── Settings System
│   ├── isettingsaccessor.h/cpp
│   ├── qsettingsaccessor.h/cpp
│   ├── settingsviewmodel.h/cpp
│   ├── settingsentry.h/cpp
│   └── settingsconfigloader.cpp
│
└── Qt Globals
    ├── qtmvvmcore_global.h/cpp
    ├── qtmvvmcore_helpertypes.h
    ├── qtmvvm_logging_p.h
    └── injection.h
```

## See Also

- [mvvmcore](../mvvmcore/README.md) - Pure C++ MVVM framework
- [mvvmwidgets](../mvvmwidgets/README.md) - Qt Widgets integration
- [Native Interface Guide](../../doc/native_interface_guide.md) - Using pure C++ interfaces
