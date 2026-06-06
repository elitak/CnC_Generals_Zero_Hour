@busybox bash "%~dpnx0" "%~dpnx0"
@exit /b
#!/bin/bash

#FIXME easier to pass $0 in instead of using bash_source dosent work
#FIXME old version didnt interact but this one triggers basherrors for lines1+2

#set -x
wd=$(dirname "$1"); shift
zhDir="D:\\_GAMES-IPFS_/cncscratch/ZeroHour"

#reconfig=1
#clean=1

[[ -n "$clean" ]] && rm -rf "$wd/build"
[[ -n "$reconfig" ]] && cmake --preset win32-deploy -D CMAKE_CONFIGURATION_TYPES=Release -D CMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="/UDO_COPY_PROTECTION"
cmake --build --preset win32-deploy --target RTS --config Release ${clean:+--clean-first}
#if [[ -z "$?" ]]; then
if true; then
  cp -a "$wd/build/win32-deploy/GeneralsMD/Code/Release"/* "$zhDir"/
  echo DONE BUILDING. Press enter to launch the game...; read
  cmd /c start /b "$zhDir\\RTS.exe" -win -quickstart
else
  echo BUILD FAILED. Press enter to exit...; read
fi
