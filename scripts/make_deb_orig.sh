#!/bin/sh

CUR=$(dirname $(readlink -f -- $0))
FROM=$(basename $(readlink -f -- $CUR/..))

cd $CUR/../..

FILES="$FROM/Makefile $FROM/src/* $FROM/*.1"

ARCHIVE=$FROM/deb/def2fgd_1.0.orig.tar.gz

tar czf "$ARCHIVE" $FILES
tar xf "$ARCHIVE" --directory $FROM/deb
