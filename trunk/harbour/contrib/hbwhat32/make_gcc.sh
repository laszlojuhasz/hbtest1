#!/bin/sh

#
# $Id$
#
export HARBOURFLAGS="-I../xhb"
../mtpl_gcc.sh $1 $2 $3 $4 $5 $6 $7 $8 $9
unset HARBOURFLAGS
