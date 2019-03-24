# Enumivo.CDT (Contract Development Toolkit)
## Version : 1.6.1

Enumivo.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the Enumivo platform.  In addition to being a general purpose WebAssembly toolchain, [Enumivo](https://github.com/enumivo/enumivo) specific optimizations are available to support building Enumivo smart contracts.  This new toolchain is built around [Clang 7](https://github.com/enumivo/llvm), which means that Enumivo.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

### Installed Tools
---
* enumivo-cpp
* enumivo-cc
* enumivo-ld
* enumivo-init
* enumivo-abidiff
* enumivo-wasm2wast
* enumivo-wast2wasm
* enumivo-ranlib
* enumivo-ar
* enumivo-objdump
* enumivo-readelf
