Name:           def2fgd
Version:        1.2
Release:        1%{?dist}
Summary:        A small tool to convert .def and .ent files to .fgd

License:        MIT
URL:            https://bitbucket.org/FreeSlave/%{name}
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++ gettext

%description
def2fgd converts .def.and .ent files 
used by GtkRadiant and Netradiant to .fgd 
used by J.A.C.K. editor.

%prep
%setup -q


%build
make %{?_smp_mflags} CXXFLAGS="%{optflags}" USE_LOCALE=gettext LOCALEDIR=/usr/share/locale
make translations


%install
make DESTDIR=%{buildroot} prefix=/usr install 
make DESTDIR=%{buildroot} prefix=/usr install-translations
make DESTDIR=%{buildroot} prefix=/usr install-bash-completion
%find_lang %{name}

%files -f %{name}.lang
%{_datadir}/bash-completion/completions/%{name}
%{_bindir}/%{name}

%doc license.txt
%{_mandir}/man1/*
%{_mandir}/ru/man1/*


%changelog
* Thu Oct 20 2016 Roman Chistokhodov 1.2-1
- New version
* Mon Nov 23 2015 Roman Chistokhodov 1.1-1
- New version
* Thu Nov 12 2015 Roman Chistokhodov 1.0-1
- Initial release
