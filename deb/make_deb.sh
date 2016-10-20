#!/bin/sh

set -e

CUR=$(dirname $(readlink -f -- $0))
cd $CUR/..

VERSION=$(cat version)
FILES="./Makefile ./src/* ./*.1 ./po/*.po ./po/*.pot ./version ./bash_completion/def2fgd"

ARCHIVE=./deb/def2fgd_$VERSION.orig.tar.gz

create_orig()
{
    tar czf "$ARCHIVE" $FILES --transform="s|\\./|def2fgd/|"
    find deb/def2fgd -mindepth 1 -maxdepth 1 -not -name 'debian' -exec rm -rf {} +
    tar xf "$ARCHIVE" --directory ./deb
}

BASETGZPATH=/var/cache/pbuilder

create_basetgz_path()
{
    echo "$BASETGZPATH/$1-$2.tgz"
}

create_pbuilder()
{
    local DISTRO=$1
    local ARCH=$2
    local BASETGZ=$(create_basetgz_path "$DISTRO" "$ARCH")
    
    if [ -f $BASETGZ ]; then
        echo "Updating $BASETGZ"
        sudo pbuilder --update --basetgz $BASETGZ --distribution $DISTRO --architecture $ARCH --override-config
    else
        echo "Creating $BASETGZ"
        sudo pbuilder create --basetgz $BASETGZ --distribution $DISTRO --architecture $ARCH
    fi
}

build_package()
{
    local DISTRO=$1
    local ARCH=$2
    
    create_pbuilder "$DISTRO" "$ARCH"
    
    (cd deb/def2fgd && dpkg-source -b .)
    (cd deb/def2fgd && mkdir -p "../$DISTRO-$ARCH" && pdebuild --buildresult "../$DISTRO-$ARCH" -- --basetgz $(create_basetgz_path "$DISTRO" "$ARCH") ../def2fgd_${VERSION}-1.dsc )
}


if [ "$1" = "orig" ]; then
    create_orig
elif [ "$1" = "build" ]; then
    if [ -z "$2" ]; then
        echo "Must specify distro (e.g. wheezy)"
        exit 1
    fi
    
    if [ -z "$3" ]; then
        echo "Must specify architecture (i386 or amd64)"
        exit 1
    fi
    
    build_package "$2" "$3"
else 
    echo "Specify one of these actions: orig|build"
    exit 1
fi
