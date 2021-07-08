@echo off

cd ..

setlocal enabledelayedexpansion
set PATHS=
for /r "src" %%P in (*.c, *.h) do (set PATHS=!PATHS! "%%~fP")

echo Running clang-format ...
call "admin/tools/bin/clang-format" --verbose --style=file -i %PATHS%
echo Source code formatted.

echo.
pause