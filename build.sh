#!/bin/bash

# get current directory-path and the path of the parent-directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PARENT_DIR="$(dirname "$DIR")"

# create build-directory
BUILD_DIR="$PARENT_DIR/build"
mkdir -p $BUILD_DIR

# create directory for the final result
RESULT_DIR="$PARENT_DIR/result"
mkdir -p $RESULT_DIR

#-----------------------------------------------------------------------------------------------------------------

# clone libKitsuneCommon
git clone  git@gitlab.com:tobiasanker/libKitsuneCommon.git "$PARENT_DIR/libKitsuneCommon"
cd "$PARENT_DIR/libKitsuneCommon"
git checkout 0.5.1

# create build directory for libKitsuneCommon and go into this directory
LIB_KITSUNE_COMMON_DIR="$BUILD_DIR/libKitsuneCommon"
mkdir -p $LIB_KITSUNE_COMMON_DIR
cd $LIB_KITSUNE_COMMON_DIR

# build libKitsuneCommon library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/libKitsuneCommon/libKitsuneCommon.pro" -spec linux-g++ "CONFIG += optimize_full"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_COMMON_DIR/src/libKitsuneCommon.so.0.5.1" "$RESULT_DIR/"
cp -r "$PARENT_DIR/libKitsuneCommon/include" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# clone libKitsunePersistence
git clone  git@gitlab.com:tobiasanker/libKitsunePersistence.git "$PARENT_DIR/libKitsunePersistence"
cd "$PARENT_DIR/libKitsunePersistence"
git checkout 0.3.0

# create build directory for libKitsunePersistence and go into this directory
LIB_KITSUNE_PERSISTENCE_DIR="$BUILD_DIR/libKitsunePersistence"
mkdir -p $LIB_KITSUNE_PERSISTENCE_DIR
cd $LIB_KITSUNE_PERSISTENCE_DIR

# build libKitsunePersistence library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/libKitsunePersistence/libKitsunePersistence.pro" -spec linux-g++ "CONFIG += optimize_full"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_PERSISTENCE_DIR/src/libKitsunePersistence.so.0.3.0" "$RESULT_DIR/"
cp -r "$PARENT_DIR/libKitsunePersistence/include" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# create build directory for libKitsuneNetwork and go into this directory
LIB_KITSUNE_NETWORK_DIR="$BUILD_DIR/libKitsuneNetwork"
mkdir -p $LIB_KITSUNE_NETWORK_DIR
cd $LIB_KITSUNE_NETWORK_DIR

# build libKitsuneNetwork library with qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$PARENT_DIR/libKitsuneNetwork/libKitsuneNetwork.pro" -spec linux-g++ "CONFIG += optimize_full"
/usr/bin/make -j4

# copy build-result and include-files into the result-directory
cp "$LIB_KITSUNE_NETWORK_DIR/src/libKitsuneNetwork.so.0.3.0" "$RESULT_DIR/"
cp -r "$PARENT_DIR/libKitsuneNetwork/include" "$RESULT_DIR/"

#-----------------------------------------------------------------------------------------------------------------

# recreate symlinks
cd "$RESULT_DIR/"
ln -s libKitsuneCommon.so.0.5.1 libKitsuneCommon.so.0.5
ln -s libKitsuneCommon.so.0.5.1 libKitsuneCommon.so.0
ln -s libKitsuneCommon.so.0.5.1 libKitsuneCommon.so

ln -s libKitsunePersistence.so.0.3.0 libKitsunePersistence.so.0.3
ln -s libKitsunePersistence.so.0.3.0 libKitsunePersistence.so.0
ln -s libKitsunePersistence.so.0.3.0 libKitsunePersistence.so

ln -s libKitsuneNetwork.so.0.3.0 libKitsuneNetwork.so.0.3
ln -s libKitsuneNetwork.so.0.3.0 libKitsuneNetwork.so.0
ln -s libKitsuneNetwork.so.0.3.0 libKitsuneNetwork.so
