#
# spec file for package phyxcalc
#
# Copyright (c) 2012 Strahlex
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

# norootforbuild

Name:		phyxcalc

# Fedora specifics
%if 0%{?fedora_version} > 0 || 0%{?rhel_version} > 0 || 0%{?centos_version} > 0
BuildRequires:  qt4-devel >= 4.7 gcc-c++  boost-devel
%endif

# Mandriva specifics
%if 0%{?mandriva_version} > 0
BuildRequires:  libqt4-devel >= 4.7 boost-devel
#BuildRequires:  XFree86-devel lcms-devel freetype-devel hal-devel  -kernel
%endif

# SUSE Specifics
%if 0%{?suse_version} > 0
BuildRequires:  update-desktop-files libqt4-devel >= 4.7 boost-devel
%endif

Version:	3rev149
Release:	0
URL:		http://sourceforge.net/projects/phyxcalc/
License:	GPLv3
Source0:	%{name}-%{version}.tar.gz
Group:		Productivity/Scientific/Math
Summary:	Physical Expression Calculator
BuildRoot:      %{_tmppath}/%{name}-%{version}-build    
#Requires:         electronics-menu

%description
PhyxCalc is an Physical Expression Calculator.

%prep
%setup -q
#%patch1 -p1

%build
%if 0%{?fedora_version} > 0 || 0%{?rhel_version} > 0
qmake-qt4
%else
qmake
%endif
%{__make} %{?jobs:-j %jobs}

%install
#%makeinstall INSTALL_ROOT=%buildroot
#rm -rf $RPM_BUILD_ROOT/usr/doc/qelectrotech \
#       $RPM_BUILD_ROOT%{_mandir}/fr.ISO8859-1 \
#       $RPM_BUILD_ROOT%{_mandir}/fr

#mv $RPM_BUILD_ROOT%{_mandir}/fr.UTF-8 $RPM_BUILD_ROOT%{_mandir}/fr

%{__install} -Dm 755 -s %{name} %{buildroot}%{_bindir}/%{name}
#%{__install} -Dm 644 %{name}.1 %{buildroot}%{_mandir}/man1/%{name}.1
%{__install} -Dm 644 misc/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop
%{__install} -Dm 644 images/%{name}.png %{buildroot}%{_datadir}/pixmaps/%{name}.png
%if 0%{?suse_version} > 0
%suse_update_desktop_file -r %{name} Graphics VectorGraphics
%endif

%clean
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc gpl.txt README doc/doc.html
#%{_bindir}/%{name}
%{_bindir}/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
#%dir %{_datadir}/mime/application
#%{_datadir}/mime/application/x-qet-*.xml
#%{_datadir}/mime/packages/%{name}.xml
#%dir %{_datadir}/mimelnk
#%dir %{_datadir}/mimelnk/application
#%{_datadir}/mimelnk/application/x-qet-*.desktop
#%exclude %{_datadir}/%{name}/examples
#%{_mandir}/man1/%{name}.*
#%lang(es) %{_mandir}/es/man1/%{name}.*

%changelog
* Sat Mar 03 2012 Strahlex <mail.aroessler@gmail.com>
- fixed some bugs
* Sat Mar 03 2012 Strahlex <mail.aroessler@gmail.com>
- added revision 146 to Build Service