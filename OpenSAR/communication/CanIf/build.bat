@echo off
set SOURCE_FILE=CanIf.c
set OUTPUT_FILE=CanIf.o
x86_64-w64-mingw32-gcc -c -I ../../include  ../../boot/config/Can_PBcfg.c 2>> log.txt
x86_64-w64-mingw32-gcc -c -I ../../include -I ../../boot/config/ -I ../../include/generic -I ../../arch/generic/mingw/kernel/ ../../boot/config/CanIf_Cfg.c 2>> log.txt
x86_64-w64-mingw32-gcc -c -I ../../include -I ../../boot/config/ ../../stumps/Can.c 2>> log.txt
x86_64-w64-mingw32-gcc -c -I ../../include -I ../../boot/config/ -I ../../include/generic -I ../../include/ppc -I ../../arch/generic/mingw/kernel/ CanIf.c 2>> log.txt
x86_64-w64-mingw32-gcc -c -I ../../include -I ../../boot/config/ ../../stumps/main.c 2>> log.txt

echo Compiling %SOURCE_FILE%...
x86_64-w64-mingw32-gcc  %SOURCE_FILE% Can.o main.o -o %OUTPUT_FILE% 2>> log.txt
if errorlevel 1 (
    echo Compilation failed.
) else (
    echo Compilation successful.
)

nm %OUTPUT_FILE% | grep CanIf > symbols.txt