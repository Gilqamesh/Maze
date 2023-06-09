@echo off

set BUILD_DIR=build

pushd %BUILD_DIR%

@REM Initialize MSVC compiler
call shell.bat

@REM Build order for all the modules

call build_random.bat

call build_console.bat

call build_readers.bat

call build_audio.bat

call build_window.bat

call build_world.bat

call build_ui.bat

@Rem Finally build the executables
call build_app.bat

call build_tests.bat

popd %BUILD_DIR%
