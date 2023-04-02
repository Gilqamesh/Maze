@echo off

set MODULE_NAME=reader

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
    /SUBSYSTEM:WINDOWS

cl %CFLAGS%^
    ../src/%MODULE_NAME%/file_reader.c^
    ../src/%MODULE_NAME%/riff_loader.c^
    ../src/%MODULE_NAME%/bmp_loader.c^
    ../src/%MODULE_NAME%/wav_loader.c^
    /link %LFLAGS%
