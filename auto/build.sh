#!/bin/bash
#
# build.sh: mGEff Daily build shell script
# Copyright (C) 2010 Feynman Software.
# Author: humingming
#


echo " "


# check Makefile give params
if [ $# -le 0  ]; then
    echo "not enought param"
    exit 1
else
    INC_FILE=$1
    echo $INC_FILE
fi

# check whether inc file exist
if [ ! -f $INC_FILE ]; then
    echo "$INC_FILE does not exist!"
    exit 1
fi

COMMON_CFG=common.cfg
# include the inc file
. ./$INC_FILE
. ./$COMMON_CFG


# export compile enviroment variables
echo "svn path is: $URI"
echo "configure is: $CONFIGURE_OPTS"
echo "MiniGUI cfg path is: $MG_CFG_PATH"
echo "MiniGUI res path is: $MG_RES_PATH"
echo "LD_LIBRARY_PATH is: $LD_LIBRARY_PATH"
echo "CFLAGS is: $CFLAGS"
echo "CPPFLAGS is: $CPPFLAGS"
echo "LDFLAGS is: $LDFLAGS"
echo " "

export MG_CFG_PATH
export MG_RES_PATH
export LD_LIBRARY_PATH
export CFLAGS
export CPPFLAGS
export LDFLAGS


# get report file path
FOLDER=${URI##*/}
BUILD_LOG=${INC_FILE/.inc/-compile.txt}
REAL_BLOG=../$BUILD_LOG
REAL_BLOG2=../../$BUILD_LOG
TEST_LOG=${INC_FILE/.inc/-gtest.txt}
REAL_TLOG=../../../$TEST_LOG


# get source from svn
rm -rf $FOLDER
echo "get source ... ..."
svn co $URI > $BUILD_LOG 2>&1
echo " "
echo " " >> $BUILD_LOG
echo " " >> $BUILD_LOG


# compile mgeff
echo "enter $FOLDER"
cd $FOLDER
svn info >> $REAL_BLOG 2>&1
echo " " >> $REAL_BLOG
./autogen.sh >> $REAL_BLOG 2>&1
./configure $CONFIGURE_OPTS >> $REAL_BLOG 2>&1
echo " " >> $REAL_BLOG
echo "make mgeff ... ..."
make install >> $REAL_BLOG 2>&1
echo "make mgeff complete " >> $REAL_BLOG
echo " " >> $REAL_BLOG
echo "make mgeff complete "
echo " "


# compile auto testcase
echo "enter testcase"
cd testcase
./autogen.sh >> $REAL_BLOG2 2>&1
./configure >> $REAL_BLOG2 2>&1
echo " " >> $REAL_BLOG2
echo "make testcase ... ..."
make >> $REAL_BLOG2 2>&1
echo "make testcase complete " >> $REAL_BLOG2
echo " " >> $REAL_BLOG2
echo "make testcase complete"
echo " "


# running auto testcase
echo " "
echo "enter auto"
cd auto
echo "running auto testcase ... ..."

if [ ! -f Makefile ]; then
    echo "can not get auto testcase program list."
    echo "$INC_FILE auto testcase not running."
else
#    PROGRAMS=$(grep noinst_PROGRAMS Makefile.am)
#    PROGRAMS=${PROGRAMS##*=}
#    echo "total testcase is: $PROGRAMS"

#    echo "begin $INC_FLIE testcase" > $REAL_TLOG
#    echo " " >> $REAL_TLOG
#	for program in $PROGRAMS 
#    do 
#        echo "running $program ... ..." 
#        echo "running $program ... ..." >> $REAL_TLOG 
#        ./$program >> $REAL_TLOG
#        echo " " >> $REAL_TLOG
#	done 

    echo "begin $INC_FLIE testcase" > $REAL_TLOG
    make autotest >> $REAL_TLOG 2>&1

    echo "$INC_FILE auto test complete"
    echo " "
fi




