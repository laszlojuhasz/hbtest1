#
# $Id$
#

# ---------------------------------------------------------------
# Copyright 2003 Przemyslaw Czerpak <druzus@polbox.com>,
# Dave Pearson <davep@davep.org>
# Harbour RPM spec file
#
# See COPYING for licensing terms.
# ---------------------------------------------------------------

######################################################################
## Definitions.
######################################################################

# please add your distro suffix if it not belong to the one recognized below
# and remember that order checking can be important

%define platform %(release=$(rpm -q --queryformat='%{VERSION}' mandriva-release-common 2>/dev/null) && echo "mdv$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %(release=$(rpm -q --queryformat='%{VERSION}' mandrake-release 2>/dev/null) && echo "mdk$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %(release=$(rpm -q --queryformat='%{VERSION}' redhat-release 2>/dev/null) && echo "rh$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %(release=$(rpm -q --queryformat='%{VERSION}' fedora-release 2>/dev/null) && echo "fc$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %(release=$(rpm -q --queryformat='%{VERSION}' suse-release 2>/dev/null) && echo "sus$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %(release=$(rpm -q --queryformat='%{VERSION}' openSUSE-release 2>/dev/null) && echo "sus$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %(release=$(rpm -q --queryformat='%{VERSION}' conectiva-release 2>/dev/null) && echo "cl$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %(release=$(rpm -q --queryformat='%{VERSION}' aurox-release 2>/dev/null) && echo "aur$release"|tr -d ".")
%if "%{platform}" == ""
%define platform %([ -f /etc/pld-release ] && cat /etc/pld-release|sed -e '/1/ !d' -e 's/[^0-9]//g' -e 's/^/pld/')
%endif
%endif
%endif
%endif
%endif
%endif
%endif
%endif


%define name     harbour
%define dname    Harbour
%define version  2.0.0
%define releasen 0
%define hb_pref  hb
%define hb_etcdir /etc/%{name}
%define hb_plat  export HB_PLATFORM=linux
%define hb_cc    export HB_COMPILER=gcc
%define hb_cflag export HB_USER_CFLAGS=
%define hb_lflag export HB_USER_LDFLAGS="${CC_HB_USER_LDFLAGS} %{?_with_static:-static}"
%define hb_gpm   export HB_WITH_GPM=%{!?_without_gpm:yes}%{?_without_gpm:no}
%define hb_crs   export HB_WITH_CURSES=%{!?_without_curses:yes}%{?_without_curses:no}
%define hb_sln   export HB_WITH_SLANG=%{!?_without_slang:yes}%{?_without_slang:no}
%define hb_x11   export HB_WITH_X11=%{!?_without_x11:yes}%{?_without_x11:no}
%define hb_local export HB_WITH_ZLIB=%{?_with_localzlib:local} ; export HB_WITH_PCRE=%{?_with_localpcre:local}
%define hb_bdir  export HB_BIN_INSTALL=%{_bindir}
%define hb_idir  export HB_INC_INSTALL=%{_includedir}/%{name}
%define hb_ldir  export HB_LIB_INSTALL=%{_libdir}/%{name}
%define hb_edir  export HB_ETC_INSTALL=%{hb_etcdir}
%define hb_cmrc  export HB_BUILD_NOGPLLIB=%{?_without_gpllib:yes}
%define hb_ctrb  export HB_CONTRIBLIBS="hbbmcdx hbbtree hbclipsm hbct hbgt hbmisc hbmzip hbnetio hbtip hbtpathy hbhpdf hbsms hbziparc xhb rddsql hbnf %{?_with_odbc:hbodbc} %{?_with_curl:hbcurl} %{?_with_ads:rddads} %{?_with_gd:hbgd} %{?_with_pgsql:hbpgsql} %{?_with_mysql:hbmysql} %{?_with_firebird:hbfbird} %{?_with_allegro:gtalleg} %{?_with_qt:hbqt hbxbp}"
%define hb_env   %{hb_plat} ; %{hb_cc} ; %{hb_cflag} ; %{hb_lflag} ; %{hb_gpm} ; %{hb_crs} ; %{hb_sln} ; %{hb_x11} ; %{hb_local} ; %{hb_bdir} ; %{hb_idir} ; %{hb_ldir} ; %{hb_edir} ; %{hb_ctrb} ; %{hb_cmrc}
%define hb_host  www.harbour-project.org
%define readme   README.RPM
######################################################################
## Preamble.
######################################################################
Summary:        Free software Clipper compatible compiler
Summary(pl):    Darmowy kompilator kompatybilny z j�zykiem Clipper.
Summary(pt_BR): Um compilador Clipper compativel Gratis
Summary(ru):    ��������� ����������, ����������� � ������ Clipper.
Summary(hu):    Szabad szoftver Clipper kompatibilis ford�t�
Name:           %{name}
Version:        %{version}
Release:        %{releasen}%{platform}
License:        GPL (plus exception)
Group:          Development/Languages
Vendor:         %{hb_host}
URL:            http://%{hb_host}/
Source:         %{name}-%{version}.src.tar.gz
Packager:       Przemys�aw Czerpak <druzus@polbox.com> Luiz Rafael Culik Guimaraes <culikr@uol.com.br>
BuildPrereq:    gcc binutils bash %{!?_without_curses: ncurses-devel} %{!?_without_gpm: gpm-devel}
Requires:       gcc binutils bash sh-utils %{name}-lib = %{?epoch:%{epoch}:}%{version}-%{release}
Provides:       %{name} harbour
BuildRoot:      /tmp/%{name}-%{version}-root

%define         _noautoreq    'libharbour.*'

%description
%{dname} is a CA-Cl*pper compatible compiler for multiple platforms. This
package includes a compiler, pre-processor, header files, virtual machine
and documentation.

See README.RPM in the documentation directory for information specific to
this RPM distribution.

%description -l pl
%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator rozwijany na
wielu r�nych platformach. Ten pakiet zawiera kompilator, preprocesor,
zbiory nag��wkowe, wirtualn+ maszyn� oraz dokumentacj�.

%description -l pt_BR
%{dname} � um compilador Clipper compativel para multiplas plataformas.
Esse pacote contem um compilador, um pr�-processador, arquivos de cabe�alho
uma maquina virtual e documenta��o.

%description -l ru
%{dname} - ������������������ ����������, ����������� � ������ CA-Cl*pper.
���� ����� �������� ����������, ������������, ����� ����������, �����������
������ � ������������.

%description -l hu
%{dname} egy t�bb platformon is m�k�d� CA-Cl*pper kompatibilis
ford�t�program. A csomag r�sze a ford�t� maga, az el�ford�t�, fejl�c
�llom�nyok, a virtu�lis g�p �s f�ggv�nyk�nyvt�rak, valamint a dokument�ci�.

######################################################################
## main shared lib
######################################################################

%package lib
Summary:        Shared runtime libaries for %{dname} compiler
Summary(pl):    Dzielone bilioteki dla kompilatora %{dname}
Summary(ru):    ��������� ������������ ���������� ��� ����������� %{dname}
Summary(hu):    Megosztott k�nyvt�rak a(z) %{dname} ford�t�hoz
Group:          Development/Languages
Provides:       lib%{name}.so lib%{name}mt.so

%description lib
%{dname} is a Clipper compatible compiler.
This package provides %{dname} runtime shared libraries for programs
linked dynamically.

%description -l pl lib
%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.
Ten pakiet udost�pnia dzielone bilioteki kompilatora %{dname}
dla program�w konsolidowanych dynamicznie.

%description -l pt_BR lib
%{dname} � um compilador compativel com o Clipper.
Esse pacote %{dname} provem as bibliotecas compartilhadas para programas
linkados dinamicamente.

%description -l ru lib
%{dname} - ����������, ����������� � ������ CA-Cl*pper.
���� ����� �������� ��������� ������������ ���������� %{dname},
����������� ��� ������ ����������� �������������� ��������.

%description -l hu lib
A(z) %{dname} egy Clipper kompatibilis ford�t�program.
Ez a csomag biztos�tja a dinamikusan szerkesztett %{dname}
programokhoz sz�ks�ges megosztott (dinamikus) futtat�k�nyvt�rakat.

######################################################################
## static libs
######################################################################

%package static
Summary:        Static runtime libaries for %{dname} compiler
Summary(pl):    Statyczne bilioteki dla kompilatora %{dname}
Summary(ru):    ����������� ���������� ��� ����������� %{dname}
Summary(hu):    Statikus k�nyvt�rak a(z) %{dname} ford�t�hoz
Group:          Development/Languages
Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}

%description static
%{dname} is a Clipper compatible compiler.
This package provides %{dname} static runtime libraries for static
program linking.

%description -l pl static
%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.
Ten pakiet udost�pnia statyczne bilioteki dla kompilatora %{dname}
niezb�dne do statycznej konsolidacji program�w.

%description -l pt_BR static
%{dname} � um compilador compativel com o clippe.
Esse pacote %{dname} provem as bibliotecas  de run time staticas para linkagem
dos os programas

%description -l ru static
%{dname} - ����������, ����������� � ������ CA-Cl*pper.
���� ����� �������� ����������� ���������� ����������� %{dname},
����������� ��� ����������� ���������� ��������.

%description -l hu lib
A(z) %{dname} egy Clipper kompatibilis ford�t�program.
Ez a csomag biztos�tja a statikusan szerkesztett %{dname}
programokhoz sz�ks�ges statikus futtat�k�nyvt�rakat.


%package contrib
Summary:        Contrib runtime libaries for %{dname} compiler
Summary(pl):    Bilioteki z drzewa contrib dla kompilatora %{dname}
Summary(pt_BR): Libs contrib para %{dname}
Summary(ru):    ���������� �� ������ contrib ��� ����������� %{dname}
Summary(hu):    Kieg�sz�t� k�nyvt�rak a(z) %{dname} ford�t�hoz
Group:          Development/Languages
Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}

%description contrib
%{dname} is a Clipper compatible compiler.
This package provides %{dname} contrib libraries for program linking.

%description -l pl contrib
%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.
Ten pakiet udost�pnia statyczne bilioteki z drzewa contrib dla
kompilatora %{dname}.

%description -l pt_BR contrib
%{dname} � um compilador compativel com o clippe.
Esse pacote %{dname} provem as bibliotecas contrib para linkagem
dos programas.

%description -l ru contrib
%{dname} - ����������, ����������� � ������ CA-Cl*pper.
���� ����� �������� ����������� ���������� %{dname} �� ������ contrib.

%description -l hu lib
A(z) %{dname} egy Clipper kompatibilis ford�t�program.
Ez a csomag kieg�sz�t� (contrib) k�nyvt�rakat biztos�t
statikus szerkeszt�shez.

## odbc library
%{?_with_odbc:%package odbc}
%{?_with_odbc:Summary:        ODBC libarary for %{dname} compiler}
%{?_with_odbc:Summary(pl):    Bilioteka ODBC dla kompilatora %{dname}}
%{?_with_odbc:Group:          Development/Languages}
%{?_with_odbc:Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_odbc:%description odbc}
%{?_with_odbc:%{dname} is a Clipper compatible compiler.}
%{?_with_odbc:This package provides %{dname} ODBC library for program linking.}

%{?_with_odbc:%description -l pl odbc}
%{?_with_odbc:%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.}
%{?_with_odbc:Ten pakiet udost�pnia statyczn+ biliotek� ODBC dla kompilatora %{dname}.}

## CURL library
%{?_with_curl:%package curl}
%{?_with_curl:Summary:        CURL libarary for %{dname} compiler}
%{?_with_curl:Summary(pl):    Bilioteka CURL dla kompilatora %{dname}}
%{?_with_curl:Group:          Development/Languages}
%{?_with_curl:Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_curl:%description curl}
%{?_with_curl:%{dname} is a Clipper compatible compiler.}
%{?_with_curl:This package provides %{dname} CURL library for program linking.}

%{?_with_curl:%description -l pl curl}
%{?_with_curl:%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.}
%{?_with_curl:Ten pakiet udost�pnia statyczn+ biliotek� CURL dla kompilatora %{dname}.}

## ADS RDD
%{?_with_ads:%package ads}
%{?_with_ads:Summary:        ADS RDDs for %{dname} compiler}
%{?_with_ads:Summary(pl):    Bilioteka sterownik�w (RDDs) ADS dla kompilatora %{dname}}
%{?_with_ads:Group:          Development/Languages}
%{?_with_ads:Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_ads:%description ads}
%{?_with_ads:%{dname} is a Clipper compatible compiler.}
%{?_with_ads:This package provides %{dname} ADS RDDs for program linking.}

%{?_with_ads:%description -l pl ads}
%{?_with_ads:%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.}
%{?_with_ads:Ten pakiet udost�pnia sterowniki (RDD) ADS dla kompilatora %{dname}.}

## mysql library
%{?_with_mysql:%package mysql}
%{?_with_mysql:Summary:        MYSQL libarary for %{dname} compiler}
%{?_with_mysql:Summary(pl):    Bilioteka MYSQL dla kompilatora %{dname}}
%{?_with_mysql:Group:          Development/Languages}
%{?_with_mysql:Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_mysql:%description mysql}
%{?_with_mysql:%{dname} is a Clipper compatible compiler.}
%{?_with_mysql:This package provides %{dname} MYSQL library for program linking.}

%{?_with_mysql:%description -l pl mysql}
%{?_with_mysql:%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.}
%{?_with_mysql:Ten pakiet udost�pnia statyczn+ biliotek� MYSQL dla kompilatora %{dname}.}

## pgsql library
%{?_with_pgsql:%package pgsql}
%{?_with_pgsql:Summary:        PGSQL libarary for %{dname} compiler}
%{?_with_pgsql:Summary(pl):    Bilioteka PGSQL dla kompilatora %{dname}}
%{?_with_pgsql:Group:          Development/Languages}
%{?_with_pgsql:Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_pgsql:%description pgsql}
%{?_with_pgsql:%{dname} is a Clipper compatible compiler.}
%{?_with_pgsql:This package provides %{dname} PGSQL library for program linking.}

%{?_with_pgsql:%description -l pl pgsql}
%{?_with_pgsql:%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.}
%{?_with_pgsql:Ten pakiet udost�pnia statyczn+ biliotek� PGSQL dla kompilatora %{dname}.}

## firebird library
%{?_with_firebird:%package firebird}
%{?_with_firebird:Summary:        FireBird libarary for %{dname} compiler}
%{?_with_firebird:Summary(pl):    Bilioteka FireBird dla kompilatora %{dname}}
%{?_with_firebird:Group:          Development/Languages}
%{?_with_firebird:Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_firebird:%description firebird}
%{?_with_firebird:%{dname} is a Clipper compatible compiler.}
%{?_with_firebird:This package provides %{dname} FireBird library for program linking.}

%{?_with_firebird:%description -l pl firebird}
%{?_with_firebird:%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.}
%{?_with_firebird:Ten pakiet udost�pnia statyczn+ biliotek� FireBird dla kompilatora %{dname}.}

## gd library
%{?_with_gd:%package gd}
%{?_with_gd:Summary:        GD libarary for %{dname} compiler}
%{?_with_gd:Summary(pl):    Bilioteka GD dla kompilatora %{dname}}
%{?_with_gd:Group:          Development/Languages}
%{?_with_gd:Requires:       %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_gd:%description gd}
%{?_with_gd:%{dname} is a Clipper compatible compiler.}
%{?_with_gd:This package provides %{dname} GD library for program linking.}

%{?_with_gd:%description -l pl gd}
%{?_with_gd:%{dname} to kompatybilny z j�zykiem CA-Cl*pper kompilator.}
%{?_with_gd:Ten pakiet udost�pnia statyczn+ biliotek� GD dla kompilatora %{dname}.}

## qt library
%{?_with_qt:%package qt}
%{?_with_qt:Summary:        QT library bindings for %{dname} compiler}
%{?_with_qt:Group:          Development/Languages}
%{?_with_qt:Requires:       libqt4-devel %{name} = %{?epoch:%{epoch}:}%{version}-%{release}}

%{?_with_qt:%description qt}
%{?_with_qt:%{dname} is a Clipper compatible compiler.}
%{?_with_qt:This package provides %{dname} QT libraries for program linking.}


######################################################################
## Preperation.
######################################################################

%prep
%setup -c %{name}
rm -rf $RPM_BUILD_ROOT

######################################################################
## Build.
######################################################################

%build
%{hb_env}
export HB_BUILD_STRIP=all
export HB_BUILD_SHARED=%{!?_with_static:yes}

make %{?_smp_mflags}

######################################################################
## Install.
######################################################################

%install

# Install harbour itself.

%{hb_env}

export HB_INST_PKGPREF=$RPM_BUILD_ROOT
export HB_BUILD_STRIP=all
export HB_BUILD_SHARED=%{!?_with_static:yes}

# necessary for shared linked hbrun used to execute postinst.prg
export LD_LIBRARY_PATH=$HB_INST_PKGPREF$HB_LIB_INSTALL

make install %{?_smp_mflags}

[ "%{?_with_allegro:1}" ]  || rm -f $HB_INST_PKGPREF$HB_LIB_INSTALL/libgtalleg.a
[ "%{?_without_curses:1}" ] && rm -f $HB_INST_PKGPREF$HB_LIB_INSTALL/libgtcrs.a
[ "%{?_without_slang:1}" ] && rm -f $HB_INST_PKGPREF$HB_LIB_INSTALL/libgtsln.a
rm -f $HB_INST_PKGPREF$HB_LIB_INSTALL/liblibhpdf.a
rm -f $HB_INST_PKGPREF$HB_LIB_INSTALL/liblibpng.a
rm -f $HB_INST_PKGPREF$HB_LIB_INSTALL/libsqlite3.a

mkdir -p $HB_INST_PKGPREF%{_mandir}/man1
install -m644 doc/man/*.1* $HB_INST_PKGPREF%{_mandir}/man1/

mkdir -p $HB_INST_PKGPREF$HB_ETC_INSTALL
install -m644 src/rtl/gtcrs/hb-charmap.def $HB_INST_PKGPREF$HB_ETC_INSTALL/hb-charmap.def

# remove unused files
rm -f $HB_INST_PKGPREF$HB_BIN_INSTALL/hbtest

# Create a README file for people using this RPM.
cat > doc/%{readme} <<EOF
This RPM distribution of %{dname} includes extra commands to make compiling
and linking with %{dname} a little easier. There are compiler and linker
wrappers called "%{hb_pref}cc", "%{hb_pref}cmp", "%{hb_pref}lnk" and "%{hb_pref}mk".

"%{hb_pref}cc" is a wrapper to the C compiler only. It sets all flags
and paths necessary to compile .c files which include %{dname} header
files. The result of its work is an object file.

Use "%{hb_pref}cmp" exactly as you would use the harbour compiler itself.
The main difference with %{hb_pref}cmp is that it results in an object file,
not a C file that needs compiling down to an object. %{hb_pref}cmp also
ensures that the harbour include directory is seen by the harbour compiler.

"%{hb_pref}lnk" simply takes a list of object files and links them together
with the harbour virtual machine and run-time library to produce an
executable. The executable will be given the basename of the first object
file if not directly set by the "-o" command line switch.

"%{hb_pref}mk" tries to produce an executable from your .prg file. It's a simple
equivalent of cl.bat from the CA-Cl*pper distribution.

All these scripts accept command line switches:
-o<outputfilename>      # output file name
-static                 # link with static %{dname} libs
-fullstatic             # link with all static libs
-shared                 # link with shared libs (default)
-mt                     # link with multi-thread libs
-gt<hbgt>               # link with <hbgt> GT driver, can be repeated to
                        # link with more GTs. The first one will be
                        #      the default at runtime
-xbgtk                  # link with xbgtk library (xBase GTK+ interface)
-hwgui                  # link with HWGUI library (GTK+ interface)
-l<libname>             # link with <libname> library
-L<libpath>             # additional path to search for libraries
-[no]strip              # strip (no strip) binaries
-main=<main_func>       # set the name of main program function/procedure.
                        # if not set then 'MAIN' is used or if it doesn't
                        # exist the name of first public function/procedure
                        # in first linked object module (link)

Link options work only with "%{hb_pref}lnk" and "%{hb_pref}mk" and have no effect
in "%{hb_pref}cc" and "%{hb_pref}cmp".
Other options are passed to %{dname}/C compiler/linker.

An example compile/link session looks like:
----------------------------------------------------------------------
druzus@uran:~/tmp$ cat foo.prg
function main()
? "Hello, World!"
return nil

druzus@uran:~/tmp$ %{hb_pref}cmp foo
Harbour 1.0.0 Intl. (Rev. 9099)
Copyright (c) 1999-2008, http://www.harbour-project.org/
Compiling 'foo.prg'...
Lines 5, Functions/Procedures 2
Generating C source output to 'foo.c'... Done.

druzus@uran:~/tmp$ %{hb_pref}lnk foo.o

druzus@uran:~/tmp$ strip foo

druzus@uran:~/tmp$ ls -l foo
-rwxrwxr-x    1 druzus   druzus       3824 maj 17 02:46 foo
----------------------------------------------------------------------

or using %{hb_pref}mk only:
----------------------------------------------------------------------
druzus@uran:~/tmp$ cat foo.prg
function main()
? "Hello, World!"
return nil

druzus@uran:~/tmp$ %{hb_pref}mk foo
Harbour 1.0.0 Intl. (Rev. 9099)
Copyright (c) 1999-2008, http://www.harbour-project.org/
Compiling 'foo.prg'...
Lines 5, Functions/Procedures 2
Generating C source output to 'foo.c'... Done.

druzus@uran:~/tmp$ ls -l foo
-rwxrwxr-x    1 druzus   druzus       3824 maj 17 02:46 foo
----------------------------------------------------------------------


In this RPM you will find additional wonderful tools: /usr/bin/hbrun
You can run clipper/xbase compatible source files with it if you only
put in their first line:
#!/usr/bin/hbrun

For example:
----------------------------------------------------------------------
druzus@uran:~/tmp$ cat foo.prg
#!/usr/bin/hbrun
function main()
? "Hello, World!, This is a script !!! :-)"
?
return nil

druzus@uran:~/tmp$ chmod +x foo.prg

druzus@uran:~/tmp$ ./foo.prg

Hello, World!, This is a script !!! :-)

I hope this RPM is useful. Have fun with %{dname}.

Many thanks to Dave Pearson <davep@davep.org>

Przemyslaw Czerpak <druzus@polbox.com>
EOF

######################################################################
## Post install
######################################################################
#%post lib
#/sbin/ldconfig

######################################################################
## Post uninstall
######################################################################
#%postun lib
#/sbin/ldconfig

######################################################################
## Clean.
######################################################################

%clean
rm -rf $RPM_BUILD_ROOT

######################################################################
## File list.
######################################################################

%files
%defattr(-,root,root,755)
%doc ChangeLog*
%doc doc/*.txt
%doc doc/%{readme}
%doc doc/en-EN/

%dir %{hb_etcdir}
%verify(not md5 mtime) %config %{hb_etcdir}/hb-charmap.def
%{_bindir}/harbour
%{_bindir}/hbpp
%{_bindir}/hb-mkdyn
%{_bindir}/hb-mkslib
%{_bindir}/%{hb_pref}-build
%{_bindir}/%{hb_pref}cc
%{_bindir}/%{hb_pref}cmp
%{_bindir}/%{hb_pref}lnk
%{_bindir}/%{hb_pref}mk
#%{_bindir}/hbtest
%{_bindir}/hbrun
%{_bindir}/hbi18n
%{_bindir}/hbformat
%{_bindir}/hbmk2
%verify(not md5 mtime) %config %{_bindir}/hbmk.cfg
%{_mandir}/man1/*.1*
%dir %{_includedir}/%{name}
%attr(644,root,root) %{_includedir}/%{name}/*

%files static
%defattr(644,root,root,755)
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/libhbcpage.a
%{_libdir}/%{name}/libhbcommon.a
%{_libdir}/%{name}/libhbcplr.a
%{_libdir}/%{name}/libhbdebug.a
%{_libdir}/%{name}/librddfpt.a
%{_libdir}/%{name}/librddcdx.a
%{_libdir}/%{name}/librddntx.a
%{_libdir}/%{name}/librddnsx.a
%{_libdir}/%{name}/libgt*.a
%{_libdir}/%{name}/libhblang.a
%{_libdir}/%{name}/libhbmacro.a
%{_libdir}/%{name}/libhbextern.a
%{_libdir}/%{name}/libhbnulrdd.a
%{_libdir}/%{name}/libhbnortl.a
%{_libdir}/%{name}/libhbpp.a
%{_libdir}/%{name}/libhbrdd.a
%{_libdir}/%{name}/libhbhsx.a
%{_libdir}/%{name}/libhbsix.a
%{_libdir}/%{name}/libhbrtl.a
%{_libdir}/%{name}/libhbvm.a
%{_libdir}/%{name}/libhbvmmt.a
%{_libdir}/%{name}/libhbusrrdd.a
%{_libdir}/%{name}/libhbuddall.a
%{?_with_localzlib:%{_libdir}/%{name}/libhbzlib.a}
%{?_with_localpcre:%{_libdir}/%{name}/libhbpcre.a}

%files contrib
%defattr(644,root,root,755)
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/libhbnf.a
%{_libdir}/%{name}/libhbbtree.a
%{_libdir}/%{name}/libhbmisc.a
%{_libdir}/%{name}/libhbmzip.a
%{_libdir}/%{name}/libhbnetio.a
%{_libdir}/%{name}/libhbct.a
%{_libdir}/%{name}/libhbtip*.a
%{_libdir}/%{name}/libxhb.a
%{_libdir}/%{name}/libhbhpdf.a
%{_libdir}/%{name}/libhbgt.a
%{_libdir}/%{name}/libhbbmcdx.a
%{_libdir}/%{name}/libhbclipsm.a
%{_libdir}/%{name}/librddsql.a
%{_libdir}/%{name}/libhbsms.a
%{_libdir}/%{name}/libhbtpathy.a
%{_libdir}/%{name}/libhbziparc.a

%files lib
%defattr(755,root,root,755)
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/*.so
%{_libdir}/*.so

%{?_with_curl:%files curl}
%{?_with_curl:%defattr(644,root,root,755)}
%{?_with_curl:%dir %{_libdir}/%{name}}
%{?_with_curl:%{_libdir}/%{name}/libhbcurl.a}

%{?_with_ads:%files ads}
%{?_with_ads:%defattr(644,root,root,755)}
%{?_with_ads:%dir %{_libdir}/%{name}}
%{?_with_ads:%{_libdir}/%{name}/librddads.a}

%{?_with_odbc:%files odbc}
%{?_with_odbc:%defattr(644,root,root,755)}
%{?_with_odbc:%dir %{_libdir}/%{name}}
%{?_with_odbc:%{_libdir}/%{name}/libhbodbc.a}
%{?_with_odbc:%{_libdir}/%{name}/libsddodbc.a}

%{?_with_mysql:%files mysql}
%{?_with_mysql:%defattr(644,root,root,755)}
%{?_with_mysql:%dir %{_libdir}/%{name}}
%{?_with_mysql:%{_libdir}/%{name}/libhbmysql.a}
%{?_with_mysql:%{_libdir}/%{name}/libsddmy.a}

%{?_with_pgsql:%files pgsql}
%{?_with_pgsql:%defattr(644,root,root,755)}
%{?_with_pgsql:%dir %{_libdir}/%{name}}
%{?_with_pgsql:%{_libdir}/%{name}/libhbpgsql.a}
%{?_with_pgsql:%{_libdir}/%{name}/libsddpg.a}

%{?_with_firebird:%files firebird}
%{?_with_firebird:%defattr(644,root,root,755)}
%{?_with_firebird:%dir %{_libdir}/%{name}}
%{?_with_firebird:%{_libdir}/%{name}/libhbfbird.a}
%{?_with_firebird:%{_libdir}/%{name}/libsddfb.a}

%{?_with_gd:%files gd}
%{?_with_gd:%defattr(644,root,root,755)}
%{?_with_gd:%dir %{_libdir}/%{name}}
%{?_with_gd:%{_libdir}/%{name}/libhbgd.a}

%{?_with_qt:%files qt}
%{?_with_qt:%defattr(644,root,root,755)}
%{?_with_qt:%dir %{_libdir}/%{name}}
%{?_with_qt:%{_libdir}/%{name}/libhbqt.a}
%{?_with_qt:%{_libdir}/%{name}/libhbqtcore.a}
%{?_with_qt:%{_libdir}/%{name}/libhbqtgui.a}
%{?_with_qt:%{_libdir}/%{name}/libhbqtnetwork.a}
%{?_with_qt:%{_libdir}/%{name}/libhbxbp.a}

####################################################################
## Spec file Changelog.
######################################################################

%changelog
* Thu Aug 05 2008 Viktor Szakats (harbour.01 syenar hu)
- removed hbdot, hbverfix, hbpptest
- hbrun now fully replaces hbdot.

* Thu Aug 23 2007 Przemyslaw Czerpak <druzus@priv.onet.pl>
+ added hbdot
- removed PP package

* Fri Mar 23 2005 Przemyslaw Czerpak <druzus@priv.onet.pl>
- removed bison and flex from dependences list

* Sat Aug 09 2003 Przemyslaw Czerpak <druzus@polbox.com>
- removed ${RPM_OPT_FLAGS} from HB_USER_CFLAGS

* Wed Jul 23 2003 Przemyslaw Czerpak <druzus@polbox.com>
- fixed file (user and group) owner for RPMs builded from non root account
- shared lib names changed from [x]harbour{mt,}.so to
  [x]harbour{mt,}-<version>.so and soft links with short names created
- 0.82 version set

* Wed Apr 30 2003 Przemyslaw Czerpak <druzus@polbox.com>
- new tool "%{hb_pref}-build" (%{hb_pref}cmp, %{hb_pref}lnk, %{hb_pref}mk) added -
  compiler/linker wrapper.
- new tool "hb-mkslib" (build shared libraries from static ones and object
  files).
- shared libraries added.
- binary package divided.

* Fri Mar 08 2002 Dave Pearson <davep@davep.org>
- Fixed gharbour so that it should work no matter the case of the name of
  the PRG file.

* Wed Feb 13 2002 Dave Pearson <davep@davep.org>
- Fixed a bug in harbour-link which meant that, since the environment
  changes of Jan 17 2002, users could not specify which GT library they
  wanted their application linked against.

* Tue Jan 22 2002 Dave Pearson <davep@davep.org>
- Used the "name" macro a lot more, especially in some paths.

* Thu Jan 17 2002 Dave Pearson <davep@davep.org>
- Removed the use of the /etc/profile.d scripts for setting the
  harbour environment variables. The settings are now placed
  directly in gharbour and harbour-link. This means that this .spec
  file should be more useful on RPM using platforms other than RedHat.

* Wed Dec 19 2001 Dave Pearson <davep@davep.org>
- Added a platform ID to the name of the RPM.

* Mon Dec 17 2001 Dave Pearson <davep@davep.org>
- todo.txt is now called TODO.

* Tue Aug 21 2001 Dave Pearson <davep@davep.org>
- Added todo.txt as a doc file.

* Sun Jul 22 2001 Dave Pearson <davep@davep.org>
- harbour-link now fully respects the setting of $HB_GT_LIB.
- HB_GT_LIB wasn't set in the csh startup script. Fixed.

* Fri Jul 20 2001 Dave Pearson <davep@davep.org>
- Added the setting of $HB_GT_LIB to the environment (ncurses is used).
- Added support for installing hbmake.

* Mon Jun 28 2001 Dave Pearson <davep@davep.org>
- Changed the gharbour script so that it only invokes the C compiler if a C
  file was output. This stops any error messages when someone is using the
  -g option to output other target types.

* Mon Mar 19 2001 Dave Pearson <davep@davep.org>
- Reinstated hbrun in the files section.

* Tue Feb 20 2001 Dave Pearson <davep@davep.org>
- Added README.RPM to the documentation directory.

* Sat Jan 06 2001 Dave Pearson <davep@davep.org>
- The gharbour script now passes the harbour include directory, using -I,
  to harbour.

* Thu Aug 24 2000 Dave Pearson <davep@davep.org>
- Changed the files section so that hbrun doesn't get installed. It isn't
  useful on GNU/Linux systems.

* Tue Aug 22 2000 Dave Pearson <davep@davep.org>
- Changed the 'egcs' requirement to 'gcc'.

* Mon Aug 21 2000 Przemyslaw Czerpak <druzus@polbox.com>
- Polish translation added
- BuildRoot marco added. Now you can build the package from normal user
  account.
- bison and flex added to BuildPrereq list
- Debug information is stripped from installed files.

* Wed Aug 02 2000 Dave Pearson <davep@davep.org>
- Removed hbtest from the list of files installed into the bin directory.
- Added 'bash' and 'sh-utils' to the list of required packages.

* Tue Aug 01 2000 Dave Pearson <davep@davep.org>
- Added harbour environment scripts to /etc/profile.d.
- Added generation of gharbour and harbour-link commands.

* Mon Jul 31 2000 Dave Pearson <davep@davep.org>
- Re-worked the layout of the spec file to make it cleaner and easier to
  read and maintain.
- The latest harbour ChangeLog is now installed into the RPM's doc
  directory.
- The content of the RPM's doc directory reflects the layout and content of
  the harbour source's doc directory.
