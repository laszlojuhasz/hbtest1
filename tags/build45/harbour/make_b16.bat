@echo off
rem
rem $Id$
rem

rem ---------------------------------------------------------------
rem This is a generic template file, if it doesn't fit your own needs 
rem please DON'T MODIFY IT.
rem
rem Instead, make a local copy and modify that one, or make a call to 
rem this batch file from your customized one. [vszakats]
rem ---------------------------------------------------------------

if "%1" == "clean" goto CLEAN
if "%1" == "CLEAN" goto CLEAN

:BUILD

   make -fmakefile.bc -DB16 %1 %2 %3 > make_b16.log
   if errorlevel 1 goto BUILD_ERR

:BUILD_OK

   copy bin\b16\*.exe bin\*.* > nul
   copy lib\b16\*.lib lib\*.* > nul
   goto EXIT

:BUILD_ERR

   edit make_b16.log
   goto EXIT

:CLEAN

   if exist bin\b16\*.exe del bin\b16\*.exe
   if exist bin\b16\*.tds del bin\b16\*.tds
   if exist bin\b16\*.map del bin\b16\*.map
   if exist lib\b16\*.lib del lib\b16\*.lib
   if exist lib\b16\*.bak del lib\b16\*.bak
   if exist obj\b16\*.obj del obj\b16\*.obj
   if exist obj\b16\*.c   del obj\b16\*.c
   if exist obj\b16\*.h   del obj\b16\*.h
   if exist make_b16.log  del make_b16.log
   goto EXIT

:EXIT

