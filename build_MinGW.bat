@echo off

pushd %~dp0

if not exist build (
    mkdir build
)

echo Compiling...

g++ .\src\memfill.cpp -o .\build\memfill.exe -I.\include -s
g++ .\src\memfill_GUI.cpp -o .\build\memfill_GUI.exe -I.\include -mwindows -s

popd

echo Done.