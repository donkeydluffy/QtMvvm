# ImGui Adapter Example

This example demonstrates how to use the pure C++ `mvvmcore` framework with ImGui, a popular immediate-mode GUI library for C++.

## What This Demonstrates

- **Framework Independence**: The same `mvvmcore` interfaces work with ImGui (immediate-mode) just as they work with Qt (retained-mode)
- **Pure C++**: No Qt dependencies in business logic
- **Cross-Platform**: ImGui runs on Windows, Linux, macOS, and more
- **Simple Integration**: Only ~200 lines of adapter code needed

## Architecture

```
ImGui Application
      ↓
ImGuiPresenter (manages ImGui window lifecycle)
      ↓
ImGuiViewAdapter (implements INativeView interface)
      ↓
mvvmcore (Pure C++ ViewModel)
```

## Features Demonstrated

1. **Property Binding**: ViewModel properties automatically update ImGui widgets
2. **Commands**: ImGui buttons call ViewModel methods
3. **Two-Way Binding**: ImGui input changes ViewModel state
4. **Real-Time Updates**: Counter increments and UI reflects changes instantly

## Building

### Prerequisites

- CMake 3.16+
- C++14 compiler
- ImGui library
- GLFW3
- OpenGL

### Build Steps

```bash
cd examples/imgui_adapter
mkdir build && cd build
cmake ..
cmake --build .
./imgui_example
```

## Key Insights

1. **No Qt Required**: The ViewModel has zero Qt dependencies
2. **Same Interface**: Uses the exact same `INativeView`, `INativeViewModel` interfaces as Qt examples
3. **Portable Business Logic**: The ViewModel can be reused in Qt, Android, or any other UI framework

## See Also

- `examples/native_widget_integration/` - Same pattern with Qt Widgets
- `examples/android_adapter/` - Same pattern with Android/Java
- `doc/ADAPTER_ARCHITECTURE_GUIDE.md` - Creating adapters guide
