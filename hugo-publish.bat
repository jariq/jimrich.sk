@setlocal

@set HUGO=c:\hugo\hugo.exe

rmdir /S /Q output
mkdir output

%HUGO% ^
  --source %CD%\input\ ^
  --destination %CD%\output\

del /S /Q output\.gitkeep

@endlocal