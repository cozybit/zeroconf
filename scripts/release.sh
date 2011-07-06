#!/bin/bash
#
# release.sh
#
# This script builds the release tarball of mdns and places it in the
# release directory. First it must call the version script to get or
# set the release version number.
#
# If called with the -n argument, it will name the release as this version,
# first creating a git tag.  For example:
#    ./scripts/release.sh -n 1.22
# will do a 'git tag v1.22' (via version.sh) and then will produce a
# release/zeroconf-1.22.tar.gz release.
#
# Calling ./scripts/release.sh without a parameter will release the current
# version.
#
# This script must be run from the zeroconf base directory.
set -u
set -e

MDNSDIR="./mdns"
VERFILE="$MDNSDIR/version.h"

if [ ! -e $MDNSDIR ]
then
	echo "Error: This script must be run from the zeroconf base directory."
	exit 1
fi

make clean

if [ $# == "0" ]
then
	VERSION=`./scripts/version.sh`
	echo "Releasing version $VERSION"
elif [ $# == "2" ]
then
	if [ $1 == "-n" ]
	then
		echo "Releasing with a new version tag $2"
		VERSION=`./scripts/version.sh -n $2`
	fi
else
	echo
	echo "release.sh usage:"
	echo
	echo "  ./scripts/release.sh :"
	echo "      create release tarball based on current version tag"
	echo "  ./scripts/release.sh -n 1.23 :"
	echo "      create release tarball, first creating new version 1.23"
	exit 1
fi

# Prepare release directory
DIR="release/zeroconf-$VERSION"
mkdir -p $DIR
mkdir -p $DIR/scripts

# Copy all files for the release. If you need to add something, add it here
cp -R mdns $DIR/.
cp -R include $DIR/.
cp -R examples $DIR/.
cp -R scripts/version.sh $DIR/scripts/.
cp -R licenses $DIR/.
cp -R test $DIR/.
cp Makefile $DIR/.
cp README $DIR/.

# Build the tarball
pushd release
tar czf zeroconf-$VERSION.tar.gz zeroconf-$VERSION
md5sum zeroconf-$VERSION.tar.gz > zeroconf-$VERSION.tar.gz.md5
popd
echo "zeroconf v$VERSION is in ./release/zeroconf-$VERSION.tar.gz"

rm -rf $DIR

exit 0
