#ifndef TRANSLATE
#define TRANSLATE

#include <string>

void generateLocale(const char* package, const char* localeDir, const char* locale = "");
std::string translate(const char* str);
std::string translate_n(const char* single, const char* mult, unsigned long int n);

#endif

