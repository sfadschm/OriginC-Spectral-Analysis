@echo off

cd ../src

echo Running clang-format ...
call "./../admin/tools/bin/clang-format" --verbose --style=file -i *.c headers/*.h
echo Source code formatted.

echo.
pause