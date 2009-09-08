#
# $Id$
#

ifeq ($(HB_BUILD_MODE),cpp)
   HB_CMP := clang
else
   HB_CMP := clang
endif

OBJ_EXT := .o
LIB_PREF := lib
LIB_EXT := .a

CC := $(HB_CCACHE) $(HB_CCPREFIX)$(HB_CMP)$(HB_CCPOSTFIX)
CC_IN := -c
# NOTE: The ending space after -o is important, please preserve it.
#       Now solved with '$(subst x,x, )' expression.
CC_OUT := -o$(subst x,x, )

CPPFLAGS := -I. -I$(HB_INC_COMPILE) -DHB_CC_CLANG

# -fno-common enables building .dylib files
CPPFLAGS += -fno-common
LDFLAGS :=

ifneq ($(HB_BUILD_WARN),no)
   CPPFLAGS += -Wall -W
endif

ifneq ($(HB_BUILD_OPTIM),no)
   CPPFLAGS += -O3
endif

ifeq ($(HB_BUILD_DEBUG),yes)
   CPPFLAGS += -g
endif

# It's to avoid warning message generated when 'long double' is used
# remove it if you have newer compiler version
#CPPFLAGS += -Wno-long-double

LD := $(HB_CCACHE) $(HB_CCPREFIX)$(HB_CMP)$(HB_CCPOSTFIX)
LD_OUT := -o$(subst x,x, )

LIBPATHS := $(LIB_DIR)

LDLIBS := $(foreach lib,$(LIBS) $(SYSLIBS),-l$(lib))
LDFLAGS += $(foreach dir,$(LIBPATHS) $(SYSLIBPATHS),-L$(dir))

AR := libtool
ARFLAGS :=
AR_RULE = $(AR) -static $(ARFLAGS) $(HB_USER_AFLAGS) -o $(LIB_DIR)/$@ $(^F) || ( $(RM) $(LIB_DIR)/$@ && false )

DY := $(AR)
DFLAGS := -dynamic -flat_namespace -undefined warning -multiply_defined suppress -single_module $(foreach dir,$(SYSLIBPATHS),-L$(dir))
DY_OUT := -o$(subst x,x, )
DLIBS := $(foreach lib,$(SYSLIBS),-l$(lib))

DY_RULE = $(DY) $(DFLAGS) -install_name "harbour$(DYN_EXT)" -compatibility_version $(HB_VER_MAJOR).$(HB_VER_MINOR) -current_version $(HB_VER_MAJOR).$(HB_VER_MINOR).$(HB_VER_RELEASE) $(HB_USER_DFLAGS) $(DY_OUT)$(DYN_DIR)/$@ $^ $(DLIBS)

include $(TOP)$(ROOT)config/rules.mk
