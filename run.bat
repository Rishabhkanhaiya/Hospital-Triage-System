@echo off
echo ======================================================================
echo    COMPILING & RUNNING HOSPITAL EMERGENCY TRIAGE SYSTEM (OOPS C++)
echo ======================================================================
g++ -Wall -Wextra simple_emergency_triage.cpp -o simple_triage.exe

if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Compiled successfully! Launching triage system...
    echo ======================================================================
    simple_triage.exe
) else (
    echo [ERROR] Compilation failed.
)
pause
