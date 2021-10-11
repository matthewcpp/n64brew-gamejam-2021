# n64brew-gamejam-2021

[API function Reference](https://framework64.readthedocs.io/en/latest/files.html)
[API Wiki and Additional Info](https://github.com/matthewcpp/framework64/wiki)

### Requirements
1. Download and install the current version of [nodejs](https://nodejs.org/en/download/current/)
2. Ensure you have VS Code with the [Remote Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension installed.

### Environment Setup / Building for N64
Begin by ensuring the docker is currently running on your system.
run from git bash / powershell:
```shell
git clone --recursive https://github.com/matthewcpp/n64brew-gamejam-2021.git
cd n64brew-gamejam-2021
npm install
npm run prepare-n64-assets
```
Now open the `n64brew-gamejam-2021` in VsCode. 
If you receive a prompt indication that `.devcontainer.json` is present, click the button to reopen the project in a container.
Once the folder is open in the container, launch a new terminal from within VSCode.
At this point, it would be a good Idea to install the C/C++ developer extensions into the container.

```shell
cd build_n64
cmake -DFW64_PLATFORM_N64=ON ..
make
```

your rom will be located at `build_n64/bin/jam.n64`

After the initial call to cmake you just need to run `make` to rebuild the ROM.

### Environment Set / Building for Desktop
Note: This has been tested with Visual Studio 2019 Community Edition

#### Configure vcpkg
Desktop build uses [vcpkg](https://vcpkg.io) for dependency management.  This configuration step will only need to be done once.
run in git bash or powershell: `git clone https://github.com/microsoft/vcpkg.git`
set environment variable `VCPKG_DIR` to the root of the repo you just cloned.  Note if VSCode is open you will probably need to relaunch it after changing this variable.
Open the directory and double click `bootstrap-vcpkg.bat`.

#### Build Assets
In a terminal run the following commands from the root repo directory:
npm run prepare-desktop-assets
npm run prepare-desktop-shaders

#### Build the project
Open the project folder in VSCode, however when it asks you if you want to reopen the folder in the container just close the popup.
Ensure the [Cmake Tools plugin from Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) is installed into vs code.
Hit CTL+ Shift P and type CMAKE: Select a Kit or you can use the selector from the bottom status bar.  From The popup pick one ending in - x86.  For Example: `Visual Studio Community Release - x86`
Hit CTRL + Shift P and Type CMake: Build.

Before running you will need to run: `npm run windows-desktop-copy-ogg-libs dbg x86`
note if you switch your build type to release you will need to specify rel, likewise if you use a 64 bit kit you will need to specify x64
now you can run and debug the game from the quick run/debug buttons in the status bar

In the bottom right of the IDE status bar, ensure that the `Desktop` Configuration is selected.