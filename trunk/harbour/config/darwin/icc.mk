#
# $Id$
#

ifeq ($(HB_BUILD_MODE),cpp)
   HB_CMP := icpc
else
   HB_CMP := icc
endif

OBJ_EXT := .o
LIB_PREF := lib
LIB_EXT := .a

CC := $(HB_CCACHE) $(HB_CMP)
CC_IN := -c
CC_OUT := -o

CPPFLAGS := -I. -I$(HB_INC_COMPILE)
CFLAGS :=
LDFLAGS :=

CFLAGS += -D_GNU_SOURCE

ifneq ($(HB_BUILD_WARN),no)
   #CFLAGS += -w2 -Wall
endif

ifneq ($(HB_BUILD_OPTIM),no)
   CFLAGS += -O3
endif

# uncomment this if you want to force relocateable code for .so libs
# it's necessary on some platforms but can reduce performance
#CFLAGS += -fPIC

ifeq ($(HB_BUILD_DEBUG),yes)
   CFLAGS += -g
endif

LD := $(HB_CCACHE) $(HB_CMP)
LD_OUT := -o

LIBPATHS := $(LIB_DIR)

LDLIBS := $(foreach lib,$(LIBS) $(SYSLIBS),-l$(lib))
LDFLAGS += $(foreach dir,$(LIBPATHS) $(SYSLIBPATHS),-L$(dir))

AR := libtool
ARFLAGS :=
AR_RULE = $(AR) -static $(ARFLAGS) $(HB_USER_AFLAGS) -o $(LIB_DIR)/$@ $(^F) || ( $(RM) $(LIB_DIR)/$@ && false )

DY := $(AR)
DFLAGS := -dynamic -flat_namespace -undefined warning -multiply_defined suppress -single_module
DY_OUT := -o$(subst x,x, )
DLIBS :=

# NOTE: The empty line directly before 'endef' HAVE TO exist!
define dyn_object
   @$(ECHO) $(ECHOQUOTE)$(subst \,/,$(file))$(ECHOQUOTE) >> __dyn__.tmp

endef
define create_dynlib
   $(if $(wildcard __dyn__.tmp),@$(RM) __dyn__.tmp,)
   $(foreach file,$^,$(dyn_object))
   $(DY) $(DFLAGS) -install_name "harbour$(DYN_EXT)" -compatibility_version $(HB_VER_MAJOR).$(HB_VER_MINOR) -current_version $(HB_VER_MAJOR).$(HB_VER_MINOR).$(HB_VER_RELEASE) $(HB_USER_DFLAGS) $(DY_OUT)$(DYN_DIR)/$@ -filelist __dyn__.tmp
endef

DY_RULE = $(create_dynlib)

include $(TOP)$(ROOT)config/rules.mk
