@echo off
rem
rem $Id$
rem

..\bin\harbour %1 %2 %3 /n /i..\include
bcc32 %1.c -e%1.exe -O2 -I..\include -L..\lib\b32 -5 -tW common.lib dbfcdx.lib dbfntx.lib debug.lib harbour.lib pp.lib macro.lib terminal.lib tools.lib
rem del %1.c
