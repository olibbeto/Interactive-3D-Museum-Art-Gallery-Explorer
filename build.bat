@echo off
echo Compiling Interactive 3D Museum...
g++ src\*.cpp -o Interactive3DMuseum.exe -lfreeglut -lopengl32 -lglu32
if %ERRORLEVEL% EQU 0 (
    echo Compilation successful! Running...
    Interactive3DMuseum.exe
) else (
    echo Compilation failed.
)
pause
