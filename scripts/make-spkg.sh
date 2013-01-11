#!/bin/sh

VERSION=1.0

set -e
if [ ! -f configure ] ; then
    echo "You must run this script in the root directory of the libXon repository."
    exit 1
fi


make dist

cd scripts/spkg
rm -rf libxon-$VERSION
mkdir libxon-$VERSION
cd libxon-$VERSION
tar xf ../../../libxon-$VERSION.tar.gz
mv libxon-$VERSION src

cp ../SPKG.txt .
cp ../spkg-install .

echo 'src' > .hgignore
hg init .
hg add
hg commit -m 'This is not the true repository, read SPKG.txt'


cd ..
sage -pkg libxon-$VERSION

