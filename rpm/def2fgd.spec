Name:           def2fgd
Version:        1.0
Release:        1%{?dist}
Summary:        A small tool to convert .def and .ent files to .fgd.

License:        MIT
URL:            https://bitbucket.org/FreeSlave/%{name}
Source0:        https://bitbucket.org/FreeSlave/%{name}-%{version}.tar.gz

BuildRequires:  gcc-c++ gettext

%description
def2fgd converts .def.and .ent files used by GtkRadiant and Netradiant to .fgd used by Jackhammer editor.

%prep
%setup -q


%build
make %{?_smp_mflags} CXXFLAGS="%{optflags}" USE_LOCALE=gettext LOCALEDIR=/usr/share/locale
make translations


%install
make DESTDIR=%{buildroot} prefix=/usr install 
make DESTDIR=%{buildroot} prefix=/usr install-translations
%find_lang %{name}

%files -f %{name}.lang
%{_bindir}/%{name}

%doc license.txt
%{_mandir}/man1/*


%changelog
* Thu Nov 12 2015 Roman Chistokhodov 1.0-1
- Initial release
