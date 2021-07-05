@echo off

cd ../src

echo Running clang-format ...
call "./../admin/tools/bin/clang-format" --verbose --style=file -i *.c header/*.h
echo Source code formatted.

echo.

pause