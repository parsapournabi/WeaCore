@echo off
setlocal EnableDelayedExpansion

REM =============================================================================
REM Defaults
REM =============================================================================

set "QRC_FILE=resources.qrc"
set "DIRECTORIES=qml shaders"

REM =============================================================================
REM Help
REM =============================================================================

if "%~1"=="-h" goto :help
if "%~1"=="--help" goto :help

:parse

if "%~1"=="" goto :generate

if "%~1"=="-q" (
    shift
    if "%~1"=="" (
        echo Missing qrc file.
        exit /b 1
    )
    set "QRC_FILE=%~1"
    shift
    goto :parse
)

if "%~1"=="--qrc" (
    shift
    if "%~1"=="" (
        echo Missing qrc file.
        exit /b 1
    )
    set "QRC_FILE=%~1"
    shift
    goto :parse
)

if "%~1"=="-d" (
    set "DIRECTORIES="
    shift

:dirs_loop
    if "%~1"=="" goto :parse

    if "%~1"=="-q" goto :parse
    if "%~1"=="--qrc" goto :parse
    if "%~1"=="-h" goto :parse
    if "%~1"=="--help" goto :parse

    set "DIRECTORIES=!DIRECTORIES! %~1"
    shift
    goto :dirs_loop
)

if "%~1"=="--dirs" (
    set "DIRECTORIES="
    shift

:dirs_loop2
    if "%~1"=="" goto :parse

    if "%~1"=="-q" goto :parse
    if "%~1"=="--qrc" goto :parse
    if "%~1"=="-h" goto :parse
    if "%~1"=="--help" goto :parse

    set "DIRECTORIES=!DIRECTORIES! %~1"
    shift
    goto :dirs_loop2
)

echo Unknown argument: %~1
echo.
goto :help

REM =============================================================================
REM Generate
REM =============================================================================

:generate

for %%F in ("%QRC_FILE%") do (
    if not "%%~dpF"=="" (
        if not exist "%%~dpF" mkdir "%%~dpF"
    )
)

(
echo ^<RCC^>
echo     ^<qresource prefix="/"^>

for %%D in (%DIRECTORIES%) do (

    if exist "%%D" (

        for /f "delims=" %%F in ('dir /b /s /a-d "%%D" ^| sort') do (

            set "FILE=%%F"
            set "FILE=!FILE:%CD%\=!"

            if "!FILE:~0,1!"=="\" set "FILE=!FILE:~1!"

            echo         ^<file^>!FILE!\^</file^>
        )
    )
)

echo     ^</qresource^>
echo ^</RCC^>

) > "%QRC_FILE%"

echo Generated: %QRC_FILE%
exit /b 0

REM =============================================================================
REM Help
REM =============================================================================

:help

echo Usage:
echo     update-qrc.bat [OPTIONS]
echo.
echo Description:
echo     Recursively scans directories and generates a Qt Resource ^(.qrc^) file.
echo.
echo Options:
echo.
echo     -d, --dirs ^<dir1^> ^<dir2^> ...
echo         Directories to scan.
echo         Default:
echo             qml shaders
echo.
echo     -q, --qrc ^<file^>
echo         Target qrc file.
echo         Default:
echo             resources.qrc
echo.
echo     -h, --help
echo         Show this help.
echo.
echo Examples:
echo.
echo     update-qrc.bat
echo.
echo     update-qrc.bat --dirs qml shaders scripts
echo.
echo     update-qrc.bat --dirs qml shaders --qrc resources.qrc

exit /b 0
