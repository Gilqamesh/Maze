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
           %MultithreadStaticRunTimeLibraryWithDebug%

set NoIncrementalLinking=/INCREMENTAL:NO
set EliminateNotReferencedFunctions=/OPT:REF

set LFLAGS=%NoIncrementalLinking%^
                %EliminateNotReferencedFunctions%^
                /SUBSYSTEM:WINDOWS^
                console.lib^
                window.lib

pushd build

cl %CFLAGS% ../src/main.c /Fe %EXECUTABLE% /link %LFLAGS%

popd
