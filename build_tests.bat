@echo off

set BUILD_DIR=build

pushd %BUILD_DIR%

@REM Initialize MSVC compiler
call shell.bat

@REM TODO: check if modules are built

@Rem Finally build the executable
call build_tests.bat

popd %BUILD_DIR%
