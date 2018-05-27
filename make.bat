@echo off

mkdir out
pushd out
cl -Zi ^
 /D "_UNICODE" ^
 /D "UNICODE" ^
 /EHsc ^
 ..\src\win32_main.cpp ^
 ..\src\render.cpp ^
 ..\src\win32_file_io.cpp ^
 OpenGL32.lib
popd
