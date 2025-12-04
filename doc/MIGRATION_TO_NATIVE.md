# Migration Guide: Moving to Native C++ Interfaces

This guide helps you migrate existing QtMvvm applications to use the new native C++ interfaces while maintaining backward compatibility.

## Why Migrate?

The native interface provides several benefits:

1. **Platform Independence**: UI code no longer depends on Qt types
2. **Framework Flexibility**: Use any C++ UI framework alongside QtMvvm
3. **Smaller Binaries**: Optional Qt dependencies reduce application size
4. **Better Testing**: Pure C++ interfaces are easier to mock and test
5. **Future-Proof**: Adapts to changing platform requirements

## Migration Strategies

### Strategy 1: Zero-Change (Fully Compatible)

Your existing code continues to work without any changes. The native interfaces are additive, not breaking.

```cpp
// This still works exactly as before
QtMvvm::WidgetsPresenter::registerView<MyView>();
// No changes required!
```

**When to use**: When you're satisfied with Qt Widgets and don't need cross-platform UI.

### Strategy 2: Gradual Adoption (Recommended)

Incrementally adopt native interfaces while keeping existing Qt code:

#### Step 1: Use Native Presenter (Drop-in Replacement)

```cpp
// Old code:
#include <QtMvvmWidgets/WidgetsPresenter>
QtMvvm::WidgetsPresenter::registerAsPresenter();

// New code (optional, provides same functionality):
#include <QtMvvmWidgets/NativeWidgetsPresenter>
QtMvvm::NativeWidgetsPresenter::registerAsPresenter();
```

**Benefits**: Start using native interfaces with zero code changes.

#### Step 2: Wrap Existing Views

Wrap your existing QWidget views to work with native interfaces:

```cpp
#include <QtMvvmWidgets/QtWidgetAdapter>

// Your existing widget
auto myWidget = new MyQtWidget(viewModel, parent);

// Wrap it for native interface
auto nativeView = new QtMvvm::QtWidgetAdapter(myWidget);

// Now accessible through INativeView interface
nativeView->show();
nativeView->setTitle("My App");
```

**Benefits**: Gradually introduce native interface usage patterns.

#### Step 3: Implement New Views with Native Interface

For new features, implement views using native interfaces:

```cpp
class MyNativeView : public QtMvvm::INativeView {
public:
    void show() override {
        // Use your preferred UI framework
        // No Qt dependencies required
    }
    
    // Implement other INativeView methods
};
```

**Benefits**: New code is framework-independent from the start.

### Strategy 3: Full Migration (Advanced)

Completely eliminate Qt dependencies from the UI layer:

#### Phase 1: Audit Dependencies

Identify where Qt types are used:
- Views using QWidget
- Signals/slots connections
- Qt containers (QVariantHash, QStringList, etc.)

#### Phase 2: Replace Qt Widgets

```cpp
// Before: Qt Widget
class MyView : public QWidget {
    Q_OBJECT
public:
    Q_INVOKABLE MyView(QtMvvm::ViewModel *vm, QWidget *parent = nullptr);
    // Qt-specific code
};

// After: Native View
class MyNativeView : public QtMvvm::INativeView {
public:
    MyNativeView(QtMvvm::ViewModel *vm, MyNativeView *parent = nullptr);
    // Framework-independent code using std::string, etc.
};
```

#### Phase 3: Update Build Configuration

```pro
# Optional: Remove Qt Widgets dependency
QT -= widgets

# Link only what you need
QT = core mvvmcore

# Your native UI framework
LIBS += -lmynativeui
```

**Benefits**: Complete platform independence and flexibility.

## Common Migration Patterns

### Pattern 1: View Registration

**Before:**
```cpp
QtMvvm::WidgetsPresenter::registerView<MyWidget>();
```

**After (with native interface):**
```cpp
auto factory = std::make_shared<QtMvvm::QtWidgetViewFactory>();
factory->registerWidgetCreator("MyViewModel", 
    [](QtMvvm::ViewModel *vm, QWidget *parent) {
        return new MyWidget(vm, parent);
    });
nativePresenter->registerViewFactory(factory);
```

### Pattern 2: Message Dialogs

**Before:**
```cpp
QtMvvm::information("Title", "Message");
```

**After (works the same way):**
```cpp
// No changes needed - works with both presenters!
QtMvvm::information("Title", "Message");
```

The adapter automatically converts between Qt and native formats.

### Pattern 3: Custom Dialogs

