#! /bin/sh

ZIP=zip -qj

mkdir -p archives
make BUILD_DIR=build-m32 USER_FLAGS="-m32 -static-libstdc++"
LINUX_M32=build-m32/bin/def2fgd
strip $LINUX_M32
$ZIP archives/def2fgd_linux_i686.zip $LINUX_M32

make BUILD_DIR=build-m64 USER_FLAGS="-m64 -static-libstdc++"
LINUX_M64=build-m64/bin/def2fgd
strip $LINUX_M64
$ZIP archives/def2fgd_linux_amd64.zip $LINUX_M64

make BUILD_DIR=build-mingw USER_FLAGS="-m32 -static" CXX=i586-mingw32msvc-g++ PROGRAM=def2fgd.exe
WIN32=build-mingw/bin/def2fgd.exe
i586-mingw32msvc-strip $WIN32
$ZIP archives/def2fgd_win32.zip $WIN32
