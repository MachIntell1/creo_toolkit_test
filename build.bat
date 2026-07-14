@echo off
REM ============================================================
REM Build script for MachIntell Creo Toolkit Test Application
REM 
REM PREREQUISITES:
REM   1. Visual Studio 2019 or 2022 with "Desktop development with C++" 
REM   2. Creo 11 installed with Pro/TOOLKIT (Creo Toolkit)
REM
REM HOW TO USE:
REM   1. Open "x64 Native Tools Command Prompt for VS 2022"
REM      (Start Menu > Visual Studio 2022 > x64 Native Tools...)
REM   2. Edit CREO_INSTALL path below to match your installation
REM   3. cd to this folder
REM   4. Run: build.bat
REM ============================================================

REM ---- CONFIGURATION ----
REM Edit this to your Creo installation path:
SET CREO_INSTALL=C:\Program Files\PTC\Creo 11.0.0.0\Common Files

REM Pro/TOOLKIT directory (usually under Common Files)
SET PROTOOLKIT_DIR=%CREO_INSTALL%\protoolkit

REM ---- VERIFY SETUP ----
echo.
echo ============================================
echo  MachIntell Creo Toolkit Test - Build
echo ============================================
echo.

IF NOT EXIST "%PROTOOLKIT_DIR%\includes\ProToolkit.h" (
    echo [ERROR] Cannot find ProToolkit.h at:
    echo         %PROTOOLKIT_DIR%\includes\ProToolkit.h
    echo.
    echo Please edit CREO_INSTALL in this build.bat file.
    echo.
    echo Try one of these paths:
    echo   C:\Program Files\PTC\Creo 11.0.0.0\Common Files
    echo   C:\PTC\Creo 11.0.0.0\Common Files
    echo   D:\Program Files\PTC\Creo 11.0.0.0\Common Files
    echo   C:\Program Files\PTC\Creo 11.0\Common Files
    echo.
    echo Also verify Pro/TOOLKIT was included in Creo installation.
    echo You should see a "protoolkit" subfolder in Common Files.
    echo.
    pause
    exit /b 1
)

where cl >nul 2>&1
IF ERRORLEVEL 1 (
    echo [ERROR] 'cl' compiler not found.
    echo.
    echo You must run this from "x64 Native Tools Command Prompt for VS 2022"
    echo   Start Menu ^> Visual Studio 2022 ^> x64 Native Tools Command Prompt
    echo.
    pause
    exit /b 1
)

echo Creo Install : %CREO_INSTALL%
echo Toolkit Dir  : %PROTOOLKIT_DIR%
echo.

REM ---- BUILD ----
IF NOT EXIST bin mkdir bin

echo [1/2] Compiling test_app.c ...
cl /c /MD /W3 /DPRO_USE_VAR_ARGS ^
   /I"%PROTOOLKIT_DIR%\includes" ^
   /I"%PROTOOLKIT_DIR%\protk_appls\includes" ^
   src\test_app.c /Fo"bin\test_app.obj" /nologo

IF ERRORLEVEL 1 (
    echo.
    echo [FAILED] Compilation error. Check messages above.
    pause
    exit /b 1
)

echo [2/2] Linking machintell_test.dll ...
link /DLL /OUT:"bin\machintell_test.dll" ^
     bin\test_app.obj ^
     "%PROTOOLKIT_DIR%\x86e_win64\obj\protoolkit.lib" ^
     "%PROTOOLKIT_DIR%\x86e_win64\obj\prodev_dll.lib" ^
     kernel32.lib user32.lib wsock32.lib advapi32.lib ^
     mpr.lib winspool.lib netapi32.lib psapi.lib ^
     gdi32.lib shell32.lib comdlg32.lib ole32.lib ^
     /nologo

IF ERRORLEVEL 1 (
    echo.
    echo [FAILED] Link error. Check messages above.
    echo.
    echo Common fix: Verify that these files exist:
    echo   %PROTOOLKIT_DIR%\x86e_win64\obj\protoolkit.lib
    echo   %PROTOOLKIT_DIR%\x86e_win64\obj\prodev_dll.lib
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================
echo  BUILD SUCCESSFUL!
echo ============================================
echo.
echo Output: bin\machintell_test.dll
echo.
echo NEXT STEPS:
echo   1. Start Creo 11
echo   2. Go to: File ^> Options ^> Configuration Editor
echo      OR: Tools ^> Auxiliary Applications (older UI)
echo   3. Click "Register" and browse to machintell_test.dat
echo   4. Click "Start"
echo   5. Look for "MachIntell Test" in Utilities menu
echo   6. Click it and verify the popup + log file
echo.
pause
