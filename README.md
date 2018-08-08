# WasmSDK
## Version : 1.1.1

WasmSDK is a toolchain for WebAssembly (WASM).  In addition to being a general purpose WebAssembly toolchain, [Enumivo](https://github.com/enumivo/enumivo) specific optimizations are available to support building Enumivo smart contracts.  This new toolchain is built around [Clang 7](https://github.com/enumivo/llvm), which means that the SDK has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

## New Features from Enumivo
- Compile (-c) option flag will compile to a WASM-elf object file
- ranlib and ar support for static libraries for WASM
- \_\_FILE\_\_ and \_\_BASE\_FILE\_\_ will now only return the file name and not the fullpath. This eliminates any non-determinism from location of the compiled binary
- Global constructors and global destructors are now supported
- _enumivo-cpp_, _enumivo-cc_, _enumivo-ld_, and _enumivo-pp_ are set the core set of tools that you will interact with.
    * These are the C++ compiler, C compiler, linker and postpass tools.
- A simple CMake interface to build Enumivo smart contracts against WasmSDK
- ABI generator (Coming Soon)

### Guided Installation
First clone

```sh
$ git clone --recursive https://github.com/enumivo/enumivo.wasmsdk
$ cd enumivo.wasmsdk
```

Now run `build.sh` and give the core symbol for the Enumivo blockchain that intend to deploy to.
    *`build.sh` will install any dependencies that are needed.
```sh
$ ./build.sh <CORE_SYMBOL>
```

Finally, install the build
    *This install will install the core to ```/usr/local/enumivo.wasmsdk``` and symlinks to the top level tools (compiler, ld, etc.) to ```/usr/local/bin```
```sh
$ sudo ./install.sh
```

### Installed Tools
---
* [enumivo-cpp](#enumivo-cpp)
* [enumivo-cc](#enumivo-cc)
* [enumivo-ld](#enumivo-ld)
* enumivo-pp (post processing pass for WASM, automatically runs with enumivo-cpp and enumivo-ld)
* enumivo-ranlib
* enumivo-ar
* enumivo-objdump
* enumivo-readelf

### Usage
---
To compile an Enumivo smart contract, the perferred method is to use the template CMakeLists.txt in the examples folder.
For example:
```CMakeLists.txt```
```
cmake_minimum_required(VERSION 3.5)
project(test_example VERSION 1.0.0)

if(WASM_ROOT STREQUAL "" OR NOT WASM_ROOT)
    set(WASM_ROOT ${CMAKE_INSTALL_PREFIX})
endif()
list(APPEND CMAKE_MODULE_PATH ${WASM_ROOT}/lib/cmake)
include(EnumivoWasmToolchain)

add_executable( test test.cpp )
```
```test.cpp```
```
#include <enulib/enu.hpp>
using namespace enumivo;
class test : public enumivo::contract {
public:
   using contract::contract;
   void test_action( account_name test ) {
   }
};
ENUMIVO_ABI( test, (test_action))
```

Since, EnumivoWasmToolchain overwrites `cmake` to cross-compile WASM, standard cmake commands of _add\_executable/ add\_library_ can then be used.  Also note, the __WASM_ROOT__ variable, this needs to be set if you decided to install to the non-default location.

To manually compile source code:
Use ```enumivo-cpp/enumivo-cc``` and ```enumivo-ld``` as if it were __clang__ and __lld__ , with all includes and options specific to Enumivo and WasmSDK being baked in.

### enumivo-cpp
---
```bash
OVERVIEW: enumivo-cpp (Enumivo C++ -> WebAssembly compiler)
USAGE: enumivo-cpp [options] <input file> ...

OPTIONS:
  -C                       - Include comments in preprocessed output
  -CC                      - Include comments from within macros in preprocessed output
  -D=<string>              - Define <macro> to <value> (or 1 if <value> omitted)
  -E                       - Only run the preprocessor
  -I=<string>              - Add directory to include search path
  -L=<string>              - Add directory to library search path
  -O=<string>              - Optimization level s, 0-3
  -S                       - Only run preprocess and compilation steps
  -U=<string>              - Undefine macro <macro>
  -W=<string>              - Enable the specified warning
  -c                       - Only run preprocess, compile, and assemble steps
  -dD                      - Print macro definitions in -E mode in addition to normal output
  -dI                      - Print include directives in -E mode in addition to normal outpu
  -dM                      - Print macro definitions in -E mode instead to normal output
  -emit-ast                - Emit Clang AST files for source inputs
  -emit-llvm               - Use the LLVM representation for assembler and object files
  -faligned-allocation     - Enable C++17 aligned allocation functions
  -fcoroutine-ts           - Enable support for the C++ Coroutines TS
  -finline-functions       - Inline suitable functions
  -finline-hint-functions  - Inline functions which are (explicitly or implicitly) marked inline
  -fmerge-all-constants    - Allow merging of constants
  -fno-cfl-aa              - Disable CFL Alias Analysis
  -fno-elide-constructors  - Disable C++ copy constructor elision
  -fno-lto                 - Disable LTO
  -fstack-protector        - Enable stack protectors for functions potentially vulnerable to stack smashing
  -fstack-protector-all    - Force the usage of stack protectors for all functions
  -fstack-protector-strong - Use a strong heuristic to apply stack protectors to functions
  -fstrict-enums           - Enable optimizations based on the strict definition of an enum's value range
  -fstrict-return          - Always treat control flow paths that fall off the end of a non-void function as unreachable
  -fstrict-vtable-pointers - Enable optimizations based on the strict rules for overwriting polymorphic C++ objects
  -include=<string>        - Include file before parsing
  -isysroot=<string>       - Set the system root directory (usually /)
  -l=<string>              - Root name of library to link
  -lto-opt=<string>        - LTO Optimization level (O0-O3)
  -o=<string>              - Write output to <file>
  -std=<string>            - Language standard to compile for
  -v                       - Show commands to run and use verbose output
  -w                       - Suppress all warnings

Generic Options:

  -help                    - Display available options (-help-hidden for more)
  -help-list               - Display list of available options (-help-list-hidden for more)
  -version                 - Display the version of this program
```

### enumivo-ld
---
```bash
OVERVIEW: enumivo-ld (WebAssembly linker)
USAGE: enumivo-ld [options] <input file> ...

OPTIONS:

Generic Options:

  -help             - Display available options (-help-hidden for more)
  -help-list        - Display list of available options (-help-list-hidden for more)
  -version          - Display the version of this program

enumivo.ld options:

  -L=<string>       - Add directory to library search path
  -fno-cfl-aa       - Disable CFL Alias Analysis
  -fno-lto          - Disable LTO
  -fno-post-pass    - Don't run post processing pass
  -fno-stack-first  - Don't set the stack first in memory
  -l=<string>       - Root name of library to link
  -lto-opt=<string> - LTO Optimization level (O0-O3)
  -o=<string>       - Write output to <file>
 ```
 
 ### Todos
 ---
 - Add ABI generation to enumivo-cpp

License
----

MIT
