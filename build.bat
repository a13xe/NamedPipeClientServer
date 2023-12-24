@echo off

REM Compile Server Application
g++ -o server.exe server.cpp -lws2_32

REM Check if Server Compiled Successfully
if %errorlevel% neq 0 (
    echo Server Compilation Failed
    exit /b %errorlevel%
)

REM Compile Client Application
g++ -o client.exe client.cpp -lws2_32

REM Check if Client Compiled Successfully
if %errorlevel% neq 0 (
    echo Client Compilation Failed
    exit /b %errorlevel%
)
