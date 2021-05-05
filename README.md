[![Actions Status](https://github.com/vsoch/Smeagle/workflows/MacOS/badge.svg)](https://github.com/vsoch/Smeagle/actions)
[![Actions Status](https://github.com/vsoch/Smeagle/workflows/Windows/badge.svg)](https://github.com/vsoch/Smeagle/actions)
[![Actions Status](https://github.com/vsoch/Smeagle/workflows/Ubuntu/badge.svg)](https://github.com/vsoch/Smeagle/actions)
[![Actions Status](https://github.com/vsoch/Smeagle/workflows/Style/badge.svg)](https://github.com/vsoch/Smeagle/actions)
[![Actions Status](https://github.com/vsoch/Smeagle/workflows/Install/badge.svg)](https://github.com/vsoch/Smeagle/actions)

<p align="center">
  <img src="https://repository-images.githubusercontent.com/254842585/4dfa7580-7ffb-11ea-99d0-46b8fe2f4170" height="175" width="auto" />
</p>

# Smeagle

This is a C++ project that will attempt to extract features from libraries and 
binaries that can be used to assess ABI Compatibility. I am calling it Smeagle
because these features are considered "the precious" - the subcorpora
of a binary that is enough to determine ABI compatibility. The name is subject
to change. :)


To cleanly separate the library and subproject code, the outer `CMakeList.txt` only defines the library itself while the tests and other subprojects are self-contained in their own directories. 
During development it is usually convenient to [build all subprojects at once](#build-everything-at-once).

## Develop using a container

The [Dockerfile](Dockerfile) will easily create an environment with all the dependencies that you
need.

```bash
docker build -t smeagle .
docker run --rm -it -v $PWD:/code smeagle bash
```

You should then be in the mounted `/code` directory, and you can compile
as follows:

```bash
$ make
```

You can also make the standalone client, the docs, or run tests.

```bash
$ make docs
$ make test
$ make standalone
```

## Details

If you don't want to use the container, it will be up to you to reproduce
the environment in the Dockerfile. Commands of interest include:

### Install dependencies

You will need cmake installed on your system.

```bash
$ sudo apt install cmake clang-format
```

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/Smeagle --help
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# or simply call the executable: 
./build/test/SmeagleTests
```

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -S test -B build/test

# view changes (I didn't get this to work)
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```
See [Format.cmake](https://github.com/vsoch/Format.cmake) for details.

### Build the documentation

The documentation is automatically built and [published](https://vsoch.github.io/Smeagle) whenever a [GitHub Release](https://help.github.com/en/github/administering-a-repository/managing-releases-in-a-repository) is created.
To manually build documentation, call the following command.

```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
# view the docs
open build/doc/doxygen/html/index.html
```

To build the documentation locally, you will need Doxygen, jinja2 and Pygments on installed your system.

### Build everything at once

The project also includes an `all` directory that allows building all targets at the same time.
This is useful during development, as it exposes all subprojects to your IDE and avoids redundant builds of the library.

```bash
cmake -S all -B build
cmake --build build

# run tests
./build/test/SmeagleTests
# format code
cmake --build build --target fix-format
# run standalone
./build/standalone/Smeagle --help
# build docs
cmake --build build --target GenerateDocs
```

### Additional tools

The test and standalone subprojects include the [tools.cmake](cmake/tools.cmake) file which is used to import additional tools on-demand through CMake configuration arguments.
The following are currently supported.

#### Sanitizers

Sanitizers can be enabled by configuring CMake with `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`.

#### Static Analyzers

Static Analyzers can be enabled by setting `-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>`, or a combination of those in quotation marks, separated by semicolons.
By default, analyzers will automatically find configuration files such as `.clang-format`.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS` variables.

#### Ccache

Ccache can be enabled by configuring with `-DUSE_CCACHE=<ON | OFF>`.
