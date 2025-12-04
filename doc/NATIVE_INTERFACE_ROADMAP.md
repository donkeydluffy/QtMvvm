# Native Interface Roadmap - Additional Components

## Overview

This document outlines additional components in QtMvvm that could benefit from native C++ interface abstractions, beyond the initial implementation (INativeView, INativePresenter, and adapters).

## Current Implementation (Phase 1) ✅

**Completed:**
- `INativeView` - Pure C++ view interface
- `INativePresenter` - Pure C++ presenter interface
- `NativeMessageConfig` - Qt-independent message configuration
- `PresenterAdapter` - Bidirectional Qt ↔ Native bridge
- `QtWidgetAdapter` - QWidget integration with native interfaces

## Additional Components for Native Interface (Phase 2)

### 1. Data Binding System

**Current State:**
- File: `src/mvvmcore/binding.h`
- Depends on: `QObject`, `QMetaProperty`, `QMetaMethod`, `QMetaObject`
- Tightly coupled to Qt's meta-object system

**Proposed Native Interface:**
```cpp
namespace QtMvvm {

//! Native property interface
class INativeProperty {
public:
    virtual ~INativeProperty() = default;
    virtual std::string name() const = 0;
    virtual std::string typeName() const = 0;
    virtual void setValue(const std::string &value) = 0;
    virtual std::string getValue() const = 0;
};

//! Native binding interface
class INativeBinding {
public:
    virtual ~INativeBinding() = default;
    
    enum Direction {
        SingleInit,
        OneWayToView,
        OneWayToViewModel,
        TwoWay
    };
    
    virtual bool isValid() const = 0;
    virtual void unbind() = 0;
};

//! Factory for creating native bindings
class INativeBindingFactory {
public:
    virtual ~INativeBindingFactory() = default;
    
    virtual INativeBinding* createBinding(
        void* viewModel,
        const std::string &viewModelProperty,
        void* view,
        const std::string &viewProperty,
        INativeBinding::Direction direction
    ) = 0;
};

}
```

**Benefits:**
- Support property binding in non-Qt frameworks
- Use std::function for change callbacks
- Observer pattern implementation
- Cross-platform property observation

**Implementation Complexity:** Medium
**Priority:** High (directly affects user code)

### 2. Service Registry and Dependency Injection

**Current State:**
- File: `src/mvvmcore/serviceregistry.h`
- Depends on: `QObject`, `QMetaObject`, Qt's introspection
- Uses Qt's type system for service resolution

**Proposed Native Interface:**
```cpp
namespace QtMvvm {

//! Native service descriptor
struct NativeServiceDescriptor {
    std::string serviceId;
    std::function<void*()> factory;
    int destructionScope;
};

//! Native service registry interface
class INativeServiceRegistry {
public:
    virtual ~INativeServiceRegistry() = default;
    
    virtual void registerService(
        const std::string &serviceId,
        std::function<void*()> factory,
        int scope = 2
    ) = 0;
    
    virtual void* getService(const std::string &serviceId) = 0;
    
    virtual bool isRegistered(const std::string &serviceId) const = 0;
    
    virtual void injectDependencies(void* object) = 0;
};

//! Service registry adapter
class ServiceRegistryAdapter : public INativeServiceRegistry {
    // Bridges between Qt ServiceRegistry and native interface
};

}
```

**Benefits:**
- Dependency injection without QObject
- Type-safe service resolution with templates
- Standard C++ factory pattern
- Plugin system without Qt plugins

**Implementation Complexity:** High
**Priority:** Medium (advanced feature)

### 3. ViewModel Base Class

**Current State:**
- File: `src/mvvmcore/viewmodel.h`
- Inherits from: `QObject`
- Uses Qt signals/slots for notifications

**Proposed Native Interface:**
```cpp
namespace QtMvvm {

//! Native view model interface
class INativeViewModel {
public:
    virtual ~INativeViewModel() = default;
    
    // Lifecycle methods
    virtual void onInit(const std::map<std::string, std::string> &params) = 0;
    virtual void onResult(uint32_t requestCode, const std::string &result) = 0;
    
    // Result notification
    using ResultCallback = std::function<void(const std::string&)>;
    virtual void setResultCallback(ResultCallback callback) = 0;
    
    // Navigation
    virtual void showViewModel(
        const std::string &viewModelName,
        const std::map<std::string, std::string> &params
    ) = 0;
};

//! Native view model base implementation
class NativeViewModelBase : public INativeViewModel {
protected:
    ResultCallback m_resultCallback;
    // Standard C++ implementation
};

//! Qt ViewModel adapter
class QtViewModelAdapter : public ViewModel {
    // Makes INativeViewModel work as Qt ViewModel
};

}
```

**Benefits:**
- ViewModel without Qt dependencies
- Standard C++ callbacks instead of signals
- Portable business logic
- Easy to test with mocks

**Implementation Complexity:** High
**Priority:** High (core component)

### 4. Message System Enhancement

**Current State:**
- File: `src/mvvmcore/message.h`
- `NativeMessageConfig` already exists ✅
- `MessageResult` and `ProgressControl` still use QObject

