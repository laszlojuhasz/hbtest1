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

cls
set HB_ARCHITECTURE=w32
set HB_COMPILER=bcc32

call bld.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
