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

or just for the standalone client and not tests (there is a current bug with compiling tests):

```bash
$ make standalone
```

And then you can ask for help, and run against a binary. For example, I built tcl with spack
and copied a library into the root folder.

```bash
$ ./build/standalone/Smeagle -h
Extract library metadata, the precious.
Usage:
  ./build/standalone/Smeagle [OPTION...]

  -h, --help         Show help
  -v, --version      Print the current version number
  -l, --library arg  Library to inspect
  -f, --fmt arg      Format to output in (default: terminal)
```
```bash
root@98ad59b3999d:/code# ./build/standalone/Smeagle -l libtcl8.6.so 
interface(TclFindElement)
abi_typelocation(function,libtcl8.6.so,TclFindElement,interp,Tcl_Interp *,unknown)
abi_typelocation(function,libtcl8.6.so,TclFindElement,list,const char *,unknown)
abi_typelocation(function,libtcl8.6.so,TclFindElement,listLength,int,%rdx)
...
```

The part that I'm focusing on now is parsing the types into actual locations 
(the unknown strings above I haven't done yet).
You can also make the standalone client, the docs, format the code, or run tests.

```bash
$ make standalone
$ make docs
$ make fmt
$ make test
```
**important** be careful about formatting code from the container -
it changes all permissions. If you do this and need to fix (from outside the container):

```bash
$ sudo chown -R $USER .
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details
on the formatting.


## Additional tools

The test and standalone subprojects include the [tools.cmake](cmake/tools.cmake) file which is used to import additional tools on-demand through CMake configuration arguments.
The following are currently supported.

### Sanitizers

Sanitizers can be enabled by configuring CMake with `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`.

### Static Analyzers

Static Analyzers can be enabled by setting `-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>`, or a combination of those in quotation marks, separated by semicolons.
By default, analyzers will automatically find configuration files such as `.clang-format`.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS` variables.

### Ccache

Ccache can be enabled by configuring with `-DUSE_CCACHE=<ON | OFF>`.

### License

This project is part of Spack. Spack is distributed under the terms of both the MIT license and the Apache License (Version 2.0). Users may choose either license, at their option.

All new contributions must be made under both the MIT and Apache-2.0 licenses.

See LICENSE-MIT, LICENSE-APACHE, COPYRIGHT, and NOTICE for details.

SPDX-License-Identifier: (Apache-2.0 OR MIT)

LLNL-CODE-811652
