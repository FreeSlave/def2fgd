#! /bin/sh

ZIP="zip -qj"
TAR="tar -czf"

CUR=$(dirname $(readlink -f -- $0))

if [ ! -d "$1" ]; then
    echo "Error: existing directory expected as the first argument"
    exit 1
fi

FROM=$CUR/..
TO=$(readlink -f -- "$1")

VERSION=$2

if [ -z $VERSION ]; then
    echo "Error: version expected as the second argument"
    exit 1
fi

CFLAGS="-O2 -fstack-protector-strong -Wformat -Werror=format-security -Wall"

NAME="def2fgd-$VERSION"

LINUX_M32_BIN=$FROM/bin/bin-m32
make OBJ_DIR=$FROM/build/build-m32 BIN_DIR=$LINUX_M32_BIN CFLAGS="$CFLAGS" USER_FLAGS="-m32 -static-libstdc++"
LINUX_M32=$LINUX_M32_BIN/def2fgd
strip $LINUX_M32
$TAR $TO/$NAME-linux-i686.tar.gz -C $LINUX_M32_BIN .

LINUX_M64_BIN=bin/bin-m64
make OBJ_DIR=build/build-m64 BIN_DIR=$LINUX_M64_BIN CFLAGS="$CFLAGS" USER_FLAGS="-m64 -static-libstdc++"
LINUX_M64=$LINUX_M64_BIN/def2fgd
strip $LINUX_M64
$TAR $TO/$NAME-linux-amd64.tar.gz -C $LINUX_M64_BIN . 

WIN32_BIN=bin/bin-mingw
make OBJ_DIR=build/build-mingw BIN_DIR=$WIN32_BIN CFLAGS="$CFLAGS" USER_FLAGS="-m32 -static" CXX=i586-mingw32msvc-g++ PROGRAM=def2fgd.exe
WIN32=$WIN32_BIN/def2fgd.exe
i586-mingw32msvc-strip $WIN32
$ZIP $TO/$NAME-win-x86.zip $WIN32
