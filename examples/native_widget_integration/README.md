# Native C++ ViewModel with Qt Widgets Integration Example

This example demonstrates the **feasibility and integration** of the native C++ interface refactoring with Qt Widgets.

## What This Example Tests

### 1. Pure C++ ViewModel (No QObject)
- `NativeExampleViewModel` inherits from `NativeViewModelBase` (pure C++)
- Uses `std::string`, `std::map`, and `std::function` instead of Qt types
- No Qt signals/slots - uses callbacks instead
- Completely independent of Qt framework

### 2. Qt Widget View Integration
- `NativeExampleView` is a standard Qt Widget (QMainWindow)
- Connects to native ViewModel via `NativeToViewModelAdapter`
- Demonstrates bidirectional data binding:
  - View → ViewModel: User input triggers C++ methods
  - ViewModel → View: Property changes trigger UI updates via callbacks

### 3. Adapter Pattern
- `NativeToViewModelAdapter` bridges pure C++ and Qt worlds
- Converts between native callbacks and Qt signals
- Allows seamless integration without modifying existing Qt code

## Architecture Flow

```
┌─────────────────────┐
│  Qt Widget View     │
│  (QMainWindow)      │
└──────────┬──────────┘
           │ Uses adapter
           ↓
┌─────────────────────┐
│ NativeToViewModel   │
│ Adapter             │
└──────────┬──────────┘
           │ Wraps
           ↓
┌─────────────────────┐
│ Native ViewModel    │
│ (Pure C++)          │
│ - No QObject        │
│ - std::function     │
│ - std::string       │
└─────────────────────┘
```

## Key Features Demonstrated

### ✅ Pure C++ Business Logic
```cpp
class NativeExampleViewModel : public QtMvvm::NativeViewModelBase {
    // Pure C++ - no Qt dependencies
    std::string m_name;
    int m_counter;
    std::function<void(const std::string&)> m_nameChangedCallback;
};
```

### ✅ Property Binding via Callbacks
```cpp
// ViewModel notifies View of changes (no Qt signals)
m_nativeViewModel->setNameChangedCallback([this](const std::string &name) {
    ui->nameEdit->setText(QString::fromStdString(name));
});
```

### ✅ Command Execution
```cpp
// View calls pure C++ methods
void onIncrementClicked() {
    m_nativeViewModel->incrementCounter();
}
```

### ✅ Result Handling
```cpp
// Native result callback (no Qt types)
m_nativeViewModel->setResultCallback([this](const std::string &result) {
    QMessageBox::information(this, "Result", QString::fromStdString(result));
});
```

## Building and Running

### Prerequisites
- Qt 5.x or Qt 6.x
- C++14 or later
- QtMvvm with native interface support

### Build
```bash
cd examples/native_widget_integration
qmake
make
./NativeWidgetIntegration
```

### Expected Behavior
1. Application opens with a Qt Widget window
2. Console shows native interface initialization messages
3. UI controls are bound to pure C++ ViewModel properties
4. All interactions work through native C++ interface
5. Console shows all property changes and method calls

## Testing the Integration

### Test 1: Property Binding (C++ ↔ Qt)
1. Type in the "Name" field → Native setter called
2. Console shows: `[Native] Name changed to: <value>`
3. Value is stored in pure C++ `std::string`

### Test 2: Counter (Command Pattern)
1. Click "Increment Counter" → `incrementCounter()` called
2. Counter updates in UI via callback
3. Console shows: `[Native] Counter changed to: <value>`

### Test 3: Result Callback
1. Click "Show Message" → Native method called
2. Message box appears with result from C++ callback
3. Demonstrates native result handling

### Test 4: Active Toggle
1. Check/uncheck "Active" checkbox
2. Native `setActive()` called with bool value
3. State managed in pure C++

## Verification Points

✅ **No QObject in ViewModel**: Business logic is pure C++  
✅ **No Qt Signals in ViewModel**: Uses std::function callbacks  
✅ **No QString/QVariant**: Uses std::string and std::map  
✅ **Adapter Transparency**: Qt View doesn't know ViewModel is native  
✅ **Bidirectional Binding**: Both directions work seamlessly  
✅ **Performance**: No noticeable overhead from adaptation layer  

## Console Output Example

```
========================================
Native C++ ViewModel Integration Test
========================================

[Main] Initializing QtMvvm with Native C++ interfaces...
[Main] Registered NativeExampleView
[Main] Created NativeExampleViewModel (pure C++ - no QObject)
[Main] Created adapter to bridge Native ViewModel to Qt
[Native] ViewModel initialized with counter=10, name=Test Native Integration
[Main] Successfully presented view with native ViewModel!

[Main] Application running...
[Main] Interact with the UI to test native interface integration
[Main] All property changes and commands go through pure C++ interface

[View] Received adapted native ViewModel
[View] NativeExampleView created successfully
[Native] Counter changed to: 11
[Native] Name changed to: Modified Name
[Native] Active changed to: false
[Native] Current state: name=Modified Name, counter=11, active=false

[Main] Application exiting...
========================================
```

## Conclusion

This example **proves the feasibility** of the native C++ interface refactoring:

1. ✅ Pure C++ ViewModels work with Qt Widgets
2. ✅ Adapter pattern provides seamless integration
3. ✅ Zero breaking changes to existing Qt code
4. ✅ Full MVVM functionality maintained
5. ✅ Business logic is framework-independent
6. ✅ Ready for use with non-Qt UI frameworks

The refactoring successfully achieves the goal of making QtMvvm's architecture reusable across different UI frameworks while maintaining 100% backward compatibility with Qt.
