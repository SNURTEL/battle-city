# Battle-city
A simple [Battle City](https://en.wikipedia.org/wiki/Battle_City) clone. Forked from my 2nd semester OOP project. Utilizes a custom event generation and handling system with a simple event loop. SFML is the only dependency, excluding Catch2 >=3.0 for unit testing.

![game](doc/img/tanks.png)

Build with CMake:
```sh
cd build
cmake .
make --build . --target tanks --clean-first
```
Executable should be written to `bin/`.

Here's the original readme:

---

## Contents
- [Repository structure](#repository-structure)
- [Configuration](#configuration)
- [Dependencies](#dependencies)
  - [CMake](#cmake)
  - [Catch2](#catch2)
  - [SFML](#sfml)
  - [Doxygen](#doxygen)
- [Build](#build)
- [Source code structure](#source-code-structure)
- [__Workflow__](#workflow)
- [Full documentation](#full-documentation)

---

## Repository structure

```
proi-projekt
   ├ bin
   │  └ lib
   ├ build
   ├ doc
   │  ├ html 
   │  └ rtf
   └ src
      ├ <name>-lib
     ...
      └ main
``` 
Where:
1. `bin/` - Pre-compiled binaries
    - `lib/` - Libraries
2. `build/` - CMake project files
3. `doc/` - Full documentation (plus tools for re-generating it)
    - `html/` - Doxygen - style documentation in HTML
    - `rtf/` - Doxygen - style documentation in RTF
4. `src/` - Project source code
    - `<name>-lib/` - Project libraries
    - `main/` - Project executable

## Configuration
Load `/build/CMakeLists.txt` into your IDE of choice.
This should be it :)

If the project does not compile, check [dependencies](#dependencies).

## Dependencies
- CMake
- Catch2 (version `v3.x`)
- SFML
- Doxygen (only for generating documentation)

### CMake
Check if CMake is installed:
```bash
cmake --version
```
If not, install a pre-compiled binary from [cmake website](https://cmake.org/download/) or build your own:
```bash
git clone https://gitlab.kitware.com/cmake/cmake.git
cd cmake/bootstrap
./bootstrap && make && sudo make install
```
[Full instructions](https://cmake.org/install/)

### Catch2
Catch2 is required to be installed globally in version `3.x`. **This project does not utilize a `catch.hpp` header and it will not compile without a globally installed Catch2**.

If Catch2 is not installed, install with:
```bash
git clone https://github.com/catchorg/Catch2.git
cd Catch2
cmake -Bbuild -H. -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install
``` 
or:
```bash
git clone
https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install catch2
```
[Full instructions](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md)

### SFML
Install  with:
```bash
sudo apt-get install libsfml-dev
```

[Full instructions](https://www.sfml-dev.org/tutorials/2.5/start-linux.php)

### Doxygen
Check if Doxygen is installed:
```bash
doxygen -v
```
If not, install with:
```bash
git clone https://github.com/doxygen/doxygen.git
cd doxygen
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
make install
```
[Full instructions](https://www.doxygen.nl/download.html)

## Build

Build with cmake:
```bash
cd build
cmake .
cmake --build ../build --target <build_target> --clean-first
```
Where `<build_target>` can be:
- `<libname>-lib`
- `test_<libname>_lib`
- `tanks`

Where `<libname>` should match the name of any library from [`src`](src).

Or simply **make use of your IDE's CMake integration**.

Binaries should be written to [`/bin`](bin).

## Source code structure
### Project structure
The majority of source code is organised in libraries under /src/. Each library should be roughly structured as:
```
─ my-lib
    ├ include
    │    ├ foo.h
    │    ├ bar.h
    |     ...
    │    └ baz.h
    ├ foo.cpp
    ├ bar.cpp
    ...
    ├ baz.cpp
    └ test
          ├ test_foo.cpp
          ├ test_bar.cpp
          ...
          └ test_baz.cpp
```
### CMakeLists.txt
Each library should have its representation in [`CMakeLists.txt`](build/CMakeLists.txt) written as follows:
```cmake
set(my_lib_dir ../src/my-lib)
set(my_lib_sources
        ${my_lib_dir}/foo.cpp
        ${my_lib_dir}/bar.cpp
        ...
        ${my_lib_dir}/baz.cpp
        )

set(my_lib_test_dir ${my_lib_dir}/test)
set(my_lib_test_sources
        ${my_lib_test_dir}/test_foo.cpp
        ${my_lib_test_dir}/test_bar.cpp
        ...
        ${my_lib_test_dir}/test_baz.cpp
        )

add_library(my-lib ${my_lib_sources})
target_link_libraries(my_lib PRIVATE dependency1 dependency2)

add_executable(test_my_lib ${my_lib_test_sources})
target_link_libraries(test_my_lib PRIVATE my-lib Catch2::Catch2WithMain)

# ...

target_link_libraries(my_main PRIVATE my-lib)
```

### Testing
Tests should be written according to BDD methodology:
```c++
SCENARIO("Some action"){
        GIVEN("Tests objects, initial requirements"){
            Foo test_foo{};
            REQUIRE(test_foo.bar() == 123);
            
            WHEN("Something happens"){
                foo.baz();
                
                THEN("There should be some response"){
                    CHECK(foo.qux() == 456);
                }
            }
        }
}
```

## Workflow
Repository should be structured according to GitHub Flow model:
```
main
├ feature_branch_foo
├ feature_branch_bar
...
└ feature_branch_baz
```

When working on the project:
1. Create a new feature branch
   ```bash
   git checkout -b feature-branch-foo
   ```
2. Do your work and commit changes. Don't forget to:
   - Update [CMakeLists.txt](build/CMakeLists.txt) according to [source code structure](#source-code-structure)
   - Write and re-generate the documentation (should match javadoc standard):
       ```bash
       cd doc
       doxygen
       ```
3. Rebase your branch on `main`:
   ```
   git rebase origin main
   ```
4. ...or merge `main` into your branch:
   ```
   git merge origin main
   ```
5. Push your commits to remote repo:
   ```
   git push [-u] origin feature-branch-foo
   ```
6. Open a merge request, address all merge conflicts and **ask someone to review your code**.

## Code documentation
Check [`doc`](/doc):
- [`doc/html`](doc/html) and [`doc/rtf`](doc/rtf) for Doxygen-style code documentation:
   ```bash
  cd ./doc/html/
   opera ./index.html
   ```

