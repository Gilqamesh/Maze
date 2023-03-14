@echo off

set MODULE_NAME=readers

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
    console.lib

pushd build

cl %CFLAGS%^
    ../src/%MODULE_NAME%/file_reader.c^
    ../src/%MODULE_NAME%/bmp_loader.c^
    /link %LFLAGS%

popd
