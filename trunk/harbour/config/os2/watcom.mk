#
# $Id$
#

# GNU Make file for Open Watcom C/C++ compiler

OBJ_EXT := .obj
LIB_PREF :=
LIB_EXT := .lib

HB_DYN_COPT := -DHB_DYNLIB

ifeq ($(HB_BUILD_MODE),cpp)
   CC := wpp386
else
   CC := wcc386
endif
CC_IN :=
CC_OUT := -fo=

CFLAGS += -zq -bt=os2 -bm
LDFLAGS += OP quiet

ifneq ($(HB_BUILD_WARN),no)
   CFLAGS += -w3
else
   CFLAGS += -w1 -wcd201 -wcd367 -wcd368
   ifneq ($(HB_BUILD_MODE),cpp)
      CFLAGS += -wcd124 -wcd136
   endif
endif

ifneq ($(HB_BUILD_OPTIM),no)
   # architecture flags
   CFLAGS += -5r -fp5
   # NOTE: This is workaround for wrong code generated by OpenWatcom 1.8
   #       when -s switch is used for functions calling APIENTRY16 functions.
   #       Such code executed from non main thread causes GPF.
   #       So we turn off -s switch for modules affected:
   ifeq ($(filter $(LIBNAME),gtos2 gtstd),)
      CFLAGS += -s
   endif
   # optimization flags
   # don't enable -ol optimization in OpenWatcom 1.1 - gives buggy code
   # -oxaht
   CFLAGS += -onaehtr -ei -zp4 -zt0
   #CFLAGS += -obl+m
   ifeq ($(CC),wpp386)
      CFLAGS += -oi+
   else
      CFLAGS += -oi
   endif
else
   CFLAGS += -3r
endif

CFLAGS += -i. -i$(HB_HOST_INC)

ifeq ($(HB_BUILD_DEBUG),yes)
   CFLAGS += -d2
endif

RC := wrc
RC_OUT := -fo=
RCFLAGS += -I. -I$(HB_HOST_INC) -q -r -zm -bt=os2

LD := wlink
ifeq ($(HB_BUILD_DEBUG),yes)
   LDFLAGS += DEBUG ALL
endif
LDFLAGS += SYS os2v2

LDLIBS := $(HB_USER_LIBS)
LDLIBS += $(foreach lib,$(LIBS),$(LIB_DIR)/$(lib))

DY := $(LD)
DFLAGS += OP quiet SYS os2v2_dll
DY_OUT :=
DLIBS := $(HB_USER_LIBS)
DLIBS += $(foreach lib,$(LIBS),$(LIB_DIR)/$(lib))

# NOTE: The empty line directly before 'endef' HAVE TO exist!
define dynlib_object
   @$(ECHO) $(ECHOQUOTE)FILE '$(file)'$(ECHOQUOTE) >> __dyn__.tmp

endef
define create_dynlib
   $(if $(wildcard __dyn__.tmp),@$(RM) __dyn__.tmp,)
   $(foreach file,$^,$(dynlib_object))
   $(DY) $(DFLAGS) $(HB_USER_DFLAGS) NAME '$(subst /,$(DIRSEP),$(DYN_DIR)/$@)' OP implib='$(IMP_FILE)' @__dyn__.tmp
endef

DY_RULE = $(create_dynlib)

include $(TOP)$(ROOT)config/common/watcom.mk
