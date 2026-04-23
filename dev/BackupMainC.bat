@echo off
cd /d %~dp0 & REM goto /dev directory where the script is
type ..\main.c > ..\main.c.bak
exit
