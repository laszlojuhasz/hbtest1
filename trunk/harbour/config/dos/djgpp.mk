#
# $Id$
#

ifeq ($(HB_BUILD_MODE),cpp)
   HB_CMP := gpp
else
   HB_CMP := gcc
endif

OBJ_EXT := .o
LIB_PREF := lib
LIB_EXT := .a

CC := $(HB_CCPATH)$(HB_CCPREFIX)$(HB_CMP)$(HB_CCPOSTFIX)
CC_IN := -c
CC_OUT := -o

CPPFLAGS := -I. -I$(HB_INC_COMPILE)
CFLAGS :=
LDFLAGS :=

ifneq ($(HB_BUILD_WARN),no)
   CFLAGS += -Wall -W
endif

ifneq ($(HB_BUILD_OPTIM),no)
   CFLAGS += -O3
endif

ifeq ($(HB_BUILD_DEBUG),yes)
   CFLAGS += -g
endif

ifneq ($(filter $(HB_BUILD_STRIP),all lib),)
   ARSTRIP = $(HB_CCPATH)$(HB_CCPREFIX)strip -S $(LIB_DIR)/$@
else
   ARSTRIP := @$(ECHO) .
endif
ifneq ($(filter $(HB_BUILD_STRIP),all bin),)
   LDSTRIP := -s
   DYSTRIP := -s
endif

SYSLIBPATHS :=

ifneq ($(HB_LINKING_RTL),)
   ifeq ($(HB_LIBNAME_CURSES),)
      HB_LIBNAME_CURSES := pdcurses
   endif
   ifneq ($(filter gtcrs, $(LIBS)),)
      SYSLIBS += $(HB_LIBNAME_CURSES)
   endif
   ifneq ($(HB_HAS_WATT),)
      SYSLIBPATHS += $(HB_LIB_WATT)
      SYSLIBS += watt
   endif
endif

SYSLIBS += m

LD := $(HB_CCPATH)$(HB_CCPREFIX)$(HB_CMP)$(HB_CCPOSTFIX)
LD_OUT := -o

LIBPATHS := $(foreach dir,$(LIB_DIR) $(SYSLIBPATHS),-L$(dir))
LDLIBS := $(foreach lib,$(HB_USER_LIBS) $(LIBS) $(SYSLIBS),-l$(lib))

# NOTE: The empty line directly before 'endef' HAVE TO exist!
#       It causes that every command will be separated by LF
define lib_object
   @$(ECHO) $(ECHOQUOTE)ADDMOD $(file)$(ECHOQUOTE) >> __lib__.tmp

endef

# We have to use script to overcome the DOS limit of max 128 characters
# in commmand line
define create_library
   @$(ECHO) $(ECHOQUOTE)CREATE $(LIB_DIR)/$@$(ECHOQUOTE) > __lib__.tmp
   $(foreach file,$(^F),$(lib_object))
   @$(ECHO) $(ECHOQUOTE)SAVE$(ECHOQUOTE) >> __lib__.tmp
   @$(ECHO) $(ECHOQUOTE)END$(ECHOQUOTE) >> __lib__.tmp
   $(AR) $(ARFLAGS) $(HB_USER_AFLAGS) -M < __lib__.tmp
   $(ARSTRIP)
endef

# NOTE: The empty line directly before 'endef' HAVE TO exist!
define link_file
   @$(ECHO) $(ECHOQUOTE)$(file)$(ECHOQUOTE) >> __link__.tmp

endef

define link_exe_file
   @$(ECHO) $(ECHOQUOTE)$(LDFLAGS) $(HB_LDFLAGS) $(HB_USER_LDFLAGS) $(LD_OUT)$(BIN_DIR)/$@$(ECHOQUOTE) > __link__.tmp
   $(foreach file,$(^F),$(link_file))
   $(foreach file,$(LIBPATHS),$(link_file))
   $(foreach file,$(LDLIBS),$(link_file))
   -$(LD) @__link__.tmp
endef

AR := ar
ARFLAGS :=
AR_RULE = $(create_library)

LD_RULE = $(link_exe_file)

#DY := $(CC)
#DFLAGS := -Wl,-shared $(LIBPATHS)
#DY_OUT := -o$(subst x,x, )
#DLIBS := $(foreach lib,$(SYSLIBS),-l$(lib))
#
## NOTE: The empty line directly before 'endef' HAVE TO exist!
#define dyn_object
#   @$(ECHO) $(ECHOQUOTE)INPUT($(subst \,/,$(file)))$(ECHOQUOTE) >> __dyn__.tmp
#
#endef
#define create_dynlib
#   $(if $(wildcard __dyn__.tmp),@$(RM) __dyn__.tmp,)
#   $(foreach file,$^,$(dyn_object))
#   $(DY) $(DFLAGS) $(HB_USER_DFLAGS) $(DY_OUT)$(DYN_DIR)/$@ __dyn__.tmp $(DLIBS) $(DYSTRIP)
#endef
#
#DY_RULE = $(create_dynlib)

include $(TOP)$(ROOT)config/rules.mk
