@echo off

set MODULE_NAME=console

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
    /PDB:%MODULE_NAME%.pdb 

pushd build

cl %CFLAGS% ../src/%MODULE_NAME%/%MODULE_NAME%.c /link %LFLAGS%

popd
