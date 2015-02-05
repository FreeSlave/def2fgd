def2fgd is tool for converting .def and .ent files used by GtkRadiant and Netradiant to .fgd files used by Jackhammer.

## Building from source

The following directions are basically for GNU/Linux.
You need **make** and **g++**:

    sudo apt-get install make g++ # for DEB-based distros
    sudo yum install make gcc-g++ # for RPM-based distros

For default build just type **make** being in the root directory of repository (where Makefile is placed):

    make

**def2fgd** should be placed to build/bin directory.

To remove generated object files type:

    make clean

Note that it does not remove executable. Run the following command to remove both objects and executable files:

    make cleanall

List of user options:

    BUILD_DIR       # build directory. Default is 'build'
    CXX             # alternate compiler. Default is g++
    USER_FLAGS      # additional flags to compiler.
    INSTALL_PATH    # change install path. Default is /usr/local/bin

Some useful examples showing usage of user options:

    make BUILD_DIR=build-m32 USER_FLAGS=-m32    # force building of 32-bit binaries
    make BUILD_DIR=build-m32 USER_FLAGS=-m32 clean # must be cleaned using the same options
    make BUILD_DIR=build-m64 USER_FLAGS=-m64    # force building of 64-bit binaries
    make BUILD_DIR=build-clang CXX=clang++      # use clang++ instead of g++
    make BUILD_DIR=build-static USER_FLAGS=-static-libstdc++ # linking to libstdc++ statically
    make BUILD_DIR=build-debug USER_FLAGS=-ggdb # build with debug symbols

## Usage

Open terminal (command line) and type:

    /path/to/def2fgd input-file output-file

Where input-file is path to .def or .ent file and output-file is name for output fgd file. 
