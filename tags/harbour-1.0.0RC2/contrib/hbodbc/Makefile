#
# $Id$
#

ROOT = ../../

LIBNAME=hbodbc

ifeq ($(HB_COMPILER),rsxnt)
HB_WITHOUT_ODBC=yes
endif

ifneq ($(HB_ARCHITECTURE),w32)
ifeq ($(HB_WITHOUT_ODBC),)

ifeq ($(HB_INC_ODBC),)
HB_INC_ODBC = /usr/include /usr/local/include
endif

HB_INC_ODBC_OK += $(foreach d, $(HB_INC_ODBC), $(if $(wildcard $(d)/sql.h),$(d),))

ifneq ($(strip $(HB_INC_ODBC_OK)),)
C_USR += $(foreach d, $(HB_INC_ODBC_OK), -I$(d))
else
HB_WITHOUT_ODBC=yes
endif

endif
endif

ifneq ($(HB_WITHOUT_ODBC),yes)

C_SOURCES= \
	odbc.c \

PRG_SOURCES= \
	todbc.prg \
	browodbc.prg \

PRG_HEADERS=\
	sql.ch \

include $(TOP)$(ROOT)config/header.cf
INSTALL_RULE_HEADERS := $(INSTALL_RULE)
include $(TOP)$(ROOT)config/lib.cf

install::
	$(INSTALL_RULE_HEADERS)

else
include $(TOP)$(ROOT)config/none.cf
endif
