@echo off

REM === Get paths ===
set SCRIPT_DIR=%~dp0
set TARGET=%SCRIPT_DIR%bin\Release\ChaosGroove.exe
set ICON=%SCRIPT_DIR%Gfx\icon.ico
set SHORTCUT_NAME=ChaosGroove

REM === Create shortcut using PowerShell ===
powershell -NoProfile -Command ^
"$s=(New-Object -COM WScript.Shell).CreateShortcut('%SCRIPT_DIR%\%SHORTCUT_NAME%.lnk'); ^
$s.TargetPath='%TARGET%'; ^
$s.WorkingDirectory='%SCRIPT_DIR%'; ^
$s.IconLocation='%ICON%'; ^
$s.Save()"

echo Shortcut created
pause