#include "translate.h"

#ifdef USE_GETTEXT
#include <locale.h>
#include <libintl.h>

void generateLocale(const char* package, const char* localeDir, const char* locale)
{
    setlocale (LC_CTYPE, locale);
    setlocale (LC_MESSAGES, locale);
    bindtextdomain (package, localeDir);
    textdomain (package);
}

std::string translate(const char* str)
{
    return gettext(str);
}

std::string translate_n(const char* single, const char* mult, unsigned long int n)
{
    return ngettext(single, mult, n);
}

#elif defined(USE_BOOST_LOCALE)
#include <boost/locale.hpp>

void generateLocale(const char* package, const char* localeDir, const char* locale)
{
    boost::locale::generator gen;

    gen.add_messages_path(localeDir);
    gen.add_messages_domain(package);
    std::locale::global(gen(locale));
    std::cout.imbue(std::locale());
    std::cerr.imbue(std::locale());
}

std::string translate(const char* str)
{
    return boost::locale::gettext(str);
}

std::string translate_n(const char* single, const char* mult, unsigned long int n)
{
    return boost::locale::ngettext(single, mult, n);
}

#else

void generateLocale(const char* package, const char* localeDir, const char* locale)
{
    (void)package;
    (void)localeDir;
    (void)locale;
}

std::string translate(const char* str)
{
    return str;
}

std::string translate_n(const char* single, const char* mult, unsigned long int n)
{
    if (n == 1) {
        return single;
    } else {
        return mult;
    }
}

#endif
