# Building QtMvvm with CMake

This document describes how to build QtMvvm using CMake instead of qmake.

## Prerequisites

- CMake 3.16 or later
- Qt 5.15 or Qt 6.x
- C++14 compatible compiler

## Building

### Standard build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### With custom Qt installation

```bash
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64 ..
cmake --build .
```

### Build options

- `BUILD_EXAMPLES` - Build example applications (default: ON)
- `BUILD_TESTS` - Build tests (default: OFF)
- `BUILD_DOCS` - Build documentation (default: OFF)

Example:

```bash
cmake -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=ON ..
```

## Installation

```bash
cmake --build . --target install
```

Or with custom prefix:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
cmake --build .
cmake --build . --target install
```

## Using QtMvvm in your CMake project

After installation, you can use QtMvvm in your CMake project:

```cmake
find_package(QtMvvm REQUIRED)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE QtMvvm::Core QtMvvm::Widgets)
```

## Building the Native Widget Integration Example

The native widget integration example demonstrates using pure C++ ViewModels with Qt Widgets:

```bash
cd build
cmake ..
cmake --build .

# Run the example
./examples/native_widget_integration/NativeWidgetIntegration
```

## Comparison with qmake

### qmake (old):
```bash
qmake
make
```

### CMake (new):
```bash
cmake -B build
cmake --build build
```

## Migration Notes

- CMake build produces the same libraries as qmake
- All header files are installed in the same locations
- Binary compatibility is maintained
- Both build systems can coexist during transition period

## Troubleshooting

### Qt not found

Ensure Qt is in your PATH or set CMAKE_PREFIX_PATH:

```bash
export CMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
```

### Build errors

Clean the build directory and try again:

```bash
rm -rf build
mkdir build
cd build
cmake ..
```

### Example won't run

Make sure the Qt libraries are in your library path:

```bash
# Linux
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/Qt/lib

# macOS
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/path/to/Qt/lib

# Windows
set PATH=%PATH%;C:\path\to\Qt\bin
```
