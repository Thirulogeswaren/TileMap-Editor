# Map Editor

making a 2D level editor and generator tool . . .

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

open up Developer PowerShell for VS2022 and navigate to the directory which contains ``vcpkg.json`` file that you have created and run the following command:

```shell
vcpkg x-update-baseline --add-initial-baseline
```
this adds the **builtin-baseline** to the manifest file. Next run ```vcpkg install``` to install project dependencies, this will create ```vcpkg_installed``` folder and installs them there.

Run the following commands:

```shell
mkdir build
```

substitude ``%VCPKG_ROOT%`` with your vcpkg installation path, usually it will be under ``C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg\...``

```shell
cmake -B build -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
cmake --build build --config Release
```