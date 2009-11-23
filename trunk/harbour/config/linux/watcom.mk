#
# $Id$
#

# GNU Make file for Open Watcom C/C++ compiler

OBJ_EXT := .o
LIB_PREF :=
LIB_EXT := .lib

ifeq ($(HB_BUILD_MODE),c)
   CC := wcc386
endif
ifeq ($(HB_BUILD_MODE),cpp)
   CC := wpp386
endif
# Build in C++ mode by default
ifeq ($(HB_BUILD_MODE),)
   CC := wpp386
endif
CC_IN :=
CC_OUT := -fo=

CFLAGS := -zq -bt=linux
LDFLAGS := OP quiet

ifneq ($(HB_BUILD_WARN),no)
   CFLAGS += -w3
else
   CFLAGS += -w0
endif

ifneq ($(HB_BUILD_OPTIM),no)
   # architecture flags
   CFLAGS += -6r -fp6

   # optimization flags
   # don't enable -ol optimization in OpenWatcom 1.1 - gives buggy code
   # -oxaht
   CFLAGS += -onaehtr -s -ei -zp4 -zt0
   #CFLAGS += -obl+m
   ifeq ($(CC),wpp386)
      CFLAGS += -oi+
   else
      CFLAGS += -oi
   endif
else
   CFLAGS += -3r
endif

CFLAGS += -i. -i$(HB_INC_COMPILE)

ifeq ($(HB_BUILD_DEBUG),yes)
   CFLAGS += -d2
endif

LD := wlink
ifeq ($(HB_BUILD_DEBUG),yes)
   LDFLAGS += DEBUG ALL
endif
LDFLAGS += SYS linux

LDLIBS := $(HB_USER_LIBS)
LDLIBS += $(foreach lib,$(LIBS),$(LIB_DIR)/$(lib))

DY := $(LD)
DFLAGS := OP quiet FORM elf dll LIBPATH $(WATCOM)/lib386 LIBPATH $(WATCOM)/lib386/linux OP exportall
DY_OUT :=
DLIBS :=

# NOTE: The empty line directly before 'endef' HAVE TO exist!
define dyn_object
   @$(ECHO) $(ECHOQUOTE)FILE '$(file)'$(ECHOQUOTE) >> __dyn__.tmp

endef
define create_dynlib
   $(if $(wildcard __dyn__.tmp),@$(RM) __dyn__.tmp,)
   $(foreach file,$^,$(dyn_object))
   $(DY) $(DFLAGS) $(HB_USER_DFLAGS) NAME '$(subst /,$(DIRSEP),$(DYN_DIR)/$@)' @__dyn__.tmp
endef

DY_RULE = $(create_dynlib)

include $(TOP)$(ROOT)config/common/watcom.mk
