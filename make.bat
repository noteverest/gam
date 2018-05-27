@echo off

mkdir out
pushd out
cl -Zi ^
 /EHsc ^
 /D "UNICODE" ^
 /D "_UNICODE" ^
 ..\src\win32_main.cpp ^
 ..\src\win32_file_io.cpp ^
 ..\src\render.cpp ^
 Shell32.lib ^
 OpenGL32.lib ^
 glew32.lib ^
 freeglut.lib
popd
