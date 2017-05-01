@echo off

IF DEFINED %VS140COMNTOOLS%
	call "%VS140COMNTOOLS%"\vsvars32.bat

set BOOST_INC=C:\Users\void\develop\boost_1_64_0\
set BOOST_LIB=C:\Users\void\develop\boost_1_64_0\stage\lib
set PLATF="x64"
set CONF=Release

REM Remove prev build
rd build /S /Q
mkdir build 
cd build

REM generate new project files
cmake .. -DBOOST_INCLUDE_PATH=%BOOST_INC% -DBOOST_LIBRARY_PATH=%BOOST_LIB% -G "Visual Studio 14 2015 Win64"

REM build
msbuild fsig.vcxproj /p:Configuration=%CONF% /p:Platform=%PLATF% /maxcpucount

REM cd back
cd ..