**Before:**
```cpp
MessageConfig config(MessageConfig::TypeMessageBox);
config.setTitle("My Dialog");
CoreApp::showDialog(config);
```

**After (with native config):**
```cpp
NativeMessageConfig config("MessageBox");
config.title = "My Dialog";
// Convert if needed
auto qtConfig = PresenterAdapter::convertMessageConfigFromNative(config);
CoreApp::showDialog(qtConfig);
```

### Pattern 4: View-ViewModel Binding

**Before:**
```cpp
class MyWidget : public QWidget {
    Q_OBJECT
    MyViewModel *_viewModel;
    
public:
    MyWidget(QtMvvm::ViewModel *vm, QWidget *parent) : QWidget(parent) {
        _viewModel = qobject_cast<MyViewModel*>(vm);
        connect(_viewModel, &MyViewModel::dataChanged, 
                this, &MyWidget::updateUI);
    }
};
```

**After (native approach):**
```cpp
class MyNativeView : public QtMvvm::INativeView {
    MyViewModel *_viewModel;
    
public:
    MyNativeView(QtMvvm::ViewModel *vm, MyNativeView *parent) {
        _viewModel = static_cast<MyViewModel*>(vm);
        // Use observers or callbacks instead of signals/slots
        _viewModel->onDataChanged([this]() { updateUI(); });
    }
};
```

## Backward Compatibility

### What Stays the Same?

1. **ViewModel API**: No changes to ViewModel interface
2. **Service Registry**: Works identically
3. **Core App**: Same API for showing views and dialogs
4. **Existing Examples**: All existing examples continue to work

### What's New?

1. **Native Interfaces**: Optional pure C++ abstractions
2. **Adapters**: Bridge between Qt and native worlds
3. **Multiple UI Frameworks**: Can mix Qt and native UIs

### Migration Checklist

- [ ] Audit current Qt dependencies in UI code
- [ ] Test existing application with NativeWidgetsPresenter
- [ ] Wrap selected views with QtWidgetAdapter
- [ ] Implement new views using INativeView
- [ ] Update build configuration as needed
- [ ] Add tests for native interfaces
- [ ] Document which views are Qt vs native

## Troubleshooting

### Issue: Compilation errors with std::string

**Problem**: Code expects QString but receives std::string

**Solution**: Use conversion helpers:
```cpp
QString qstr = QString::fromStdString(nativeStr);
std::string stdstr = qstr.toStdString();
```

### Issue: Views not found

**Problem**: Native presenter can't find views

**Solution**: Ensure view factories are registered:
```cpp
presenter->registerViewFactory(myFactory);
```

### Issue: Signals/slots not working

**Problem**: Native views don't support Qt signals

**Solution**: Use callbacks or observer pattern:
```cpp
// Instead of signals/slots
viewModel->registerObserver([this](const std::string &data) {
    // Handle update
});
```

## Performance Considerations

### Memory Usage
- **Native interfaces**: Minimal overhead, uses standard C++ types
- **Adapters**: Small wrapper objects, negligible overhead
- **Qt widgets**: Same as before when using QtWidgetAdapter

### Execution Speed
- **Direct native calls**: No conversion overhead
- **Adapted calls**: Minimal string conversion overhead
- **Mixed approach**: Overhead only at interface boundaries

### Binary Size
- **With Qt Widgets**: No size increase (same dependencies)
- **Native only**: Significant reduction (no Qt Widgets)
- **Hybrid**: Slightly larger (includes both)

## Best Practices

1. **Start Small**: Migrate one view at a time
2. **Use Adapters**: Keep Qt code working during transition
3. **Test Thoroughly**: Ensure both Qt and native paths work
4. **Document Changes**: Mark which code uses native interfaces
5. **Keep It Simple**: Don't over-engineer the abstraction

## Resources

- [Native Interface Guide](native_interface_guide.md) - Comprehensive reference
- [Examples](../examples/native/) - Working code samples
- [API Documentation](https://skycoder42.github.io/QtMvvm/) - Complete API reference

## Getting Help

If you encounter issues during migration:

1. Check the examples in `examples/native/`
2. Review the API documentation
3. Look for similar issues in the GitHub repository
4. Open a new issue with a minimal reproduction case

## Conclusion

Migration to native interfaces is optional and gradual. You can adopt as much or as little as makes sense for your application. The framework maintains full backward compatibility while opening new possibilities for cross-platform development.
