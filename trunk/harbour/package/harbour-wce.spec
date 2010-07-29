#
# $Id$
#

# ---------------------------------------------------------------
# Copyright 2007 Przemyslaw Czerpak (druzus/at/priv.onet.pl),
# Harbour-WinCE cross build RPM spec file
#
# See COPYING for licensing terms.
# ---------------------------------------------------------------

######################################################################
## Definitions.
######################################################################

%define name      harbour-wce
%define version   2.1.0
%define releasen  beta2
%define hb_pref   hbce

# Workaround for the problem of /usr/bin/strip not handling PE binaries.
%define hb_ccpath /opt/mingw32ce/bin
%define hb_ccpref arm-wince-mingw32ce-
%define __strip   %{hb_ccpath}/%{hb_ccpref}strip
%define __objdump %{hb_ccpath}/%{hb_ccpref}objdump

######################################################################
## Preamble.
######################################################################

Summary:        Free software Clipper compatible compiler
Summary(pl):    Darmowy kompilator kompatybilny z j�zykiem Clipper.
Name:           %{name}
Version:        %{version}
Release:        %{releasen}
License:        GPL (plus exception)
Group:          Development/Languages
URL:            http://harbour-project.org/
Source:         harbour-%{version}.src.tar.gz
BuildPrereq:    gcc binutils bash
Requires:       gcc binutils bash sh-utils cegcc-mingw32ce harbour = %{?epoch:%{epoch}:}%{version}-%{release}
Provides:       %{name}
BuildRoot:      /tmp/%{name}-%{version}-root

%define         _noautoreq    'libharbour.*'

%description
Harbour is a CA-Cl*pper compatible compiler for multiple platforms. This
package includes a compiler, pre-processor, header files, virtual machine
and libraries for creating WinCE application in Linux box using MinGW-CE
GCC port.

%description -l pl
Harbour to kompatybilny z j�zykiem CA-Cl*pper kompilator rozwijany na
wielu r�nych platformach. Ten pakiet zawiera kompilator, preprocesor,
zbiory nag��wkowe, wirtualn+ maszyn� oraz biblioteki pozwalaj+ce na
tworzenie aplikacji dla WinCE-PocketPC przy u�yciu MinGW-CE GCC.


######################################################################
## Preperation.
######################################################################

%prep
%setup -c harbour
rm -fR $RPM_BUILD_ROOT

######################################################################
## Build.
######################################################################

%build

export HB_BUILD_CONTRIBS=no
export HB_PLATFORM=linux
export HB_COMPILER=gcc

make %{?_smp_mflags}

unset HB_COMPILER
unset HB_BUILD_CONTRIBS

export HB_BUILD_PARTS=lib
export HB_PLATFORM=wce
export HB_BIN_COMPILE="$(pwd)/bin/linux/gcc"

make %{?_smp_mflags}

######################################################################
## Install.
######################################################################

%install

# Install harbour itself.

unset HB_COMPILER

export HB_BUILD_PARTS=lib
export HB_PLATFORM=wce
export HB_BIN_COMPILE="$(pwd)/bin/linux/gcc"

export HB_INSTALL_BIN=$RPM_BUILD_ROOT/%{_bindir}
export HB_INSTALL_INC=$RPM_BUILD_ROOT/%{_includedir}/%{name}
export HB_INSTALL_LIB=$RPM_BUILD_ROOT/%{_libdir}/%{name}
export HB_INSTALL_DYN=${HB_INSTALL_LIB}
export HB_BUILD_STRIP=lib

mkdir -p $HB_INSTALL_BIN

make install %{?_smp_mflags}

# remove unused files
rm -fR ${HB_INSTALL_BIN}/{harbour,hbpp,hbmk2,hbrun,hbi18n,hbtest}.exe

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
rm -fR $RPM_BUILD_ROOT

######################################################################
## File list.
######################################################################

%files
%defattr(-,root,root,755)

%{_bindir}/hbmk2

%defattr(644,root,root,755)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/*.a

%defattr(755,root,root,755)
%{_libdir}/%{name}/*.dll

######################################################################
## Spec file Changelog.
######################################################################

%changelog
* Thu Oct 23 2007 Przemyslaw Czerpak (druzus/at/priv.onet.pl)
- initial release
