# Maze generator
No third party libraries were used, everything is built from scratch, except for:
- windows gdi calls, win32 api
- libc
- compiler


## Build steps

**Run the following batch file to build:**
- build.bat

> If it didn't work, please look into `build/shell.bat` and change the path from `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build` to where you have Visual Studio installed to initialize the MSVC compiler for the duration of the build

## Controls
- Press the overlay buttons to generate a maze with increased/decreased dimensions

> Due to lack of asset loading / texture handling implementation in my framework, can't currently make this part better
