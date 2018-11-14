# Enumivo.CDT (Contract Development Toolkit)
## Version : 1.4.1

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
### Building your first smart contract
- Navigate to the hello folder in examples (./examples/hello).
- You should then see the hello.cpp file
- Now run the compiler
```sh
$ enumivo-cpp -abigen hello.cpp -o hello.wasm
```
- Or with CMake
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```
This will generate two files:
* The compiled binary wasm (hello.wasm)
* The generated ABI file (hello.abi)

#### using enumivo-abigen alone
To generate an ABI with ```enumivo-abigen```, only requires that you give the main '.cpp' file to compile and the output filename `--output` and generating against the contract name `--contract`.

Example:
```bash
$ enumivo-abigen hello.cpp --contract=hello --output=hello.abi
```

This will generate one file:
* The generated ABI file (hello.abi)

## Differences between Version 1.2.x and Version 1.3.x

### enulib C API
- Removed the following typedefs to `uint64_t`:
  - `account_name`
  - `permission_name`
  - `scope_name`
  - `table_name`
  - `action_name`
- Added a `uint64_t` typedef called `capi_name` to replace the removed typedefs above:
    - These have been replaced by `capi_name` and as a practice should not be used when writing C++ contract code. Instead, the new version of the `enumivo::name` type from the enulib C++ API should be used to replace these instances. This decision was made because of bad implicit casting issues with `uint64_t` and the new pattern should allow for better type safety.
- Removed `symbol_name` typedef:
    - This has no C equivalent to replace it. When writing C++ contract code, the `enumivo::symbol_code` struct should be used instead. As with the previously mentioned named types, this was removed and replaced with `enumivo::symbol_code` to allow for better type safety in contracts.  To use a symbol, i.e. symbol name and precision, use the `enumivo::symbol` class.
- Removed `time` and `weight_type` typedefs.
- Removed the `transaction_id_type` and `block_id_type` typedefs.
- Removed the `account_permission` struct.
- Renamed the following typedefs:
  - `checksum160` -> `capi_checksum160`
  - `checksum256` -> `capi_checksum256`
  - `checksum512` -> `capi_checksum512`
  - `public_key`  -> `capi_public_key`
  - `signature`   -> `capi_signature`
- Removed the non-existent intrinsics declarations `require_write_lock` and `require_read_lock`.

### enulib C++ API
- Removed enulib/vector.hpp:
  - Removed alias `enumivo::vector` and typedef `bytes`.
  - Going forward contract writers should include `<vector>` from the STL and use `std::vector<char>` instead of bytes.
- Removed enulib/types.hpp.
- Removed enulib/optional.hpp. Use `std::optional` as a replacement.
- Removed enulib/core_symbol.hpp. The contract writer should explicitly specify the symbol.
- Added enulib/name.hpp.

#### enulib/types.hpp
- Moved the typedef `enumivo::extensions_types` to enulib/transaction.hpp.
- Removed comparison functions for `checksum` structs.
- Removal of `enumivo::char_to_symbol`, `enumivo::string_to_name`, `enumivo::name_suffix` functions
- Removal of the `N` macro. The `""_n` operator or the `name` constructor should be used as a type safe replacement. Example: `N(foo)` -> `"foo"_n`, or `N(foo)` -> `name("foo")`.
- Moved `enumivo::name` struct definition and `""_n` operator to enulib/name.hpp.

#### enulib/name.hpp
- Removed implicit and explicit conversions to `uint64_t`.
- Added `enum class` `enumivo::name::raw` which is implicitly converted from an `enumivo::name` (used for template non-type parameters).
- Added `bool` conversion operator for conditionally testing if a name is empty.
- All constructors are now `constexpr`. These take either a `uint64_t`, an `enumivo::name::raw` or a `std::string_view`.
- Added `constexpr` methods `enumivo::name::length` and `enumivo::name::suffix`.
- Added equivalence, inverted equivalence and less than operators to `enumivo::name`.

#### enulib/symbol.hpp
- Removed `enumivo::symbol_type` struct and replaced with `enumivo::symbol` class.
- Added struct `enumivo::symbol_code`:
  - Added two `constexpr` constructors that take either a raw `uint64_t` or an `std::string_view`.
  - Added `constexpr` methods `is_valid`, `length` and `raw`.
  - Added a print method.
  - Added `bool` conversion operator to test is `symbol_code` is empty.
- Removed `enumivo::string_to_symbol`, `enumivo::is_valid_symbol`, `enumivo::symbol_name_length` functions.
- Removed the `S` macro. The symbol constructor should be used as a type safe replacement. Example: `S(4,ENU)` -> `symbol(symbol_code("ENU"), 4)` (or simply `symbol("ENU", 4)` as of v1.3.1).
- Added struct `enumivo::symbol`:
  - Added three `constexpr` constructors that take either a raw `uint64_t`, `symbol_code` and a `uint8_t` precision or an `std::string_view` and a `uint8_t` precision.
  - Added `constexpr` methods `is_valid`, `precision`, `code`, and `raw`. These, respectively, check if the `symbol` is valid, get the `uint8_t` precision, get the `symbol_code` part of the `symbol`, and get the raw `uint64_t` representation of `symbol`.
  - Added equivalence, inverted equivalence and less than operators to `enumivo::symbol`.
- Modified struct `enumivo::extended_symbol`:
  - Restricted fields to private.
  - Added `constexpr` constructor that takes a `enumivo::symbol` and an `enumivo::name`.
  - Added `constexpr` methods `get_symbol` and `get_contract`.
  - Made existing comparison operators `constexpr`.

#### enulib/asset.hpp
- The main constructor now requires a `int64_t` (quantity) and `enumivo::symbol` explicitly.
- The default constructor no longer initializes the instance to a valid zero quantity asset with a symbol equivalent to "core symbol". Instead the default constructed `enumivo::asset` is a bit representation of all zeros (which will cause `is_valid` to fail) so that check is bypassed to allow for `multi_index` and `datastream` to work.
- Old contracts that use `enumivo::asset()` should be changed to either use the core symbol of the specific chain they are targeting i.e. `enumivo::asset(0, symbol(symbol_code("ENU"),4))`. To reduce writing `symbol(symbol_code("ENU"),4)` over and over, a `constexpr` function to return the symbol or `constexpr` global variable should be used.

#### enulib/contract.hpp
- The constructor for `enumivo::contract` now takes an `enumivo::name` for the receiver, an `enumivo::name` for the code, and a `enumivo::datastream<const char*>` for the datastream used for the contract.  The last argument is for manually unpacking an action, see the section on `enumivo::ignore` for a more indepth usage.

#### enulib/dispatcher.hpp
- Renamed the macro `ENUMIVO_ABI` to `ENUMIVO_DISPATCH` as this is more descriptive of what this macro actually does.
- Modified the definition of `ENUMIVO_DISPATCH` to work with the new constructor for `enumivo::contract`.

#### enulib/multi_index.hpp
- The first template parameter for `indexed_by` now requires the argument be convertible to `enumivo::name::raw` (replacing `uint64_t`).
- The first template parameter for `multi_index` now requires the argument be convertible to `enumivo::name::raw` (replacing `uint64_t`).
- The constructor now takes an `enumivo::name` type for the code (replacing `uint64_t`). Scope is still `uint64_t`.
- Various other replacements of `uint64_t` to `enumivo::name`.

#### enulib/singleton.hpp
- The first template parameter for `enumivo::singleton` now requires the argument be convertible to `enumivo::name::raw` (replacing `uint64_t`).
- The constructor now takes an `enumivo::name` type for the code.
- In the methods `get_or_create` and `set`, the argument `bill_to_account` is now of type `enumivo::name` (replacing `uint64_t`).

#### enulib/action.hpp
- Added C++ function `enumivo::require_auth`.
- Added C++ function `enumivo::has_auth`.
- Added C++ function `enumivo::is_account`.
- Redefined `enumivo::permission_level` to use `enumivo::name` in place of `uint64_t`.
- Removed the macro `ACTION`. (The identifier `ACTION` has been reused for another macro described below in the Macros section.)

#### enulib/permission.hpp
 - The optional provided_keys argument of the function `enumivo::check_transaction_authorization` is now of the type `std::set<enumivo::public_key>` rather than the type `std::set<capi_public_key>`. C++ contract code should most likely be using the `enumivo::public_key` struct (defined in "enulib/public_key.hpp") if they need to deal with Enumivo-compatible public keys rather than the `capi_public_key` struct (now renamed from its original name of `::public_key`) from the enulib C API. Note that existing contract code that just referred to the type `public_key` without namespace qualification may have accidentally been using the `capi_public_key` struct and therefore should ideally be modified to use the `enumivo::public_key` C++ type.
 - The `account` and `permission` arguments of `enumivo::check_transaction_authorization` are both `enumivo::name` now instead of `uint64_t`.

#### enulib/ignore.hpp
- Added new type `ignore`:
  - This type acts as a placeholder for actions that don't want to deserialize their fields but want the types to be reflected in the ABI.
    ```c
    ACTION action(ignore<some_type>) { some_type st; _ds >> st; }
    ```
- Added new type `ignore_wrapper`:
  - This allows for calling `SEND_INLINE_ACTION` with `ignore_wrapper(some_value)` against an action with an `ignore` of matching types.

### Macros
- Added `ACTION` macro which is simply a shortcut for `[[enumivo::action]] void`.
- Added `TABLE` macro which is simply a shortcut for `struct [[enumivo::table]]`.
- Added `CONTRACT` macro which is simply a shortcut for `class [[enumivo::contract]]`.

### CMake
- Added `enumivo.cdt-config.cmake` to allow for `find_package(enumivo.cdt)`. See enumivo.cdt/examples/hello or enumivo.cdt/examples/template for an example.
- Added new macro `add_contract`. This new contract takes a contract name, cmake target, then any normal arguments you would give to `add_executable`. See enumivo.cdt/examples/hello or enumivo.cdt/examples/template.
- New version checking mechanism is included. See enumivo.contracts/CMakeLists.txt to see this in use.

### libc
- Replaced `printf`, `sprintf`, and `snprintf` with new minimal variants. This allows contracts to use these functions without causing stack overflow issues.

### libcxx
- Removed `sstream` with the intent to return this after more has been done.
- Added `__cxa_pure_virtual` to allow for pure virtual methods in contract classes.
- `std::to_string` now works without the issues of stack overflows.

### attributes
- Added `[[enumivo::ignore]]` attribute to flag a type as being ignored by the deserializer. This attribute is primarily only used for internal use within enulib.
- Added `[[enumivo::contract]]` attribute. This new attribute is used to mark a contract class as "contract" with the name being either the C++ name of the class or a user specified name (i.e. `[[enumivo::contract("somecontract")]]`). This attribute can also be used in conjunction with the `enumivo::action` and `enumivo::table` attributes for tables that you would like to define outside of the `enumivo::contract` class.  This is used in conjunction with either the raw `enumivo-cpp` option `--contract <name>`, `-o <name>.wasm` or with CMake `add_contract`.  It acts as a filter enabling contract developers to include a header file with attributes from another contract (e.g. enu.token) while generating an ABI devoid of those actions and tables.
  ```c++
  #include <enulib/enu.hpp>
  using namespace enumivo;
  CONTRACT test : public enumivo::contract {
  public:
     using contract::contract;
     ACTION acta(){}
     TABLE taba {
        uint64_t a;
        float b;
        uint64_t primary_key() const { return a; }
     };
  };
  struct [[enumivo::table, enumivo::contract("test")]]
  tabb {
     uint64_t a;
     int b;
  };
  typedef enumivo::multi_index<"testtaba"_n, test::taba> table_a;
  typedef enumivo::multi_index<"testtabb"_n, tabb> table_b;
  ENUMIVO_DISPATCH( test, (acta) )
  ```
  The above code will produce the tables `testtaba` and `testtabb` in your ABI. Example: `enumivo-cpp -abigen test.cpp -o test.wasm` will mark this compilation and ABI generation for the `enumivo::contract` `test`. The same thing can be done with `enumivo-cpp -abigen test.cpp -o test_contract.wasm --contract test` or with the CMake command `add_contract( test, test_contract, test.cpp )`. Either of the previous two approaches will produce a test_contract.wasm and test_contract.abi generated under the context of the contract name of `test`.

### Boost
- Boost is now part of the library. No more external dependence on Boost and all system inclusion are within it's `sysroot`. (Boost will be removed in a future release.)


## ABI generator attributes
Unlike the old ABI generator tool, the new tool uses C++11 or GNU style attributes to mark ```actions``` and ```tables```.
#### [[enumivo::action]]
This attribute marks either a struct or a method as an action.
Example (four ways to declare an action for ABI generation):
```c++
// this is the C++11 and greater style attribute
[[enumivo::action]]
void testa( name n ) {
   // do something
}

