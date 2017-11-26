@setlocal

@set HUGO=c:\hugo\hugo.exe

start "" http://localhost:1313

%HUGO% server ^
  --disableFastRender ^
  --buildDrafts ^
  --forceSyncStatic ^
  --noHTTPCache ^
  --source %CD%\input\

@endlocal