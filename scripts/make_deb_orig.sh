#!/bin/sh

CUR=$(dirname $(readlink -f -- $0))
FROM=$(basename $(readlink -f -- $CUR/..))

VERSION=$(cat $CUR/../version)

cd $CUR/../..

FILES="$FROM/Makefile $FROM/src/* $FROM/*.1 $FROM/po/*.po $FROM/po/*.pot $FROM/version $FROM/bash_completion/def2fgd"

ARCHIVE=$FROM/deb/def2fgd_$VERSION.orig.tar.gz

tar czf "$ARCHIVE" $FILES
tar xf "$ARCHIVE" --directory $FROM/deb
