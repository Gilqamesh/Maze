@echo off

set MODULE_NAME=window

set CFLAGS=^
    /Oi^
    /Zo^
    /Z7^
    /nologo^
    /EHa-^
    /MTd^
    /LDd

set LFLAGS=^
    /INCREMENTAL:NO^
    /OPT:REF^
    /DEBUG^
    /PDB:%MODULE_NAME%.pdb^
    /SUBSYSTEM:WINDOWS^
    console.lib^
    Kernel32.lib^
    User32.lib^
    Gdi32.lib

cl %CFLAGS%^
    ../src/%MODULE_NAME%/window.c^
    ../src/%MODULE_NAME%/window_utils.c^
    ../src/%MODULE_NAME%/bit_buffer.c^
    /link %LFLAGS%
