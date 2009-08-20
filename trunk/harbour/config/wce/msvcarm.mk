#
# $Id$
#

# Work in progress. Please test.

OBJ_EXT := .obj
LIB_PREF :=
LIB_EXT := .lib

HB_DYN_COPT := -DHB_DYNLIB

ifeq ($(HB_VISUALC_VER_PRE80),)
   CC := cl.exe
else
   CC := clarm.exe
endif
CC_IN := -c
CC_OUT := -Fo

CPPFLAGS := -I. -I$(HB_INC_COMPILE)
CFLAGS := -nologo -DWINCE -D_WINCE -D_WINDOWS -DARM -D_ARM_ -DARMV4 -D_M_ARM -D_UNICODE -D_UWIN -DUNDER_CE
LDFLAGS :=

ifeq ($(HB_BUILD_MODE),c)
   CFLAGS += -TC
endif
ifeq ($(HB_BUILD_MODE),cpp)
   CFLAGS += -TP
endif

ifneq ($(HB_BUILD_WARN),no)
   CFLAGS += -W4
endif

ifneq ($(HB_BUILD_OPTIM),no)
   ifeq ($(HB_VISUALC_VER_PRE80),)
      CFLAGS += -Od -Os -Gy -GS- -EHsc-
   else
      CFLAGS += -Oxsb1 -EHsc -YX -GF
   endif
endif

ifeq ($(HB_BUILD_DEBUG),yes)
   CFLAGS += -Zi
endif

ifeq ($(HB_VISUALC_VER_PRE80),)
   LD := cl.exe
else
   LD := clarm.exe
endif
LD_OUT := -Fe

SYSLIBS += corelibc

LIBPATHS := /libpath:$(LIB_DIR)
LDLIBS := $(foreach lib,$(LIBS) $(SYSLIBS),$(lib)$(LIB_EXT))

LDFLAGS += /nologo /link /subsystem:windowsce /nodefaultlib:oldnames.lib /nodefaultlib:kernel32.lib
ifeq ($(HB_VISUALC_VER_PRE80),)
   LDFLAGS += /manifest:no
endif
LDFLAGS += $(LIBPATHS)

AR := lib.exe
ARFLAGS :=
AR_RULE = $(AR) $(ARFLAGS) $(HB_USER_AFLAGS) /nologo /out:$(LIB_DIR)/$@ $(^F) || $(RM) $(LIB_DIR)/$@

include $(TOP)$(ROOT)config/rules.mk
