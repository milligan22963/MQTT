#!/bin/bash

BUILD_TYPE=Release

usage()
{
    echo "Usage: $0"
    echo "  -t (--type) - indicates the build type, Release or Debug"
    echo "  -h (--help) - print this usage statement"
}

while [ "$1" != "" ]; do
    case $1 in
        -t | --type )
            shift
            BUILD_TYPE=$1
            ;;
        -h | --help )
            usage
            exit
            ;;
        * )
        usage
        exit 1
    esac
    shift
done

BUILD_TYPE_MASSAGED=`echo $BUILD_TYPE | tr '[:upper:]' '[:lower:]'`

BUILD_DIR="cmake-$BUILD_TYPE_MASSAGED"

if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

cd $BUILD_DIR

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

cores=`grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4}'`
make -j$cores
