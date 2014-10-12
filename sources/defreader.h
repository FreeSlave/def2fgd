#ifndef DEFREADER_H
#define DEFREADER_H

#include <stdexcept>
#include "entity.h"

class DefReadError : public std::runtime_error
{
public:
    DefReadError(const std::string& what, size_t line, size_t column);
    
    size_t line() const;
    size_t column() const;
    
private:
    size_t _line;
    size_t _column;
};

std::vector<Entity> readDefFile(const char* fileName);

#endif
