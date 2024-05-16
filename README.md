# D.E.A.R.
D.E.A.R. (**D**ata **E**xtractor **a**nd **R**epacker) is a tool developed for Luigi's Mansion 2 to extract data from DATA-DICT bundles, as well as the possibility to repack it.

This tool is very much still in early development. For questions please join the LM2 modding [Discord](https://discord.gg/FQ5S5c46), and if you are curious about the inner workings of the .data and .dict files found in LM2's `romfs` please take a look at the [LM2 Modding Wiki](https://lm2.lmhack.net/index.php/Main_Page).

**As this tool is by no means complete, bugs can occur. If that happens please open up an issue on Github's issue tracker.**

## Download
You can download D.E.A.R for Windows or Linux under the releases tab.

An OSX version should be possible to build, but hasn't been tested yet.

## Building
D.E.A.R. makes use of the [Texus' Graphical User Interface](https://github.com/texus/TGUI) library with [SFML](https://github.com/SFML/SFML) as the backend. Furthermore the [zlib compression](https://github.com/madler/zlib) library is used to handle compressed .data files. These are libraries that can be provided by yourself, or if they aren't installed on your system are automatically build through CMake.

### Linux
On Linux simply configure the project with this command:
```
cmake -S . -B build
```
And then build with:
```
cmake --build build
```

## Windows
Visual Studio should be able to configure CMake by default when starting the IDE. After that you only need to click `Build All` under `Build->Build All`.

As Windows is not my primary machine, building on Windows might not work in different circumstances. If that is the case, please come in contact with me.

## Credits
D.E.A.R uses different libaries to make development easier and more focused on the task at hand. These libraries are listed below.
- Protinon - https://github.com/Protinon/Binary-Reader
- texus (Bruno Van de Velde) - https://github.com/texus/TGUI
- SFML - https://github.com/SFML/SFML
- madler (Mark Adler) - https://github.com/madler/zlib
