
@if (%1) == () goto usage

@rem Creating a variable named 'VSINSTALLDIR' breaks vcvarsall.bat, so we use a different name
@set myVSINSTALLDIR=%1
@set VCVARSOPTS=%2

@set use_sdk=no
@if "%VCVARSOPTS%"=="x86_amd64" (
if exist "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" (
  @set use_sdk=yes
))
@if %use_sdk%==yes (
call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /x64
) else (
if exist %myVSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat (
call %myVSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat %VCVARSOPTS%
) else (
if exist %VSINSTALLDIR%\VC\vcvarsall.bat (
call %myVSINSTALLDIR%\VC\vcvarsall.bat %VCVARSOPTS%
) else (
if exist %VSINSTALLDIR%\Common7\Tools\vsvars32.bat (
call %myVSINSTALLDIR%\Common7\Tools\vsvars32.bat
) else (
if exist %VSINSTALLDIR%\VC98\bin\vcvars32.bat (
call %myVSINSTALLDIR%\VC98\bin\vcvars32.bat
) else (
@echo "Could not find either of these files:"
@echo %myVSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat
@echo %myVSINSTALLDIR%\VC\vcvarsall.bat
@echo %myVSINSTALLDIR%\Common7\Tools\vsvars32.bat
@echo %myVSINSTALLDIR%\VC98\bin\vcvars32.bat
goto :eof
)))))
@rem /MD : link with MSVCRT.lib
cl  /c /MD /O2 /DNDEBUG random.c
link random.obj /dll /def:random.def
goto :eof

:usage
@echo Error in script usage.  The correct usage is:
@echo    %0 (visual studio root folder) [vcvarsall.bat options]
