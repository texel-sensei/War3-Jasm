@echo off
jasm.exe %1 -o "%~n1.jasm" --input-preload
pause