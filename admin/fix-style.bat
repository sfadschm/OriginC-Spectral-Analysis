@echo off

cd ..

echo Running clang-format ...
call clang-format --verbose --style=file -i *.c header/*.h
echo.
echo Source code formatted.

echo.

pause