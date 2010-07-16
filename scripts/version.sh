#!/bin/bash
#
# version.sh
#
# This script creates the version number and then both creates mdns/version.h
# as well as outputs the version on the commandline.
#
# If called with the -n argument, it will create a new git tag.
# For example:
#    ./scripts/version.sh -n 1.22
# will do a 'git tag v1.22' and then will continue on to create the version
# number and produce the version.h.
#
# Calling ./scripts/version.sh without a parameter will release the current
# version based on the most recent tag.  If HEAD is the same as the tag, it
# will make a version number the same as the tag.  If it isn't it will add
# the the SHA of the most recent commit to the tag: 1.04-d9f147f3.
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

if [ $# == "2" ]
then
	if [ $1 == "-n" ]
	then
		# Creating new tag
		VERSET=$2
		git tag "v$VERSET"
	fi
elif [ $# -ne "0" ]
then
	echo
	echo "version.sh usage:"
	echo
	echo "  ./scripts/version.sh : "
	echo "      create version.h and output version based on current version tag"
	echo "  ./scripts/version.sh -n 1.23 :"
	echo "      create version.h, first creating new version tag 1.23"
	exit 1
fi

TAG=`git tag -l | grep ^v | tail -n 1`
TAGREF=`git show-ref $TAG | awk '{print $1}'`
HEADREF=`git show-ref HEAD | awk '{print $1}'`
COMMIT=`git show-ref $TAG | head -c 8`

if [ "$TAGREF" == "$HEADREF" ]
then
	VERSION=${TAG:1}
else
	VERSION="${TAG:1}-$COMMIT"
fi

cat > $VERFILE <<endofhfile
#ifndef MDNS_VERSION_H
#define MDNS_VERSION_H

/* Defines the version number of this release of mdns */
#define MDNS_VERSION $VERSION

/* Defines the current git tags for reference */
#define MDNS_GIT_TAG $TAG
#define MDNS_GIT_COMMIT $TAGREF

#endif
endofhfile

echo $VERSION
exit 0
