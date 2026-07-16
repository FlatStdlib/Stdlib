@echo off 2> $null

ls /bin >nul 2>&1; sudo make; exit 0 # DO STUFF IN THE PARATHENSIS AS FUNCTION ( WINDOWS IGNORES THIS )

if "%OS%"=="Windows_NT" call :myFunction
goto :eof

:myFunction
@echo ALGO THE GOD HERE
exit /b
