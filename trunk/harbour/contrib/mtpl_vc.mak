#
# $Id$
#

# Makefile common section for Harbour Project Contrib libs
# for Microsoft Visual C
# --------------------------------------------------------

# NOTE: You can use these envvars to configure the make process:
#   	(note that these are all optional)
#
#   	CFLAGS  		 - Extra C compiler options for libraries and for
#   							   executables
#   	C_USR   		 - Extra C compiler options for libraries and for
#   							   executables (GNU make compatible envvar)
#   	CLIBFLAGS    - Extra C compiler options for the libraries
#   	HARBOURFLAGS - Extra Harbour compiler options
#   	PRG_USR 		 - Extra Harbour compiler options
#   							   (GNU make compatible envvar)

# Visual C++ version
!ifndef HB_VISUALC_VER
HB_VISUALC_VER = 60
!endif

#**********************************************************

!ifndef HB_ROOT
HB_ROOT = ..\..
!endif

!include $(HB_ROOT)\contrib\mtpl.mak

#**********************************************************
# C compiler definition and C flags. These should never have to change.
#**********************************************************

CC     = cl.exe
LINKER = link.exe
MKLIB  = lib.exe

#**********************************************************

CFLAGS  	   = -I$(INCLUDE_DIR) -W3 -nologo $(C_USR) $(CFLAGS)
CLIBFLAGS      = -c $(CFLAGS) $(CLIBFLAGS)
CLIBFLAGSDEBUG = -Zi $(CLIBFLAGS)
HARBOURFLAGS   = -i$(INCLUDE_DIR) -n -q0 -w2 -es2 -gc0 $(PRG_USR) $(HARBOURFLAGS)
LDFLAGS 	   = $(LDFLAGS)

#**********************************************************
# COMPILE Rules
#**********************************************************

#**********************************************************
# General *.c --> *.obj COMPILE rule for STATIC Libraries
{.}.c{$(OBJ_DIR)}$(OBJEXT)::
   $(CC) $(CLIBFLAGS) -Fo$(OBJ_DIR)\ $<
#**********************************************************
# General *.cpp --> *.obj COMPILE rule for STATIC Libraries
{.}.cpp{$(OBJ_DIR)}$(OBJEXT)::
   $(CC) $(CLIBFLAGS) -Fo$(OBJ_DIR)\ $<
#**********************************************************
# General *.prg --> *.obj COMPILE rule for STATIC Libraries
{.}.prg{$(OBJ_DIR)}$(OBJEXT):
   $(HARBOUR_EXE) $(HARBOURFLAGS) -o$(OBJ_DIR)\ $<
   $(CC) $(CLIBFLAGS) -Fo$(OBJ_DIR)\ $(OBJ_DIR)\$(*B).c
#**********************************************************
# General Library BUILD rule - does not work
#{$(OBJ_DIR)}$(OBJEXT){$(LIB_PATH)}.lib:
#   IF EXIST "$@" $(DEL) "$@" > nul
#   $(MKLIB) /out:$@ @<<
#$**
#<<$(KEEPSTATE)
#**********************************************************

!include common.mak

#**********************************************************
$(LIB_PATH) : $(LIB_OBJS)
   IF EXIST "$@" $(DEL) "$@" > nul
   $(MKLIB) /out:$@ @<<
$**
<<$(KEEPSTATE)
#**********************************************************

#**********************************************************
# CLEAN rule(s)
#**********************************************************

clean: doClean
CLEAN: doClean

doClean:
   $(ECHO) echo off 					  > delone.bat
   $(ECHO) if "%1" == "" goto skip  	 >> delone.bat
   $(ECHO) if exist %1.c   $(DEL) %1.c   >> delone.bat
   $(ECHO) if exist %1.obj $(DEL) %1.obj >> delone.bat
   $(ECHO) :skip						 >> delone.bat
   type <<alldel.bat
call delone.bat $(LIB_OBJS:.obj=^
call delone.bat )
<<KEEP
   $(ECHO) @echo off > delall.bat
   type alldel.bat  >> delall.bat
   delall.bat
   if exist $(LIB_PATH) $(DEL) $(LIB_PATH) > nul
   if exist $(LIB_PATH) $(DEL) $(LIB_PATH:.lib=.exp) > nul
   $(DEL) alldel.bat > nul
   $(DEL) delone.bat > nul
   $(DEL) delall.bat > nul

#**********************************************************

#**********************************************************
# INSTALL rule(s)
#**********************************************************

install: doInstall
INSTALL: doInstall

doInstall:
   if exist $(LIB_DIR)\$(LIBNAME)$(LIBEXT) copy $(LIB_DIR)\$(LIBNAME)$(LIBEXT) $(HB_LIB_INSTALL) > nul
   if exist *.api copy *.api $(HB_INC_INSTALL) > nul
   if exist *.ch  copy *.ch  $(HB_INC_INSTALL) > nul