**Proposed Enhancement:**
```cpp
namespace QtMvvm {

//! Native message result interface
class INativeMessageResult {
public:
    virtual ~INativeMessageResult() = default;
    
    virtual bool hasResult() const = 0;
    virtual std::string result() const = 0;
    virtual void setResult(const std::string &result) = 0;
    
    virtual void complete(int button) = 0;
    virtual void discard() = 0;
    
    using CompletionCallback = std::function<void(int button)>;
    virtual void setCompletionCallback(CompletionCallback callback) = 0;
};

//! Native progress control interface
class INativeProgressControl {
public:
    virtual ~INativeProgressControl() = default;
    
    virtual bool isIndeterminate() const = 0;
    virtual void setIndeterminate(bool indeterminate) = 0;
    
    virtual int minimum() const = 0;
    virtual void setMinimum(int minimum) = 0;
    
    virtual int maximum() const = 0;
    virtual void setMaximum(int maximum) = 0;
    
    virtual int progress() const = 0;
    virtual void setProgress(int progress) = 0;
    
    virtual void close() = 0;
    virtual void updateLabel(const std::string &text) = 0;
    
    using CancelCallback = std::function<void(int button)>;
    virtual void setCancelCallback(CancelCallback callback) = 0;
};

}
```

**Benefits:**
- Dialog results without Qt
- Progress tracking with callbacks
- Platform-independent dialogs

**Implementation Complexity:** Low
**Priority:** Medium (enhances existing work)

### 5. Settings System

**Current State:**
- Files: `settingssetup.h`, `isettingsaccessor.h`, `settingsviewmodel.h`
- Depends on: Qt's property system, QSettings

**Proposed Native Interface:**
```cpp
namespace QtMvvm {

//! Native settings accessor interface
class INativeSettingsAccessor {
public:
    virtual ~INativeSettingsAccessor() = default;
    
    virtual bool contains(const std::string &key) const = 0;
    virtual std::string value(const std::string &key, const std::string &defaultValue = {}) const = 0;
    virtual void setValue(const std::string &key, const std::string &value) = 0;
    virtual void remove(const std::string &key) = 0;
    virtual void sync() = 0;
};

//! Native settings entry
struct NativeSettingsEntry {
    std::string key;
    std::string title;
    std::string type;
    std::string defaultValue;
    std::map<std::string, std::string> properties;
};

}
```

**Benefits:**
- Settings without QSettings
- Use JSON, INI, or custom formats
- Platform-specific implementations

**Implementation Complexity:** Medium
**Priority:** Low (specialized feature)

## Implementation Priority

### Phase 2A (High Priority)
1. **ViewModel Interface** - Core component affecting all applications
2. **Data Binding** - Essential for MVVM pattern

### Phase 2B (Medium Priority)
3. **Message Result/Progress** - Completes message system
4. **Service Registry** - Advanced DI features

### Phase 2C (Low Priority)
5. **Settings System** - Specialized use cases

## Design Principles

### Consistency
- Follow same adapter pattern as Phase 1
- Maintain 100% backward compatibility
- Use std:: types consistently

### Flexibility
- Support both Qt and native implementations
- Allow mixing Qt and native components
- Provide clear migration paths

### Performance
- Minimal overhead from adapters
- Efficient type conversions
- No runtime penalties

## Migration Strategy

### For ViewModel
```cpp
// Option 1: Continue using Qt ViewModel (unchanged)
class MyViewModel : public QtMvvm::ViewModel {
    Q_OBJECT
    // Qt signals/slots
};

// Option 2: Use native ViewModel
class MyNativeViewModel : public QtMvvm::NativeViewModelBase {
    // Standard C++ callbacks
};

// Option 3: Adapter for gradual migration
auto qtViewModel = new MyViewModel();
auto adapter = new QtMvvm::QtViewModelAdapter(qtViewModel);
```

### For Data Binding
```cpp
// Option 1: Qt binding (unchanged)
QtMvvm::bind(viewModel, "property", view, "property");

// Option 2: Native binding
auto binding = nativeBindingFactory->createBinding(
    viewModel, "property",
    view, "property",
    INativeBinding::TwoWay
);
```

## Backward Compatibility

All enhancements will:
- ✅ Keep existing APIs unchanged
- ✅ Add new native interfaces alongside Qt versions
- ✅ Use adapters for interoperability
- ✅ Require no changes to existing code

## Testing Strategy

### Unit Tests
- Test each native interface independently
- Verify adapter conversions
- Mock implementations for testing

### Integration Tests
- Mix Qt and native components
- Test adapter boundaries
- Verify performance

### Migration Tests
- Test gradual migration scenarios
- Verify backward compatibility
- Document migration patterns

## Documentation Requirements

For each new interface:
1. API reference documentation
2. Migration guide
3. Working examples
4. Performance characteristics

## Timeline Estimate

- **Phase 2A** (ViewModel + Binding): 2-3 weeks
- **Phase 2B** (Messages + Services): 2-3 weeks
- **Phase 2C** (Settings): 1 week
- **Testing & Documentation**: 1-2 weeks

**Total**: 6-9 weeks for complete native interface coverage

## Questions for Consideration

1. Should we provide a pure C++ ViewModel base, or only adapters?
2. How should native bindings handle type conversions?
3. Should service registry support compile-time type safety?
4. What's the minimal set of features needed for ViewModel?
5. Should we support Qt and native bindings simultaneously?

## Conclusion

The current Phase 1 implementation provides a solid foundation for native interfaces in QtMvvm. Phase 2 would extend this to cover all major components, enabling completely Qt-free applications while maintaining full backward compatibility.

The adapter pattern has proven effective and should continue to be the primary strategy for adding native interface support to remaining components.

---

*This roadmap is subject to community feedback and prioritization based on actual use cases and requirements.*
