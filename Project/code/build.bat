@echo off
mkdir ..\..\build
pushd ..\..\build
call "D:Program Files (x86)\Visual Studio\VC\Auxiliary\Build\vcvarsall.bat" x64
cl -Zi C:\Users\Isaac\Desktop\Work\Git\Project\code\win32_GoL.cpp user32.lib gdi32.lib
popd
pause
