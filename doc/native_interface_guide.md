# Native C++ Interface Guide

## Overview

QtMvvm now provides pure C++ interfaces that allow you to use the MVVM architecture without depending on Qt types in your UI layer. This makes the framework more portable and allows integration with various C++ UI frameworks.

## Key Components

### 1. INativeView

The `INativeView` interface provides a pure C++ abstraction for views:

```cpp
class INativeView {
public:
    virtual ~INativeView() = default;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void close() = 0;
    virtual bool isVisible() const = 0;
    virtual void setTitle(const std::string &title) = 0;
    virtual std::string title() const = 0;
    // ... more methods
};
```

**Benefits:**
- No Qt dependencies
- Works with any C++ UI framework
- Easy to implement for custom platforms
- Simplifies testing with mock implementations

### 2. INativePresenter

The `INativePresenter` interface manages view presentation without Qt types:

```cpp
class INativePresenter {
public:
    virtual ~INativePresenter() = default;
    virtual void present(ViewModel *viewModel, 
                        const std::map<std::string, std::string> &params, 
                        ViewModel *parentViewModel = nullptr) = 0;
    virtual void showDialog(const NativeMessageConfig &config, 
                           MessageResult *result) = 0;
    // ... more methods
};
```

**Features:**
- Uses standard C++ containers (std::map, std::string)
- Decoupled from Qt's type system
- Can be implemented for any platform

### 3. Adapter Pattern

Two adapter classes bridge between Qt and native interfaces:

#### PresenterAdapter
Wraps an `INativePresenter` and exposes it as an `IPresenter`:
```cpp
PresenterAdapter adapter(nativePresenter);
ServiceRegistry::instance()->registerInterface<IPresenter>(&adapter);
```

#### NativePresenterAdapter
Wraps an `IPresenter` and exposes it as an `INativePresenter`:
```cpp
NativePresenterAdapter adapter(qtPresenter);
adapter.present(viewModel, nativeParams, parent);
```

### 4. QtWidgetAdapter

Makes QWidget compatible with `INativeView`:

```cpp
auto widget = new MyWidget(viewModel, parent);
auto nativeView = new QtWidgetAdapter(widget);
// Now use nativeView through INativeView interface
nativeView->show();
```

## Usage Patterns

### Pattern 1: Pure Native Implementation

For completely Qt-independent UIs:

```cpp
// 1. Implement your native view
class MyNativeView : public INativeView {
    // Implement all INativeView methods
    // Use your preferred UI framework
};

// 2. Implement your native presenter
class MyNativePresenter : public NativePresenterBase {
    void present(ViewModel *vm, const std::map<std::string, std::string> &params, 
                 ViewModel *parent) override {
        auto view = new MyNativeView();
        // Initialize and show view
    }
};

// 3. Use it
auto presenter = new MyNativePresenter();
// Register and use presenter
```

### Pattern 2: Gradual Migration from Qt

Migrate existing Qt code incrementally:

```cpp
// Step 1: Keep existing Qt widgets
auto qtWidget = new MyQtWidget(viewModel, parent);

// Step 2: Wrap in adapter
auto nativeView = new QtWidgetAdapter(qtWidget);

// Step 3: Use through native interface
nativeView->show(); // Works with native interface

// Step 4: Eventually replace with pure native implementation
```

### Pattern 3: Hybrid Approach

Use both Qt and native views in the same application:

```cpp
// Register Qt widget factory
auto qtFactory = std::make_shared<QtWidgetViewFactory>();
qtFactory->registerWidgetCreator("MyViewModel", 
    [](ViewModel *vm, QWidget *parent) {
        return new MyQtWidget(vm, parent);
    });

// Register native view factory
auto nativeFactory = std::make_shared<MyNativeViewFactory>();

// Register both with presenter
presenter->registerViewFactory(qtFactory);
presenter->registerViewFactory(nativeFactory);
```

## Migration Guide

### Step 1: Assess Current Architecture

Identify components using Qt-specific types:
- `QWidget` in view implementations
- `QVariantHash` in presenter interfaces
- Qt signals/slots for view-viewmodel communication

### Step 2: Introduce Adapters

Use adapter pattern for gradual migration:

```cpp
// Old code:
WidgetsPresenter::registerView<MyWidget>();

// Transition code with adapter:
auto qtPresenter = new WidgetsPresenter();
auto adapter = new PresenterAdapter(nativePresenter);
ServiceRegistry::instance()->registerInterface<IPresenter>(adapter);
```

### Step 3: Implement Native Views

Create native implementations gradually:

```cpp
class MyNativeView : public INativeView {
    // Use your preferred UI framework
    // No Qt dependencies required
};
```

### Step 4: Update Build System

Configure conditional compilation:

```pro
# In your .pro file
CONFIG += native_interface

native_interface {
    DEFINES += QTMVVM_USE_NATIVE_INTERFACE
    # Don't link Qt Widgets if not needed
}
```

### Step 5: Testing

Test both interfaces during transition:

```cpp
// Unit test with mock view
class MockNativeView : public INativeView {
    // Easy to implement for testing
};

// Integration test with Qt widgets
class TestableQtView : public QWidget, public INativeView {
    // Can still use Qt if needed
};
```

## Best Practices

### 1. Design for Abstraction
- Program to the INativeView interface, not concrete implementations
- Keep business logic in ViewModels, not views
- Use dependency injection for presenters

### 2. Minimize Dependencies
- Keep native interfaces free of platform-specific types
- Use standard C++ types (std::string, std::map, etc.)
- Avoid tight coupling to specific UI frameworks

### 3. Maintain Compatibility
- Keep Qt implementations for backward compatibility
- Use adapters during transition periods
- Document which interfaces are Qt-free

### 4. Performance Considerations
- Adapters add minimal overhead
- Native interfaces can be more efficient than Qt's meta-object system
- Consider binary size when choosing between Qt and native

## Example: Console Application

A complete example of a Qt-free console application:

```cpp
#include <QtMvvmCore/nativeview.h>
#include <QtMvvmCore/nativepresenter.h>
#include <iostream>

class ConsoleView : public QtMvvm::INativeView {
    std::string m_title;
    bool m_visible = false;

public:
    void show() override {
        m_visible = true;
        std::cout << "=== " << m_title << " ===" << std::endl;
    }
    
    void hide() override {
        m_visible = false;
    }
    
    void setTitle(const std::string &title) override {
        m_title = title;
    }
    
    // Implement other INativeView methods...
};

// Use like any other QtMvvm view!
```

## Future Enhancements

Planned improvements to the native interface:

1. **More Native Types**: Replace remaining Qt types with C++ standard types
2. **Platform Adapters**: Pre-built adapters for popular UI frameworks
3. **Native Examples**: Complete sample applications without Qt UI
4. **Documentation**: Expanded guides for each use case

## Troubleshooting

### Issue: Adapter not working
**Solution**: Ensure proper lifetime management of adapted objects

### Issue: Type conversion errors
**Solution**: Use provided conversion helpers in PresenterAdapter

### Issue: Missing view registration
**Solution**: Register view factories before presenting viewmodels

## Resources

- See `examples/native/` for working examples
- Check `src/mvvmcore/nativeview.h` for complete interface
- Review `src/mvvmcore/presenteradapter.h` for Qt integration
- Examine `src/mvvmwidgets/qtwidgetadapter.h` for Qt widget adaptation

## Conclusion

The native interface architecture makes QtMvvm more flexible and portable while maintaining full backward compatibility with existing Qt-based applications. You can adopt it incrementally or use it for completely new projects targeting multiple platforms.
