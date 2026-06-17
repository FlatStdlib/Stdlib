# Linux Use: null=/dev/null bash test.bat
# Windows Use: cmd /c test.bat
@echo off 2> $null

# DO STUFF IN THE PARATHENSIS AS FUNCTION ( WINDOWS IGNORES THIS )
ls /bin >nul 2>&1; $(touch test.file); exit 0

if "%OS%"=="Windows_NT" call :myFunction
goto :eof

:myFunction
@echo Function executed
exit /b