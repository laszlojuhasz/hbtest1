#!/bin/sh

#
# $Id$
#

export CFLAGS=-I./include
../mtpl_gcc.sh $1 $2 $3 $4 $5 $6 $7 $8 $9
unset CFLAGS