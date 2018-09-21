# Enumivo.CDT (Contract Development Toolkit)
## Version : 1.2.1

Enumivo.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the Enumivo platform.  In addition to being a general purpose WebAssembly toolchain, [Enumivo](https://github.com/enumivo/enumivo) specific optimizations are available to support building Enumivo smart contracts.  This new toolchain is built around [Clang 7](https://github.com/enumivo/llvm), which means that Enumivo.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

## New Features from Enumivo
- Compile (-c) option flag will compile to a WASM-elf object file
- ranlib and ar support for static libraries for WASM
- \_\_FILE\_\_ and \_\_BASE\_FILE\_\_ will now only return the file name and not the fullpath. This eliminates any non-determinism from location of the compiled binary
- Global constructors and global destructors are now supported

- _enumivo-cpp_, _enumivo-cc_, _enumivo-ld_, _enumivo-pp_, and _enumivo_abigen_ are set the core set of tools that you will interact with.
    * These are the C++ compiler, C compiler, linker, postpass tool and ABI generator.
- A simple CMake interface to build Enumivo smart contracts against Enumivo.CDT

### Guided Installation
First clone

```sh
$ git clone --recursive https://github.com/enumivo/enumivo.cdt
$ cd enumivo.cdt
```

Now run `build.sh` and give the core symbol for the Enumivo blockchain that intend to deploy to.
    *`build.sh` will install any dependencies that are needed.
```sh
$ ./build.sh <CORE_SYMBOL>
```

Finally, install the build
    *This install will install the core to ```/usr/local/enumivo.cdt``` and symlinks to the top level tools (compiler, ld, etc.) to ```/usr/local/bin```
```sh
$ sudo ./install.sh
```

### Building your first smart contract
- Navigate to the hello folder in examples (./examples/hello).
- You should then see the hello.cpp file
- Now run the compiler
```sh
$ enumivo-cpp hello.cpp -o hello.wasm
```
- As of this release abi generation is not available.

#### Optional, if you know cmake
- If you want to test out the CMake system, stay in the same directory as the previous manual build.
- Then create a build directory ```mkdir build``` and cd into that directory ```cd build```
- Then run ```cmake ../```, this will generate the cache and supporting files for CMake to do it's job.
- Now simply run ```make```.
- You should now have a `hello` file in the build directory, this is the wasm file,  if you would like to use ```enucli set contract``` and you need to change the name to have the .wasm extension that is perfectly fine, or you can change the add_executable target name to ```hello.wasm```, or you can use ```enucli set code <path-to-compiled-wasm>/hello``` to load the file to the blockchain , without the .wasm extension.

### How to use enumivo-abigen
#### using with enumivo-cpp
To generate an abi with ```enumivo-cpp```, the only flag you need to pass to ```enumivo-cpp``` is `-abigen`, this will tell the compiler to run `enumivo-abigen` after compilation and linking stages.  If the output filename is specified as a '.wasm' file with the `-o` option (e.g. \<filename\>.wasm) then enumivo-cpp will tell the abi generator to create the abi with the name \<filename\>.abi, if no '.wasm' suffix is used then the resulting output filename is still \<filename\>.abi 

Example:
```bash
$ enumivo-cpp hello.cpp -o hello.wasm --abigen
```
This will generate two files:
* The compiled binary wasm (hello.wasm)
* The generated abi file (hello.abi)

#### using enumivo-abigen alone
To generate an abi with ```enumivo-abigen```, only requires that you give the main '.cpp' file to compile and the output filename `--output`.

Example:
```bash
$ enumivo-abigen hello.cpp --output=hello.abi
```

This will generate one file:
* The generated abi file (hello.abi)

### Difference from old abi generator
Unlike the old abi generator tool, the new tool uses C++11 or GNU style attributes to mark ```actions``` and ```tables```.

Example (four ways to declare an action for ABI generation):
```c++
// this is the C++11 and greater style attribute
[[enumivo::action]]
void testa( account_name n ) {
	// do something
}

// this is the GNU style attribute, this can be used in C code and prior to C++ 11
__attribute__((enumivo_action)) 
void testa( account_name n ){
	// do something
}

struct [[enumivo::action]] testa {
	account_name n;
    ENULIB_SERIALIZE( testa, (n) )
};

struct __attribute__((enumivo_action)) testa {
	account_name n;
    ENULIB_SERIALIZE( testa, (n) )
};
```
If your action name is not a valid Enumivo name you can explicitly specify the name in the attribute 
```c++ [[enumivo::action("<valid action name>")]]```

Example (Two ways to declare a table for abi generation):
```c++
struct [[enumivo::table]] testtable {
	uint64_t owner;
  	/* all other fields */
};

struct __attribute__((enumivo_table)) testtable {
	uint64_t owner;
    /* all other fields */
};

typedef enumivo::multi_index<N(tablename), testtable> testtable_t;
```
If you don't want to use the multi-index you can explicitly specify the name in the attribute ```c++ [[enumivo::table("<valid action name>")]]```

For an example contract of abi generation please see the file ./examples/abigen_test/test.cpp, you can generate the abi for this file with `enumivo-abigen test.cpp --output=test.abi`.

### Fixing an ABI, or Writing an ABI
- The sections to the abi are pretty simple to understand and the syntax is purely JSON, so we are going to write our own ABI file.
- Even after ABI generation is available, an important note should be made that the generator will never be completely perfect for every contract written, advanced features of the newest version of the ABI will require manual construction of the ABI, and odd and advanced C++ patterns could capsize the generators type deductions, so having a good knowledge of how to write an ABI should be an essential piece of knowledge of a smart contract writer.

### Installed Tools
---
* [enumivo-cpp](#enumivo-cpp)
* [enumivo-cc](#enumivo-cc)
* [enumivo-ld](#enumivo-ld)
* enumivo-abigen
* enumivo-pp (post processing pass for WASM, automatically runs with enumivo-cpp and enumivo-ld)
* enumivo-wasm2wast
* enumivo-wast2wasm
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

if(ENUMIVO_CDT_ROOT STREQUAL "" OR NOT ENUMIVO_CDT_ROOT)
    set(ENUMIVO_CDT_ROOT "/usr/local/enumivo.cdt")
endif()
list(APPEND CMAKE_MODULE_PATH ${ENUMIVO_CDT_ROOT}/lib/cmake)
include(EnumivoWasmToolchain)

add_executable( test.wasm test.cpp )
```
```test.cpp```
```
#include <enulib/enu.hpp>
using namespace enumivo;
class test : public enumivo::contract {
public:
   using contract::contract;
   [[enumivo::action]]
   void testact( account_name test ) {
   }
};
ENUMIVO_ABI( test, (testact))
```

Since, EnumivoWasmToolchain overwrites `cmake` to cross-compile WASM, standard cmake commands of _add\_executable/ add\_library_ can then be used.  Also note, the __ENUMIVO_CDT_ROOT__ variable, this needs to be set if you decided to install to the non-default location.

To manually compile source code:
Use ```enumivo-cpp/enumivo-cc``` and ```enumivo-ld``` as if it were __clang__ and __lld__ , with all includes and options specific to Enumivo and CDT being baked in.

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

### enumivo-abigen
---
```bash
USAGE: enumivo-abigen [options] <source0> [... <sourceN>]

OPTIONS:

Generic Options:

  -help                      - Display available options (-help-hidden for more)
  -help-list                 - Display list of available options (-help-list-hidden for more)
  -version                   - Display the version of this program

enumivo-abigen:
generates an abi from C++ project input

  -extra-arg=<string>        - Additional argument to append to the compiler command line
  -extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -output=<string>           - Set the output filename and fullpath
  -p=<string>                - Build path
```

License
----

MIT
