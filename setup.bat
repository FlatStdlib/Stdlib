@echo off 2> null

ls /bin >$null 2>&1; sudo make; exit 0 # DO STUFF IN THE PARATHENSIS AS FUNCTION ( WINDOWS IGNORES THIS )

if "%OS%"=="Windows_NT" call :setup_fsl
goto :eof

:setup_fsl

@REM Windows Sucks

@echo [31mSetting up FSL for Windows[0m
del $null
del null

cl >nul 2>&1
if errorlevel 0 (
    echo [32mMSVC Already Installed and Setup![0m
    goto build
)

for /f "delims=" %%A in ('where /r C:\ VsDevCmd.bat 2^>nul') do set "test=%%A"

if not defined test (
    echo MSVC Dev Setup Not Found!!
    goto build
)

echo [32mSetting up MSVC Variables![0m

call "%test%" -arch=x64

@REM if exist "C:\Program Files\Microsoft Visual Studio\" (
@REM     echo MSVC Installed.
@REM ) else (
@REM     echo MSVC Compiler Not Found!
@REM     exit /b 1
@REM )

:build
cl /I"headers" /LD src/c/internal_win.c src/c/allocator.c src/c/memory.c src/c/stdlib/char.c src/c/stdlib/int.c /Fe:build/fsl.dll
del *.obj
echo [32mFSL Setup Complete![0m
exit /b
