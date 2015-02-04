@echo off
set DIR=%~dp0
set PATH=%PATH%;%DIR%..\src
echo %PATH%
%DIR%\tests.exe
