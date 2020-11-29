@echo off
echo Building main.cpp...
g++ main.cpp -o main.exe -Os

if %errorlevel% neq 0 exit

echo Built (main.exe)