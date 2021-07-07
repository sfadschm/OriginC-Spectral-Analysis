@echo off

cd "./../src/Spectral Analysis"

echo Running cppcheck ...
call cppcheck "Spectral Analysis.c" headers/*.h
echo.

echo Running clang-format ...
call "./../../admin/tools/bin/clang-format" --verbose --style=file -i *.c headers/*.h
echo Source code formatted.

echo.
pause