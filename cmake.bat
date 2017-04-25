@echo off

REM -DCMAKE_BUILD_TYPE=Debug -DDEB="-gd"

rmdir build
mkdir build

cd build
cmake .. -DBOOST_INCLUDE_PATH=C:\Users\void\develop\boost_1_64_0 -DBOOST_LIBRARY_PATH=C:\Users\void\develop\boost_1_64_0\stage\lib -DDEB="" -G "Visual Studio 14 2015 Win64"

cd ..