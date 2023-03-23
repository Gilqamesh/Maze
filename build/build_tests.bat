@echo off

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
            random.lib^
            console.lib^
            window.lib^
            button.lib^
            file_reader.lib

FOR %%F IN (../src/testers/*.c) DO cl %CFLAGS% ../src/testers/%%F /link %LFLAGS%
