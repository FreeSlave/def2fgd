#!/bin/sh

set -e

CUR=$(dirname $(readlink -f -- $0))
cd $CUR/..

VERSION=$(cat version)
FILES="./Makefile ./license.txt ./src/* ./*.1 ./po/*.po ./po/*.pot ./version ./bash_completion/def2fgd"

TARGET_DIR=$HOME/rpmbuild

ARCHIVE=def2fgd-$VERSION.tar.gz

create_orig()
{
    tar czf $TARGET_DIR/SOURCES/$ARCHIVE $FILES --transform="s|\\./|def2fgd-$VERSION/|"
    cp rpm/def2fgd.spec "$TARGET_DIR/SPECS"
}

build_package()
{
    local CONFIGURATION=$1
    local VENDOR=$2
    local SRCRPM="def2fgd-$VERSION-1.$VENDOR.src.rpm"
    
    mock --resultdir="$TARGET_DIR/SRPMS" -r "$CONFIGURATION" --buildsrpm --spec "$TARGET_DIR/SPECS/def2fgd.spec" --sources "$TARGET_DIR/SOURCES"
    mock --resultdir="./rpm/$CONFIGURATION" -r "$CONFIGURATION" "$TARGET_DIR/SRPMS/$SRCRPM"
}

if [ "$1" = "orig" ]; then
    create_orig
elif [ "$1" = "build" ]; then
    if [ -z "$2" ]; then
        echo "Must specify configuration (e.g. epel-6-x86_64 or epel-6-i386)"
        exit 1
    fi
    
    if [ -z "$3" ]; then
        echo "Must specify vendor (e.g. el6)"
        exit 1
    fi
    
    build_package "$2" "$3"
else 
    echo "Specify one of these actions: orig|build"
    exit 1
fi
