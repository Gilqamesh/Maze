@echo off

set BUILD_DIR=build

pushd %BUILD_DIR%

@REM Initialize MSVC compiler
call shell.bat

@REM Build order for all the modules
call build_console.bat

call build_readers.bat

call build_window.bat

call build_ui.bat

@Rem Finally build the executable
call build_app.bat

popd %BUILD_DIR%
