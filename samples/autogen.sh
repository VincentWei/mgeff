#!/bin/bash

OPT=

if [ "x$PREFIX" != "x" ]; then
    OPT="-D $OPT CMAKE_INSTALL_PREFIX=$PREFIX"
fi

ccmake $OPT .

echo "*****************************************************************"
echo "There is no need to 'configure', just type 'make' to build."
echo ""
echo "$ make"
echo "*****************************************************************"
