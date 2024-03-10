# TileMap Editor

making a general tile map editor ...

## Project Dependencies
- [Dear ImGui](https://github.com/ocornut/imgui)
- [SFML](https://github.com/SFML/SFML)
- [Native File Dialog](https://github.com/mlabbe/nativefiledialog)

## Building on Windows

To build the project, download:
  - [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) 
  - use cmake and vcpkg that comes with Visual Studio 2022

Open up Developer PowerShell for VS2022 and navigate to project source directory, then run:

```shell
vcpkg x-update-baseline --add-initial-baseline
```

this adds the **builtin-baseline** to the manifest file, then run the following commands to build the project:

```shell
cmake --preset win-release
cmake --build --preset Release
```

## Building on Linux 

make sure your linux distro have [cmake](https://cmake.org/) installed, if you are on debian or ubuntu based distribution run:

```shell
sudo apt install cmake
```

On Arch based distribution:

```shell
sudo pacman -S cmake
```

Download [vcpkg](https://vcpkg.io/en/getting-started) to install project dependencies, then run:

```shell
vcpkg install imgui-sfml nativefiledialog imgui[docking-experimental]
```

Now download the project [source](https://github.com/madnesly/LevelEditor/archive/refs/heads/main.zip) and extract them by running ```unzip [filename].zip```, then run the following commands to build the project:

```shell
cd Level-Editor-main
mkdir build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake 
cmake --build build
```
