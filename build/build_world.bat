@echo off

set MODULE_NAME=world

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
    window.lib^
    /PDB:%MODULE_NAME%.pdb

cl %CFLAGS%^
    ../src/%MODULE_NAME%/camera.c^
    ../src/%MODULE_NAME%/world.c^
    ../src/%MODULE_NAME%/entity.c^
    ../src/%MODULE_NAME%/renderer.c^
    ../src/%MODULE_NAME%/push_buffer.c^
    ../src/%MODULE_NAME%/sim_region.c^
    ../src/%MODULE_NAME%/world_grid.c^
    ../src/%MODULE_NAME%/world_position.c^
    /link %LFLAGS%
