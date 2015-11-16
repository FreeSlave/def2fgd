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

VERSION=$(cat $FROM/version)

CXXFLAGS="-O2 -Wformat -Werror=format-security -Wall"

NAME="def2fgd-$VERSION"

# LINUX_M32_BIN=$FROM/bin/bin-m32
# make OBJ_DIR=$FROM/build/build-m32 BIN_DIR=$LINUX_M32_BIN CXXFLAGS="$CXXFLAGS" USER_FLAGS="-m32"
# LINUX_M32=$LINUX_M32_BIN/def2fgd
# strip $LINUX_M32
# $TAR $TO/$NAME-linux-i686.tar.gz -C $FROM license.txt -C $LINUX_M32_BIN def2fgd
# 
# LINUX_M64_BIN=$FROM/bin/bin-m64
# make OBJ_DIR=build/build-m64 BIN_DIR=$LINUX_M64_BIN CXXFLAGS="$CXXFLAGS" USER_FLAGS="-m64"
# LINUX_M64=$LINUX_M64_BIN/def2fgd
# strip $LINUX_M64
# $TAR $TO/$NAME-linux-amd64.tar.gz -C $FROM license.txt -C $LINUX_M64_BIN def2fgd

MINGW_X86=$(command -v i586-mingw32msvc-g++)
MINGW_X86_STRIP=$(command -v i586-mingw32msvc-strip)

if ! [ -x "$MINGW_X86" ]; then
	MINGW_X86=$(command -v i686-w64-mingw32-g++)
	MINGW_X86_STRIP=$(command -v i686-w64-mingw32-strip)
fi

if [ -x "$MINGW_X86" ]; then
	WIN32_BIN=$FROM/bin/bin-mingw-x86
	make OBJ_DIR=build/build-mingw-x86 BIN_DIR=$WIN32_BIN CXXFLAGS="$CXXFLAGS" USER_FLAGS="-m32 -static" CXX="$MINGW_X86" PROGRAM=def2fgd.exe
	WIN32=$WIN32_BIN/def2fgd.exe
	$MINGW_X86_STRIP $WIN32
	$ZIP $TO/$NAME-windows-x86.zip $FROM/license.txt $WIN32
fi

MINGW_X86_64=$(command -v x86_64-w64-mingw32-g++)

if [ -x "$MINGW_X86_64" ]; then
	WIN64_BIN=$FROM/bin/bin-mingw-x86_64
	make OBJ_DIR=build/build-mingw-x86_64 BIN_DIR=$WIN64_BIN CXXFLAGS="$CXXFLAGS" USER_FLAGS="-m64 -static" CXX="$MINGW_X86_64" PROGRAM=def2fgd.exe
	WIN64=$WIN64_BIN/def2fgd.exe
	x86_64-w64-mingw32-strip $WIN64
	$ZIP $TO/$NAME-windows-x86_64.zip $FROM/license.txt $WIN64
fi


