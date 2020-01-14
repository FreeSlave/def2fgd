def2fgd is tool for converting .def and .ent files used by GtkRadiant and Netradiant to .fgd files used by J.A.C.K.

# Usage

Open terminal and type:

    /path/to/def2fgd input-file output-file

On Windows open cmd.exe and type:

    path\to\def2fgd.exe input-file output-file

Where input-file is path to .def or .ent file and output-file is name for generated fgd file. 
Generated fgd should be suitable for loading in J.A.C.K., but it's still not perfect due to some fundamental differences between formats. Besides J.A.C.K. fgd can provide information not provided by Radiant .def or .ent files at all. So some handwork is still needed if you want to make the fgd file better. See [this list](https://bitbucket.org/FreeSlave/def2fgd/wiki/Refined%20fgd%20files) for already modified 'good' fgd files.

def2fgd also has some options for automated adding of offset and bobbing parameters for point entities. To get help run:

    def2fgd -help

# Building

## Building on GNU/Linux or OS X

The following directions are basically for GNU/Linux, but also applied to other platforms (see below).
On Linux you need **make** and **g++**:

    sudo apt-get install make g++ # for DEB-based distros
    sudo yum install make gcc-g++ # for RHEL-based distros. Use dnf instead of yum on Fedora 22 and newer.

On OS X these tools are usually pre-installed.

For default build just type **make** being in the root directory of repository (where Makefile is placed):

    make

**def2fgd** should be placed to 'bin' directory.

## Building on Windows

### Using MinGW

Download MinGW installer from the [official website](http://www.mingw.org/). Ensure that MinGW\bin is in your **PATH**. Open cmd.exe, go to root repository directory and type:

    mingw32-make

Search for def2fgd.exe in 'bin' directory.

### Using Visual Studio

Solution for Visual Studio 2010 is placed in **msvc** directory. Open def2fgd.sln, change configuration to Release, then in menu Build -> Build Solution. def2fgd.exe should appear in build-msvc-release\bin.

## Building on *BSD

Same as on GNU/Linux, but use **gmake** instead of 'make' everywhere. Ensure gmake is installed:

    pkg install gmake # for FreeBSD 10 and above
    pkg_add gmake # for OpenBSD and older FreeBSD

Go to repository directory and run:

    gmake

## User options

List of user options that can be passed to make:

    OBJ_DIR         # build directory. Default is 'build'
    CXX             # alternate compiler. Default is c++
    USER_FLAGS      # additional flags to compiler.
    PREFIX          # prefix for install path. Default is /usr/local
    PROGRAM         # name of executable file to compile. Default is def2fgd.

Some useful examples showing usage of user options:

    make BIN_DIR=bin-m32 OBJ_DIR=build-m32 USER_FLAGS=-m32    # force building of 32-bit binaries
    make BIN_DIR=bin-m32 OBJ_DIR=build-m32 USER_FLAGS=-m32 clean # must be cleaned using the same options
    make BIN_DIR=bin-m64 OBJ_DIR=build-m64 USER_FLAGS=-m64    # force building of 64-bit binaries
    make BIN_DIR=bin-clang OBJ_DIR=build-clang CXX=clang++      # use clang++ instead of g++
    make BIN_DIR=bin-static OBJ_DIR=build-static USER_FLAGS=-static-libstdc++ # linking to libstdc++ statically
    make BIN_DIR=bin-debug OBJ_DIR=build-debug USER_FLAGS=-ggdb # build with debug symbols
    make BIN_DIR=bin-mingw OBJ_DIR=build-mingw CXX=i686-w64-mingw32-g++ PROGRAM=def2fgd.exe # cross-compilation

## Cross-compilation

Being on GNU/Linux it's possible to build def2fgd for both GNU/Linux x86 and x86_64 as well as for MS Windows.

    sudo apt-get install zip mingw32 mingw-w64 gcc-multilib g++-multilib
    mkdir archives
    ./make_archives archives

.tar.gz archives for GNU/Linux and .zip archives for MS Windows should appear in **archives** directory.
