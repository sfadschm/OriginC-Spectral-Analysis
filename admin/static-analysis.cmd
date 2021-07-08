@echo off

cd ..

setlocal enabledelayedexpansion
set PATHS=
for /r "src" %%P in (*.c, *.h) do (set PATHS=!PATHS! "%%~fP")

echo Running cppcheck ...
call cppcheck %PATHS%
echo.

echo.
pause