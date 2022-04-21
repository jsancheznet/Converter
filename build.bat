@echo off

pushd build

set ASSIMP_INCLUDE="..\vendor\include"
set ASSIMP_LIB="..\vendor\lib\assimp"

set IncludeDirectories=-I%ASSIMP_INCLUDE%
set LibDirectories=-LIBPATH:%ASSIMP_LIB%

set CompilerFlags= -nologo -W4 -Ot -FS %IncludeDirectories% -Zi -EHsc -MD /D "_WINDOWS"
set LinkerFlags=-nologo -DEBUG %LibDirectories%

cl ..\converter.cpp %CompilerFlags% /link %LinkerFlags% -SUBSYSTEM:CONSOLE assimp-vc142-mt.lib

popd
