# Adapter Architecture Guide

This guide explains the design principles behind QtMvvm's adapter architecture and answers common questions about module organization.

## Module Separation: mvvmqtadapter vs mvvmwidgets

### Question: Should these modules be combined or kept separate?

**Answer: Keep them SEPARATE**

### Rationale

**1. Different Responsibilities**
- **mvvmqtadapter**: Provides Qt Core integration (QObject, signals/slots, QtCore types)
- **mvvmwidgets**: Provides Qt Widgets-specific features (QWidget hierarchy, widget adapters)

**2. Flexibility**
- QML-only applications can use `mvvmqtadapter` without pulling in QtWidgets
- Console Qt applications don't need widgets
- Reduces binary size for apps that don't need widgets

**3. Single Responsibility Principle**
- mvvmqtadapter: "Bridge mvvmcore to Qt framework"
- mvvmwidgets: "Provide widget-specific MVVM features"

**4. Parallel with Other Frameworks**
Each UI framework gets an appropriate adapter structure:
- Some frameworks need single adapter (ImGui, Android)
- Qt needs two layers (Core vs Widgets) due to its architecture

## Framework Comparison Table

| Framework | Core Adapter | UI Layer | Separation Reason |
|-----------|-------------|----------|-------------------|
| **Qt** | mvvmqtadapter | mvvmwidgets | QML vs Widgets distinction |
| **ImGui** | imgui_adapter | - | Immediate mode (no separate UI layer) |
| **Android** | android_adapter | - | Activity-based (single layer sufficient) |
| **wxWidgets** | wx_adapter | - | Widgets integrated in core |

## Architecture Patterns

### Pattern 1: Single-Layer Adapter (ImGui, Android)

```
Application
     ↓
Framework Adapter (implements INativeView)
     ↓
mvvmcore (Pure C++)
```

**When to use:**
- Framework has unified architecture
- No distinction between "core" and "UI" layers
- Immediate-mode or simple retained-mode frameworks

### Pattern 2: Two-Layer Adapter (Qt)

```
Application
     ↓
UI-Specific Layer (mvvmwidgets)
     ↓
Core Adapter Layer (mvvmqtadapter)
     ↓
mvvmcore (Pure C++)
```

**When to use:**
- Framework has distinct core and UI components
- Need to support multiple UI paradigms (QML vs Widgets)
- Want to minimize dependencies for non-UI applications

## Creating New Adapters

### Step 1: Identify Framework Architecture

Ask these questions:
1. Does the framework have a clear core/UI separation?
2. Are there multiple UI paradigms (like QML vs Widgets)?
3. What are the minimal dependencies needed?

### Step 2: Choose Pattern

- **Single-layer** if framework is unified
- **Two-layer** if framework has distinct core/UI separation

### Step 3: Implement INativeView

```cpp
class MyFrameworkViewAdapter : public INativeView {
public:
    void show() override {
        // Call framework's show method
    }
    
    void setTitle(const std::string &title) override {
        // Convert std::string to framework's string type
        // Set window/view title
    }
    
    // Implement other INativeView methods...
};
```

### Step 4: Create Presenter (if needed)

```cpp
class MyFrameworkPresenter : public INativePresenter {
public:
    void present(INativeViewModel *viewModel,
                 const std::map<std::string, std::string> &params) override {
        // Create view for this framework
        // Connect view to viewModel
        // Show view
    }
};
```

### Step 5: Handle Framework-Specific Lifecycle

- Window/Activity creation and destruction
- Event loops
- Resource management

## Examples by Framework

### ImGui Example

**Single-layer adapter** because ImGui is immediate-mode:

```cpp
// examples/imgui_adapter/
class ImGuiViewAdapter : public INativeView {
    void render() {
        ImGui::Begin("My View");
        // Render UI based on ViewModel state
        ImGui::End();
    }
};
```

### Android Example

**Single-layer adapter** using JNI for Java ↔ C++ bridge:

```cpp
// examples/android_adapter/
class AndroidViewAdapter : public INativeView {
    JNIEnv *env;
    jobject activity;
    
    void show() override {
        // Call Java method via JNI
        jmethodID method = env->GetMethodID(..., "show", "()V");
        env->CallVoidMethod(activity, method);
    }
};
```

### Qt Example

**Two-layer adapter** for flexibility:

```cpp
// src/mvvmqtadapter/ - Core Qt bridge
class QtViewModelAdapter : public QObject {
    Q_OBJECT
    // Bridges INativeViewModel to QObject signals/slots
};

// src/mvvmwidgets/ - Widget-specific
class QtWidgetAdapter : public QWidget, public INativeView {
    // Adapts QWidget to INativeView interface
};
```

## Dependency Graph Examples

### Minimal Qt Core Application
```
MyQtCoreApp
     ↓
mvvmqtadapter (Qt Core only)
     ↓
mvvmcore (Pure C++)
```

### Full Qt Widgets Application
```
MyQtWidgetsApp
     ↓
mvvmwidgets (Qt Widgets)
     ↓
mvvmqtadapter (Qt Core)
     ↓
mvvmcore (Pure C++)
```

### ImGui Application
```
MyImGuiApp
     ↓
imgui_adapter
     ↓
mvvmcore (Pure C++)
```

### Android Application
```
Android App (Java)
     ↓ (JNI)
android_adapter (C++)
     ↓
mvvmcore (Pure C++)
```

## Benefits of This Architecture

1. **Framework Independence**: mvvmcore has zero dependencies
2. **Flexibility**: Use only what you need
3. **Portability**: ViewModels work everywhere
4. **Testability**: Easy to mock adapters
5. **Maintainability**: Clear separation of concerns

## Migration Guide

### From Monolithic to Layered

**Before:**
```
MyApp → FrameworkSpecificMVVM
```

**After:**
```
MyApp → FrameworkAdapter → mvvmcore (portable)
```

### Existing Qt Applications

No changes needed! The two-layer architecture maintains 100% backward compatibility:

```cpp
// This still works:
#include <QtMvvmCore/ViewModel>
#include <QtMvvmWidgets/WidgetsPresenter>

class MyViewModel : public QtMvvm::ViewModel {
    // Unchanged
};
```

## Conclusion

The adapter architecture makes mvvmcore truly framework-independent while providing optimal integration for each specific UI framework. The decision to keep mvvmqtadapter and mvvmwidgets separate follows sound software engineering principles and provides maximum flexibility for Qt developers.

For other frameworks, choose single or two-layer adapters based on the framework's own architecture.
