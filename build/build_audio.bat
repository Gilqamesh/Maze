@echo off

set MODULE_NAME=audio

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
    Ole32.lib^
    Xaudio2.lib^
    file_reader.lib^
    /PDB:%MODULE_NAME%.pdb

cl %CFLAGS%^
    ../src/%MODULE_NAME%/audio.c^
    ../src/%MODULE_NAME%/audio_callback.c^
    /link %LFLAGS%
