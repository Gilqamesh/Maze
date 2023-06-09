@echo off

set EXECUTABLE=mazegenerator

set GenerateIntrinsicFunctions=/Oi
set EnhanceOptimizedDebugging=/Zo
set FullSmbolicDebuggingInformation=/Z7
set NoLogo=/nologo
set DisableStandardCStackUnwinding=/EHa-
set MultithreadStaticRunTimeLibraryWithDebug=/MTd

set CFLAGS=%GenerateIntrinsicFunctions%^
           %EnhanceOptimizedDebugging%^
           %FullSmbolicDebuggingInformation%^
           %NoLogo%^
           %DisableStandardCStackUnwinding%^
           %MultithreadStaticRunTimeLibraryWithDebug%^
           /Fe%EXECUTABLE%

set NoIncrementalLinking=/INCREMENTAL:NO
set EliminateNotReferencedFunctions=/OPT:REF

set LFLAGS=%NoIncrementalLinking%^
                %EliminateNotReferencedFunctions%^
                /SUBSYSTEM:WINDOWS^
                random.lib^
                console.lib^
                window.lib^
                button.lib^
                file_reader.lib

cl %CFLAGS% ../src/main.c ../src/maze.c /link %LFLAGS%
