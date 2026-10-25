@echo off
setlocal
cd /d "%~dp0"
if not exist "build\Debug\equation-solver.exe" (
    echo Build the project first using the Windows steps in README.md.
    pause
    exit /b 1
)
echo === Equation Solver examples ===
"build\Debug\equation-solver.exe"
echo.
echo === Regression cases ===
if exist "build\Debug\solver-tests.exe" "build\Debug\solver-tests.exe"
echo.
echo See README.md for usage and supported equation formats.
pause
