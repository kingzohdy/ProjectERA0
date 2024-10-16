@rem Script to build LuaJIT under "Visual Studio .NET Command Prompt".
@rem Do not run from this directory; run it from the toplevel: etc\luavs.bat .
@rem It creates luadll_d.dll, luadll_d.lib and luajit.exe in src.
@rem (contributed by David Manura and Mike Pall)

@setlocal
@set MYCOMPILE=cl /nologo /MD /O2 /W3 /c /D_CRT_SECURE_NO_DEPRECATE /I . /I ..\dynasm
@set MYLINK=link /nologo
@set MYMT=mt /nologo

cd src
%MYCOMPILE% /DLUA_BUILD_AS_DLL l*.c
del lua.obj luac.obj
%MYLINK% /DLL /out:luadll_d.dll l*.obj
if exist luadll_d.dll.manifest^
  %MYMT% -manifest luadll_d.dll.manifest -outputresource:luadll_d.dll;2
%MYCOMPILE% /DLUA_BUILD_AS_DLL lua.c
%MYLINK% /out:luajit.exe lua.obj luadll_d.lib
if exist luajit.exe.manifest^
  %MYMT% -manifest luajit.exe.manifest -outputresource:luajit.exe
del *.obj *.manifest
cd ..
