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

### PresenterAdapter
Bridges between Qt-based IPresenter and pure C++ INativePresenter, allowing gradual migration.

### QtWidgetAdapter
Adapter that makes QWidget compatible with INativeView, enabling Qt widgets to work with native interfaces.

## Example Structure

- **simple_native_example.cpp** - A minimal example showing how to implement a native view and presenter
- **native_viewmodel.h/cpp** - Example ViewModel that works with both Qt and native interfaces
- **console_view.h/cpp** - A simple console-based view implementation

## Building

These examples demonstrate the concept but may require Qt for the ViewModel base class.
For a truly Qt-independent version, you would need to implement your own ViewModel base
that doesn't depend on QObject.

## Migration Path

1. Start using INativeView and INativePresenter interfaces
2. Use PresenterAdapter to bridge to existing Qt code
3. Gradually replace Qt-specific views with native implementations
4. Eventually remove Qt dependencies from your UI layer entirely
