@echo off
set name=%~n1
set name=%name: =_%
jasm.exe %1 -o "%~n1.j" -a "%name%" --input-jasm
pause