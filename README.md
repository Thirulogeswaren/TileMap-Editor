# Tilemap Editor

To build the project, you will need
- CMake, ninja-build, and
- a `compiler` that supports the C++17 standard.
> [!NOTE]
> When working on Windows, Visual Studio usually comes with CMake and ninja. If not use the msvc installer to download CMake Tools, and make sure to use the `x64 Native Tools Command Prompt` provided by the Visual Studio.

## On Windows
Run these commands to generate the build files and then compile the project. After running them, you can find the `executable` under `bin`, which is located in the root directory.
``` shell
cmake --preset config-windows
cmake --build --preset Build-Windows
```

## On Linux
``` shell
cmake --preset config-linux
cmake --build --preset Build-Linux
```
