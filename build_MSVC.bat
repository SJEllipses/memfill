@echo off

pushd %~dp0

if not exist build (
    mkdir build
)

echo Compiling...

cl /O2 /MD /DNDEBUG .\src\memfill.cpp /I .\include /Fe:.\build\memfill.exe /EHsc /link /OPT:REF /OPT:ICF
cl /O2 /MD /DNDEBUG .\src\memfill_GUI.cpp /I .\include /Fe:.\build\memfill_GUI.exe /EHsc /link /SUBSYSTEM:WINDOWS user32.lib gdi32.lib /OPT:REF /OPT:ICF

rm memfill.obj
rm memfill_GUI.obj

popd

echo Done.