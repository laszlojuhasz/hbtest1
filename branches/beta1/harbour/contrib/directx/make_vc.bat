@echo off
rem
rem $Id: make_vc.bat 7844 2007-10-21 09:23:41Z vszakats $
rem

:BUILD

   nmake /f makefile.vc %1 %2 %3 > make_vc.log
   if errorlevel 1 goto BUILD_ERR

:BUILD_OK

   copy ..\..\lib\vc\hbwin32ddrw.lib ..\..\lib\*.* >nul
   goto EXIT

:BUILD_ERR

   notepad make_vc.log

:EXIT

