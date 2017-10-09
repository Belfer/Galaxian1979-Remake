#!/bin/sh

if [ ! -d build ]; then mkdir build; fi;
cd ./build

if [ $1 = "Release" ]; then
  if [ ! -d Release ]; then mkdir Release; fi;
  cd ./Release
  cmake -DCMAKE_BUILD_TYPE=Release .
  make ./
else
  if [ ! -d Debug ]; then mkdir Debug; fi;
  cd ./Debug
  cmake -DCMAKE_BUILD_TYPE=Debug .
  make ./
fi
