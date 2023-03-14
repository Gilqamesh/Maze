@echo off

set MODULE_NAME=ui

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
    window.lib

cl %CFLAGS%^
    ../src/%MODULE_NAME%/button.c^
    ../src/%MODULE_NAME%/ui_group.c^
    /link %LFLAGS%
