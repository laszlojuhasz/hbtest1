#
# $Id$
#

ROOT = ../../

C_SOURCES =\
	addascii.c \
	asciisum.c \
	ascpos.c \
	atadjust.c \
	atnum.c \
	atrepl.c \
	bitnum.c \
	charevod.c \
	charlist.c \
	charmirr.c \
	charmix.c \
	charone.c \
	charonly.c \
	charop.c \
	charrepl.c \
	charsort.c \
	charsprd.c \
	charswap.c \
	color.c \
	count.c \
	ctc.c \
	ctchksum.c \
	ctcrypt.c \
	ctmath.c \
	ctmath2.c \
	ctnet.c \
	ctpad.c \
	ctset.c \
	ctstr.c \
	ctstrfil.c \
	ctwfunc.c \
	ctwin.c \
	cursor.c \
	datetime.c \
	dattime2.c \
	dattime3.c \
	dbftools.c \
	disk.c \
	expand.c \
	exponent.c \
	files.c \
	finan.c \
	ftoc.c \
	justify.c \
	keyset.c \
	like.c \
	lton.c \
	maxline.c \
	misc1.c \
	misc2.c \
	misc3.c \
	num1.c \
	numat.c \
	numcount.c \
	numline.c \
	pack.c \
	pos1.c \
	pos2.c \
	posdiff.c \
	print.c \
	range.c \
	relation.c \
	remove.c \
	replace.c \
	screen1.c \
	screen2.c \
	setlast.c \
	setrc.c \
	strdiff.c \
	strswap.c \
	tab.c \
	token1.c \
	token2.c \
	trig.c \
	video.c \
	wordrepl.c \
	wordtoch.c \

PRG_SOURCES=\
	blank.prg \
	ct.prg \
	ctmisc.prg \
	ctrand.prg \
	cttime.prg \
	fcopy.prg \
	getinfo.prg \
	getinput.prg \
	getsecrt.prg \
	keysave.prg \
	keysec.prg \
	keytime.prg \
	numconv.prg \
	screen3.prg \
	scrmark.prg \
	showtime.prg \

PRG_HEADERS=\
	ct.ch \
	ctdisk.ch \
	cterror.ch \
	ctextern.ch \
	ctvideo.ch \

LIBNAME=hbct

include $(TOP)$(ROOT)config/header.cf
INSTALL_RULE_HEADERS := $(INSTALL_RULE)
include $(TOP)$(ROOT)config/lib.cf

install::
	$(INSTALL_RULE_HEADERS)
