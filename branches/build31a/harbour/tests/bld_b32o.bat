@echo off
rem
rem $Id$
rem

..\bin\harbour %1 /n /i..\include /gf
tlink32 -L..\lib\b32;c:\bc5\lib c:\bc5\lib\c0x32.obj %1.obj hvm.obj,%1.exe,, harbour.lib terminal.lib common.lib import32.lib cw32mt.lib
del %1.obj

