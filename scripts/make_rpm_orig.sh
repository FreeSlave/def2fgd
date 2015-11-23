#!/bin/sh

VERSION=$(cat version)

mkdir -p rpm/def2fgd-$VERSION
cp -r Makefile src/ *.1 po/ version license.txt bash_completion/ rpm/def2fgd-$VERSION

ARCHIVE=def2fgd-$VERSION.tar.gz

tar czf "rpm/$ARCHIVE" -C rpm def2fgd-$VERSION

TARGET_DIR=$HOME/rpmbuild

if [ ! -z "$1" ]; then
    TARGET_DIR="$1"
fi

cp "rpm/$ARCHIVE" "$TARGET_DIR/SOURCES"
cp rpm/def2fgd.spec "$TARGET_DIR/SPECS"
