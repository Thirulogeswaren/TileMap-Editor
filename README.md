# Tilemap Editor

To build the project, you will need
- CMake, ninja-build, and
- a `compiler` that supports the C++17 standard.
> [!NOTE]
> when working on Windows, Visual Studio usually comes with CMake and ninja.

## On Linux
Run these commands to generate the build files and then compile the project. After running them, you can find the `executable` under `bin`, which is located in the root directory.
``` shell
cmake --preset linux-build
cmake --build --preset Linux-Build
```
