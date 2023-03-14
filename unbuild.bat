@echo off

set BUILD_DIR=build

pushd %BUILD_DIR%

del *.pdb
del *.obj
del *.exp
del *.dll
del *.exe
del *.lib

popd
