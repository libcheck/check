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
%dir %{_prefix}/share/doc/%{name}-%{version}
%doc %{_prefix}/share/doc/%{name}-%{version}/ChangeLog
%doc %{_prefix}/share/doc/%{name}-%{version}/ChangeLogOld
%doc %{_prefix}/share/doc/%{name}-%{version}/NEWS
%doc %{_prefix}/share/doc/%{name}-%{version}/README
%doc %{_prefix}/share/doc/%{name}-%{version}/COPYING
%doc %{_prefix}/share/doc/%{name}-%{version}/example-1.html
%doc %{_prefix}/share/doc/%{name}-%{version}/example-2.html
%doc %{_prefix}/share/doc/%{name}-%{version}/example-3.html
%doc %{_prefix}/share/doc/%{name}-%{version}/example-4.html
%doc %{_prefix}/share/doc/%{name}-%{version}/example-5.html
%doc %{_prefix}/share/doc/%{name}-%{version}/example.html
%doc %{_prefix}/share/doc/%{name}-%{version}/example.lyx
%doc %{_prefix}/share/doc/%{name}-%{version}/example.sgml 
%dir %{_prefix}/share/doc/%{name}-%{version}/money
%doc %{_prefix}/share/doc/%{name}-%{version}/money/money.h
%doc %{_prefix}/share/doc/%{name}-%{version}/money/money.c
%{_prefix}/share/doc/%{name}-%{version}/money/check_money.c
%doc %{_prefix}/share/doc/%{name}-%{version}/money/aclocal.m4
%doc %{_prefix}/share/doc/%{name}-%{version}/money/configure.in
%doc %{_prefix}/share/doc/%{name}-%{version}/money/Makefile.am
%doc %{_prefix}/share/doc/%{name}-%{version}/money/Makefile.in
%doc %{_prefix}/share/doc/%{name}-%{version}/money/configure
%doc %{_prefix}/share/doc/%{name}-%{version}/money/README
%doc %{_prefix}/share/doc/%{name}-%{version}/money/AUTHORS
%doc %{_prefix}/share/doc/%{name}-%{version}/money/COPYING
%doc %{_prefix}/share/doc/%{name}-%{version}/money/INSTALL
%doc %{_prefix}/share/doc/%{name}-%{version}/money/ChangeLog
%doc %{_prefix}/share/doc/%{name}-%{version}/money/NEWS
%doc %{_prefix}/share/doc/%{name}-%{version}/money/stamp-h.in
%doc %{_prefix}/share/doc/%{name}-%{version}/money/config.h.in

%changelog
* Sat Jun 2 2001 Arien Malec <arien_malec@yahoo.com>
- First packaging.
