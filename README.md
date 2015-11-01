def2fgd is tool for converting .def and .ent files used by GtkRadiant and Netradiant to .fgd files used by Jackhammer.

## Building from source

The following directions are basically for GNU/Linux, but also applied to MinGW on Windows (see below).
You need **make** and **g++**:

    sudo apt-get install make g++ # for DEB-based distros
    sudo yum install make gcc-g++ # for RPM-based distros

For default build just type **make** being in the root directory of repository (where Makefile is placed):

    make

**def2fgd** should be placed to build/bin directory.

To remove generated object files type:

    make clean

Note that it does not remove executable. Run the following command to remove both objects and executable files:

    make distclean

List of user options:

    BUILD_DIR       # build directory. Default is 'build'
    CXX             # alternate compiler. Default is c++
    USER_FLAGS      # additional flags to compiler.
    PREFIX          # prefix for install path. Default is /usr/local
    PROGRAM         # name of executable file to compile. Default is def2fgd.

Some useful examples showing usage of user options:

    make BUILD_DIR=build-m32 USER_FLAGS=-m32    # force building of 32-bit binaries
    make BUILD_DIR=build-m32 USER_FLAGS=-m32 clean # must be cleaned using the same options
    make BUILD_DIR=build-m64 USER_FLAGS=-m64    # force building of 64-bit binaries
    make BUILD_DIR=build-clang CXX=clang++      # use clang++ instead of g++
    make BUILD_DIR=build-static USER_FLAGS=-static-libstdc++ # linking to libstdc++ statically
    make BUILD_DIR=build-debug USER_FLAGS=-ggdb # build with debug symbols
    make BUILD_DIR=build-mingw CXX=i586-mingw32msvc-g++ PROGRAM=def2fgd.exe # cross-compilation

## Building on Windows using MinGW

Use **mingw32-make** instead of 'make' everywhere. MinGW\bin must be in your **PATH** environment variable.

## Building on Windows using Visual Studio 2010

Solution for Visual Studio 2010 is placed in **msvc** directory. Open def2fgd.sln, change configuration to Release, then Build -> Build Solution. def2fgd.exe should appear in build-msvc-release\bin.

## Prebuilt packages

See **Downloads** section to check if there's prebuilt package for your platform.

## Usage

Open terminal and type:

    /path/to/def2fgd input-file output-file

On Windows open cmd.exe and type:

    path\to\def2fgd.exe input-file output-file

Where input-file is path to .def or .ent file and output-file is name for output fgd file. 
Generated fgd should be suitable for loading in Jackhammer, but it's still not perfect due to some fundamental differences between formats. Besides Jackhammer fgd can provide information not provided by Radiant def or ent files at all. So some handwork is still needed if you want make the fgd more appropriate. See **Downloads** section for already modified 'good' fgd files.
