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
<<<<<<< HEAD
    ../src/%MODULE_NAME%/audio_callback.c^
=======
>>>>>>> b4525ede878d8dd81f1598897960cc095e29bf62
    /link %LFLAGS%
