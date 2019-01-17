# Enumivo.CDT (Contract Development Toolkit)
## Version : 1.5.0

Enumivo.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the Enumivo platform.  In addition to being a general purpose WebAssembly toolchain, [Enumivo](https://github.com/enumivo/enumivo) specific optimizations are available to support building Enumivo smart contracts.  This new toolchain is built around [Clang 7](https://github.com/enumivo/llvm), which means that Enumivo.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

## Important!
Enumivo.CDT Version 1.3.x introduced quite a few breaking changes.  To have binary releases we needed to remove the concept of a core symbol from Enumivo.CDT. This meant drastic changes to symbol, asset and other types/functions that were connected to them. Since these changes would be disruptive, we decided to add as many disruptive changes needed for future contract writing, so that disruption should only occur once. Please read the **_Differences between Version 1.2.x and Version 1.3.x_** section of this readme.

**If you have previously installed Enumivo.CDT, please run the `uninstall` script (it is in the directory where you cloned Enumivo.CDT) before downloading and using the binary releases.**

### Guided Installation (Building from Scratch)
```sh
$ git clone --recursive https://github.com/enumivo/enumivo.cdt
$ cd enumivo.cdt
$ ./build.sh
$ sudo ./install.sh
```

### Installed Tools
---
* enumivo-cpp
* enumivo-cc
* enumivo-ld
* enumivo-init
* enumivo-abigen
* enumivo-abidiff
* enumivo-pp (post processing pass for WASM, automatically runs with enumivo-cpp and enumivo-ld)
* enumivo-wasm2wast
* enumivo-wast2wasm
* enumivo-ranlib
* enumivo-ar
* enumivo-objdump
* enumivo-readelf
