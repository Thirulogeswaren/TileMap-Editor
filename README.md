# Level Editor

making a general tile map editor with some new features... (unnamed) editor

## Project Dependencies
- [Dear ImGui](https://github.com/ocornut/imgui)
- [SFML](https://github.com/SFML/SFML)
- [Native File Dialog](https://github.com/mlabbe/nativefiledialog)

## Building on Windows

download the latest version of [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/), use cmake and vcpkg package manager that comes with visual studio.

Now download the project [source](https://github.com/madnesly/LevelEditor/archive/refs/heads/main.zip) and extract them, then create a `vcpkg.json` file inside the project's folder and add the contents to that file given below:

```json
{
  "dependencies": [
    {
      "name": "imgui",
      "features": [
        "docking-experimental"
      ]
    },
    "imgui-sfml",
    "nativefiledialog"
  ]
}
```

open up Developer PowerShell for VS2022 and navigate to the directory which contains ``vcpkg.json`` file that you have created and run:

```shell
vcpkg x-update-baseline --add-initial-baseline
```
this adds the **builtin-baseline** to the manifest file, then run the following commands to build the project:

```shell
mkdir build
```

substitude ``%VCPKG_ROOT%`` with your vcpkg installation path, usually it will be under ``C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg\...`` (for vcpkg that comes with visual studio)

```shell
cmake -B build -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
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

Now download the project [source](https://github.com/madnesly/LevelEditor/archive/refs/heads/main.zip) and extract them by running ```unzip Level-Editor-main.zip```, then run the following commands to build the project:

```shell
cd Level-Editor-main
mkdir build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake 
cmake --build build
```
