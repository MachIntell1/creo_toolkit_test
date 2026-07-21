@echo off
REM ============================================================
REM Build script for MachIntell J-Link Test Application
REM 
REM PREREQUISITES:
REM   - Java JDK 11+ (or JRE that comes with Creo)
REM   - Creo 11 installed with J-Link / OTK Java
REM
REM USAGE:
REM   1. Edit CREO_COMMON path below
REM   2. Edit JAVA_HOME if needed
REM   3. Run: build.bat
REM ============================================================

REM ---- CONFIGURATION (Edit these paths) ----

REM Path to Creo's Common Files
SET CREO_COMMON=C:\Program Files\PTC\Creo 11.0.0.0\Common Files

REM Java - try Creo's bundled Java first, then system Java
IF EXIST "%CREO_COMMON%\..\Parametric\bin\jre\bin\javac.exe" (
    SET JAVAC="%CREO_COMMON%\..\Parametric\bin\jre\bin\javac.exe"
) ELSE IF EXIST "%JAVA_HOME%\bin\javac.exe" (
    SET JAVAC="%JAVA_HOME%\bin\javac.exe"
) ELSE (
    SET JAVAC=javac
)

REM ---- FIND J-LINK JAR ----
REM Creo 11 puts pfc.jar in one of these locations:
SET PFC_JAR=

IF EXIST "%CREO_COMMON%\text\java\pfc.jar" (
    SET PFC_JAR=%CREO_COMMON%\text\java\pfc.jar
) ELSE IF EXIST "%CREO_COMMON%\otk\otk_java\pfc.jar" (
    SET PFC_JAR=%CREO_COMMON%\otk\otk_java\pfc.jar
) ELSE IF EXIST "%CREO_COMMON%\jlink\pfc.jar" (
    SET PFC_JAR=%CREO_COMMON%\jlink\pfc.jar
)

echo.
echo ============================================
echo  MachIntell J-Link Test - Build
echo ============================================
echo.

IF "%PFC_JAR%"=="" (
    echo [ERROR] Cannot find pfc.jar in Creo installation.
    echo.
    echo Searched:
    echo   %CREO_COMMON%\text\java\pfc.jar
    echo   %CREO_COMMON%\otk\otk_java\pfc.jar
    echo   %CREO_COMMON%\jlink\pfc.jar
    echo.
    echo Please find pfc.jar on your system and set PFC_JAR manually in this script.
    echo   To search: dir /s /b "C:\Program Files\PTC\pfc.jar"
    echo.
    pause
    exit /b 1
)

echo Creo Common : %CREO_COMMON%
echo PFC JAR     : %PFC_JAR%
echo Java Compiler: %JAVAC%
echo.

REM ---- COMPILE ----
echo [1/1] Compiling Java source...

IF NOT EXIST classes mkdir classes

%JAVAC% -cp "%PFC_JAR%" -d classes src\com\machintell\creotest\MachIntellTest.java

IF ERRORLEVEL 1 (
    echo.
    echo [FAILED] Compilation error. Check messages above.
    echo.
    echo Common fixes:
    echo   - Ensure JAVA_HOME is set or JDK is in PATH
    echo   - Ensure pfc.jar path is correct
    pause
    exit /b 1
)

echo.
echo ============================================
echo  BUILD SUCCESSFUL!
echo ============================================
echo.
echo Compiled classes in: classes\
echo.
echo NEXT STEPS:
echo   1. Start Creo 11
echo   2. Go to: Utilities ^> Auxiliary Applications
echo   3. Click "Register" ^> browse to machintell_jlink_test.dat
echo   4. Click "Start"
echo   5. Look for "MachIntell J-Link Test" in Utilities menu
echo   6. Click it and verify the popup + log file
echo.
pause
