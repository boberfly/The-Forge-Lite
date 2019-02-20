#!/usr/bin/env bash

set echo off

cmake -E make_directory build/linux_release
cd build/linux_release
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_VULKAN=1 -DBUILD_LINUX=1 -DBUILD_SPIRV_CROSS=1 -DUSE_MEMORY_TRACKING=0 -DBUILD_EXAMPLES=1 -DCMAKE_INSTALL_PREFIX=../../install/linux_release $@ ../..
make -j `getconf _NPROCESSORS_ONLN`
make install

set echo on