// this is the GNU style attribute, this can be used in C code and prior to C++ 11
__attribute__((enumivo_action))
void testa( name n ){
   // do something
}

struct [[enumivo::action]] testa {
   name n;
   ENULIB_SERIALIZE( testa, (n) )
};

struct __attribute__((enumivo_action)) testa {
   name n;
   ENULIB_SERIALIZE( testa, (n) )
};
```
If your action name is not a valid Enumivo name you can explicitly specify the name in the attribute ```c++ [[enumivo::action("<valid action name>")]]```

Example (two ways to declare a table for ABI generation):
```c++
struct [[enumivo::table]] testtable {
   uint64_t owner;
   /* all other fields */
};

struct __attribute__((enumivo_table)) testtable {
   uint64_t owner;
   /* all other fields */
};

typedef enumivo::multi_index<"tablename"_n, testtable> testtable_t;
```
If you don't want to use the multi-index you can explicitly specify the name in the attribute ```c++ [[enumivo::table("<valid action name>")]]```.

For an example contract of ABI generation please see the file ./examples/abigen_test/test.cpp. You can generate the ABI for this file with `enumivo-abigen test.cpp --output=test.abi`.

### Fixing an ABI or Writing an ABI Manually
- The sections to the ABI are pretty simple to understand and the syntax is purely JSON, so it is reasonable to write an ABI file manually.
- The ABI generation will never be completely perfect for every contract written. Advanced features of the newest version of the ABI will require manual construction of the ABI, and odd and advanced C++ patterns could capsize the generators type deductions. So having a good knowledge of how to write an ABI should be an essential piece of knowledge of a smart contract writer.

### Adding Ricardian Contracts and Clauses to ABI
- As of Enumivo.CDT 1.4.0 the ABI generator will try to automatically import contracts and clauses into the generated ABI.  There are a few caveats to this, one is a strict naming policy of the files and an HTML tag used to mark each Ricardian contract and each clause.
- The Ricardian contracts should be housed in a file with the name <contract name>.contracts.md and the clauses should be in a file named <contract name>.clauses.md.
 - For each Ricardian contract the header `<h1 class="contract">ActionName</h1>` should be used, as this directs the ABI generator to attach this Ricardian contract to the specified action.
 - For each Ricardian clause the header `<h1 class="clause">ClauseID</h1>` should be used, as this directs the ABI generator to the clause id and the subsequent body.
 - The option `-R` has been added to enumivo-cpp and enumivo-abigen to add "resource" paths to search from, so you can place these files in any directory structure you like and use `-R<path to file>` in the same vein as `-I` for include paths.
 - To see these in use please see ./examples/hello/hello.contracts.md and ./examples/hello/hello.clauses.md.
  
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
To compile an Enumivo smart contract, the preferred method is to use the template CMakeLists.txt in the examples folder.


For example:

In `CMakeLists.txt`:
```
cmake_minimum_required(VERSION 3.5)
project(test_example VERSION 1.0.0)

find_package(enumivo.cdt)

add_contract( test test test.cpp )
```


In `test.cpp`:

```
#include <enulib/enu.hpp>
using namespace enumivo;

CONTRACT test : public enumivo::contract {
public:
   using contract::contract;

   ACTION testact( name test ) {
   }
};

ENUMIVO_DISPATCH( test, (testact) )
```

To manually compile the source code, use `enumivo-cpp/enumivo-cc` and `enumivo-ld` as if it were __clang__ and __lld__. All the includes and options specific to Enumivo and CDT are baked in.

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
generates an ABI from C++ project input

  -extra-arg=<string>        - Additional argument to append to the compiler command line
  -extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -output=<string>           - Set the output filename and fullpath
  -p=<string>                - Build path
```

License
----

MIT
