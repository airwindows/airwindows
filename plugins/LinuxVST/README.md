# Airwindows open-source VST plugins for Linux


> **NB!** There are official Linux builds of airwindows plugins available
> ([read more](http://www.airwindows.com/airwindows-linux/)). The purpose of
> this repository now is to experiment with build configuration and, probably,
> cross-compilation. I’d suggest to use official website to download Linux VSTs.

This repository contains a CMake project that builds Airwindows plugins for
Linux. Thanks to the Patreon supporters, Chris now publishes some of his plugins
under the MIT license on his GitHub, thus making this possible. Please, consider
supporting him. More info:

1. <https://patreon.com/airwindows>
2. <https://github.com/airwindows/airwindows>
3. <http://airwindows.com>

This CMake project is created by Eugene Cherny, whose work with CMake and scripting
allowed for every Airwindows plugin to be conveniently ported to LinuxVST, and
allowed for Linux as a platform to be targeted from now on. Thank you, ech2!
1. <https://github.com/ech2>
2. <http://oscii.ru>


## Building

Install CMake from your package manager:

- Debian / Ubuntu: `apt install cmake`
- Fedora / CentOS: `dnf install cmake`
- Arch / Manjaro: `pacman -S cmake`

You need to grab the VST SDK yourself, because the Steinberg’s license doesn’t
allow to distribute their SDK with licenses other than GPL.
You can get it at:[https://www.steinberg.net/en/company/developers.html](https://www.steinberg.net/en/company/developers.html).

Unzip the VST SDK into a directory called vstsdk3610_11_06_2018_build_37, then:
```
git clone https://github.com/airwindows/airwindows.git
unzip vstsdk3610_11_06_2018_build_37.zip
cp -r vstsdk3610_11_06_2018_build_37/VST2_SDK/pluginterfaces airwindows/plugins/LinuxVST/include/vstsdk/pluginterfaces
cp -r vstsdk3610_11_06_2018_build_37/VST2_SDK/public.sdk/source/vst2.x/* airwindows/plugins/LinuxVST/include/vstsdk/
```

Afterwards, the structure of the `include/vsdsdk` directory should be as follows:

```
include/vstsdk/
├── pluginterfaces
│   └── vst2.x
│       ├── aeffect.h
│       ├── aeffectx.h
│       └── vstfxstore.h
├── aeffeditor.h
├── audioeffect.cpp
├── audioeffect.h
├── audioeffectx.cpp
├── audioeffectx.h
├── CMakeLists.txt
└── vstplugmain.cpp
```


Then, build and install:
```
cd airwindows/plugins/LinuxVST/
cmake
make
mkdir -p /usr/local/lib/lxvst
cp *.so /usr/local/lib/lxvst
```

## Adding new plugins

1. Create a new directory `PluginName` in the `src` dir.
2. Copy the plugin’s `.cpp` and `.h` files to it.
3. Add line `add_airwindows_plugin(PluginName)` to the root `CMakeLists.txt`.
   This will create a new target `PluginName` with all the sources copied, as
   well as include VST-related files.
4. Build the project.

## What’s needed

- [X] Check the VS / Xcode projects for some extra compilation flags and add it
    to this project.
- [ ] Try to make this build cross-platform and generate Xcode and VS projects.
- [ ] Automatic builds for different platforms.
