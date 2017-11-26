@setlocal

@set arg1=%1
@set HUGO=c:\hugo\hugo.exe

%HUGO% new %arg1%.md ^
  --source %CD%\input\

mkdir %CD%\input\static\%arg1%
type NUL > %CD%\input\static\%arg1%\.gitkeep

@endlocal