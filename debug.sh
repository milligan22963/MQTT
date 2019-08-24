#!/bin/bash

BUILD_TYPE=Release

usage()
{
    echo "Usage: $0"
    echo "  -t (--type) - indicates the build type, Release or Debug"
    ech0 "  -h (--help) - print this usage statement"
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
gdb mqttClientTest core
