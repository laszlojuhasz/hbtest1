@echo off
rem
rem $Id$
rem

rem *******************************************************
rem Bat file for creating (almost all) contrib libs
rem for Harbour Project for Microsoft Visual C/C++
rem *******************************************************

rem *******************************************************
rem Copyright 2007 Marek Paliwoda (mpaliwoda "at" interia "dot" pl)
rem See doc/license.txt for licensing terms.
rem *******************************************************

rem *******************************************************
rem The compilation is done in three steps. PLEASE DO NOT MODIFY IT
rem or you will break a Windows9x command.com line length limit !!!
rem *******************************************************

set HB_SHOW_ERRORS=no

rem *******************************************************
rem Creating a worker bat file ...
rem *******************************************************

set __BATWORKER__=_hbwrk_.bat

echo @echo off                                          >%__BATWORKER__%
echo if %%1x == x goto EXIT                            >>%__BATWORKER__%
echo if not exist %%1\make_vc.bat goto EXIT            >>%__BATWORKER__%
echo echo Entering: %%1                                >>%__BATWORKER__%
echo cd %%1                                            >>%__BATWORKER__%
echo call make_vc.bat %%2 %%3 %%4 %%5 %%6 %%7 %%8 %%9  >>%__BATWORKER__%
echo cd ..                                             >>%__BATWORKER__%
echo :EXIT                                             >>%__BATWORKER__%

rem *******************************************************
rem Compiling contrib dirs ...
rem *******************************************************

set _HB_DIRS=rddado hbbmcdx hbbtree gtwvg hbct hbgt hbmisc hbnf hbmsql hbmzip
for %%n in ( %_HB_DIRS% ) do %COMSPEC% /c %__BATWORKER__% %%n %1 %2 %3 %4 %5 %6 %7 %8 %9

set _HB_DIRS=hbole hbziparch hbodbc hbtpathy hbtip hbvpdf hbw32 hbwhat32 xhb
for %%n in ( %_HB_DIRS% ) do %COMSPEC% /c %__BATWORKER__% %%n %1 %2 %3 %4 %5 %6 %7 %8 %9

set _HB_DIRS=hbclipsm hbw32ddr
if not "%HB_DIR_APOLLO%"    == "" set _HB_DIRS=%_HB_DIRS% hbapollo
if not "%HB_DIR_FIREBIRD%"  == "" set _HB_DIRS=%_HB_DIRS% hbfbird
if not "%HB_DIR_FREEIMAGE%" == "" set _HB_DIRS=%_HB_DIRS% hbfimage
if not "%HB_DIR_GD%"        == "" set _HB_DIRS=%_HB_DIRS% hbgd
if not "%HB_DIR_MYSQL%"     == "" set _HB_DIRS=%_HB_DIRS% hbmysql
if not "%HB_DIR_PGSQL%"     == "" set _HB_DIRS=%_HB_DIRS% hbpgsql
if not "%HB_DIR_ADS%"       == "" set _HB_DIRS=%_HB_DIRS% rddads
if not "%HB_DIR_LIBHARU%"   == "" set _HB_DIRS=%_HB_DIRS% hbhpdf
for %%n in ( %_HB_DIRS% ) do %COMSPEC% /c %__BATWORKER__% %%n %1 %2 %3 %4 %5 %6 %7 %8 %9

rem *******************************************************
rem Cleaning ...
rem *******************************************************

del %__BATWORKER__% > nul

set _HB_DIRS=
set __BATWORKER__=
set HB_SHOW_ERRORS=
