# n64brew-gamejam-2021

[API function Reference](https://framework64.readthedocs.io/en/latest/files.html)

### Requirements
1. Download and install the current version of [nodejs](https://nodejs.org/en/download/current/)
2. Ensure you have VS Code with the [Remote Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension installed.

### Environment Setup / Building
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