Summary: A unit test framework for C
Name: check
Version: 0.5.2
Release: 1
Epoch: 1
Source: http://prdownloads.sourceforge.net/check/check-0.5.2.tar.gz
Group: Development/Tools
Copyright: GPL
URL: http://check.sourceforge.net
Prefix: %{_prefix}
BuildPrereq: lyx sgml-tools
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
Check is a unit test framework for C. It features a simple interface for defining unit tests, putting little in the way of the developer. Tests are run in a separate address space, so Check can catch both assertion failures and code errors that cause segmentation faults or other signals. The output from unit tests can be used within source code editors and IDEs.

%prep
%setup

%build

./configure --prefix %{_prefix} 

make

%install
rm -rf ${RPM_BUILD_ROOT}
mkdir -p ${RPM_BUILD_ROOT}%{_prefix}
make prefix=$RPM_BUILD_ROOT%{_prefix} install

%clean
rm -rf ${RPM_BUILD_ROOT}

%files
%defattr(-,root,root)
%{_prefix}/include/check.h
%{_prefix}/lib/libcheck.a
%doc
%dir %{_prefix}/share/doc/%{name}-%{version}
%{_prefix}/share/doc/%{name}-%{version}/ChangeLog
%{_prefix}/share/doc/%{name}-%{version}/ChangeLogOld
%{_prefix}/share/doc/%{name}-%{version}/NEWS
%{_prefix}/share/doc/%{name}-%{version}/README
%{_prefix}/share/doc/%{name}-%{version}/COPYING
%{_prefix}/share/doc/%{name}-%{version}/example-1.html
%{_prefix}/share/doc/%{name}-%{version}/example-2.html
%{_prefix}/share/doc/%{name}-%{version}/example-3.html
%{_prefix}/share/doc/%{name}-%{version}/example-4.html
%{_prefix}/share/doc/%{name}-%{version}/example-5.html
%{_prefix}/share/doc/%{name}-%{version}/example.html
%{_prefix}/share/doc/%{name}-%{version}/example.lyx
%{_prefix}/share/doc/%{name}-%{version}/example.sgml 
%dir %{_prefix}/share/doc/%{name}-%{version}/money
%{_prefix}/share/doc/%{name}-%{version}/money/money.h
%{_prefix}/share/doc/%{name}-%{version}/money/money.c
%{_prefix}/share/doc/%{name}-%{version}/money/check_money.c
%{_prefix}/share/doc/%{name}-%{version}/money/aclocal.m4
%{_prefix}/share/doc/%{name}-%{version}/money/configure.in
%{_prefix}/share/doc/%{name}-%{version}/money/Makefile.am
%{_prefix}/share/doc/%{name}-%{version}/money/Makefile.in
%{_prefix}/share/doc/%{name}-%{version}/money/configure
%{_prefix}/share/doc/%{name}-%{version}/money/README
%{_prefix}/share/doc/%{name}-%{version}/money/AUTHORS
%{_prefix}/share/doc/%{name}-%{version}/money/COPYING
%{_prefix}/share/doc/%{name}-%{version}/money/INSTALL
%{_prefix}/share/doc/%{name}-%{version}/money/ChangeLog
%{_prefix}/share/doc/%{name}-%{version}/money/NEWS
%{_prefix}/share/doc/%{name}-%{version}/money/stamp-h.in
%{_prefix}/share/doc/%{name}-%{version}/money/config.h.in

%changelog
* Sat Jun 2 2001 Arien Malec <arien_malec@yahoo.com>
- First packaging.
