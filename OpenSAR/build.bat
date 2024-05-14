@echo off
set SOURCE_FILE=CanIf.c
set OUTPUT_FILE=CanIfExe

echo Compiling %SOURCE_FILE%...
gcc %SOURCE_FILE% Can.c -o %OUTPUT_FILE%

if errorlevel 1 (
    echo Compilation failed.
) else (
    echo Compilation successful.
)