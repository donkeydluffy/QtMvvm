# Native C++ Interface Example

This directory contains examples demonstrating how to use QtMvvm with pure C++ interfaces, without Qt dependencies in the UI layer.

## Overview

The native interface allows you to:
- Use QtMvvm's MVVM architecture with any C++ UI framework
- Implement custom views without depending on Qt Widgets or QML
- Maintain clean separation between business logic and UI implementation
- Easily port applications to different platforms or UI frameworks

## Key Concepts

### INativeView
Pure C++ interface for views, providing basic operations like show(), hide(), close(), etc.

### INativePresenter
Pure C++ presenter interface that manages view lifecycle and presentation without Qt dependencies.

### INativeViewModel (NEW - Phase 2A) ✅
Pure C++ ViewModel interface without QObject dependency. Uses std::function callbacks instead of Qt signals.

### INativeBinding (NEW - Phase 2A) ✅
Pure C++ data binding interface without Qt meta-object system. Enables two-way property synchronization.

### ViewModelAdapter (NEW - Phase 2A) ✅
Bridges between Qt ViewModel and INativeViewModel, enabling interoperability.

### INativeMessageResult (NEW - Phase 2B) ✅
Pure C++ interface for message dialog results without QObject dependency. Uses callbacks for completion notification.

### INativeProgressControl (NEW - Phase 2B) ✅
Pure C++ interface for progress tracking without Qt. Supports indeterminate and determinate modes.

### PresenterAdapter
Bridges between Qt-based IPresenter and pure C++ INativePresenter, allowing gradual migration.

### QtWidgetAdapter
Adapter that makes QWidget compatible with INativeView, enabling Qt widgets to work with native interfaces.

## Example Structure

- **simple_native_example.cpp** - A minimal example showing how to implement a native view and presenter
- **integration_example.cpp** - Shows how to mix Qt and native views in the same application
- **viewmodel_binding_example.cpp** - Demonstrates native ViewModel and data binding without Qt dependencies
- **message_example.cpp** - Shows native message result and progress control without Qt dependencies

## Key Features (Phase 2A)

### INativeViewModel
Pure C++ ViewModel interface that works with both Qt and native interfaces.

### INativeBinding
Data binding system using callbacks instead of Qt signals/slots.

### Complete Examples
The examples now demonstrate fully Qt-independent business logic using native interfaces.

## Building

To build the standalone examples (no Qt required for business logic):
```bash
g++ -std=c++14 simple_native_example.cpp -o simple_example
g++ -std=c++14 viewmodel_binding_example.cpp -o viewmodel_example
g++ -std=c++14 message_example.cpp -o message_example -pthread
./simple_example
./viewmodel_example
./message_example
```

For Qt integration examples, use qmake as usual.

## Migration Path

1. Start using INativeView and INativePresenter interfaces
2. Use PresenterAdapter to bridge to existing Qt code
3. Gradually replace Qt-specific views with native implementations
4. Eventually remove Qt dependencies from your UI layer entirely
