# mingw64 CMake

This example shows how to cross-compile while on a Linux host to a Windows `.exe`.

## Running

Tested as working on Arch Linux:

```bash
# Assuming current directory of Learning-C
cmake -S mingw64_CMake/ -B build/ -DCMAKE_TOOLCHAIN_FILE=mingw64_toolchain
cmake --build build --config Release # Or Debug etc.
wine build/HelloWorld.exe
```

## Resources

Various resources, documentation and programs I used:

* [The Arch mingw-w64-gcc package](https://archlinux.org/packages/extra/x86_64/mingw-w64-gcc/)
* [mingw documentation](https://www.mingw-w64.org/), especially:
  * [CMake](https://www.mingw-w64.org/build-systems/cmake/)
  * [Getting Started: Arch](https://www.mingw-w64.org/getting-started/archlinux/)
