//Copyright (c) 2015 Roman Chistokhodov

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>

#include <stdexcept>
#include "defreader.h"

namespace
{
    const char* const quakedstr = "QUAKED";
    size_t quakednum = 6;
    const char* const unexpectedeof = "Unexpected end of line";
    
    static std::string::iterator skipSpaces(std::string::iterator current, const std::string::iterator& end)
    {
        while(current != end && isspace(*current))
            current++;
        return current;
    }
    
    static std::string::iterator skipAlpha(std::string::iterator current, const std::string::iterator& end)
    {
        while(current != end && (isalnum(*current) || *current == '_'))
            current++;
        return current;
    }

    static std::string::iterator skipDigits(std::string::iterator current, const std::string::iterator& end)
    {
        while(current != end && isdigit(*current))
            current++;
        return current;
    }

    static std::string::iterator skipFloat(std::string::iterator current, const std::string::iterator& end)
    {
        while(current != end && (isdigit(*current) || *current == '.'))
            current++;
        return current;
    }

    static std::string::iterator readBox(std::string::iterator it, size_t lineNum, const std::string::iterator& begin, const std::string::iterator& end, int* size)
    {
        if (*it == '(')
        {
            it++;
            for (int i=0; i<3; ++i)
            {
                it = skipSpaces(it, end);
                if (it == end)
                    throw DefReadError(unexpectedeof, lineNum, it-begin);
                std::string::iterator start = it;
                if (*it == '-')
                    it++;
                if (it == end)
                    throw DefReadError(unexpectedeof, lineNum, it-begin);
                it = skipDigits(it, end);
                if (it == end)
                    throw DefReadError(unexpectedeof, lineNum, it-begin);
                std::string numstr(start, it);
                size[i] = static_cast<int>(strtol(numstr.c_str(), NULL, 10));
            }
            if (*it == ')')
            {
                it++;
                return it;
            }
        }
        throw DefReadError("Unexpected symbol", lineNum, it-begin);
    }

    static std::string::iterator readFlags(std::string::iterator it, size_t lineNum, const std::string::iterator& begin, const std::string::iterator& end, std::string* flags)
    {
        if (it == end)
            return it;
        int i=0;
        while(it != end && (isalnum(*it) || (*it) == '-'))
        {
            if (*it == '-')
            {
                it++;
            }
            else
            {
                std::string::iterator start = it;
                it = skipAlpha(it, end);
                flags[i] = std::string(start, it);
            }
            it = skipSpaces(it, end);
            i++;
        }
        return it;
    }
}

DefReadError::DefReadError(const std::string& what, size_t line, size_t column) : std::runtime_error(what), _line(line), _column(column)
{
    
}

size_t DefReadError::line() const
{
    return _line;
}

size_t DefReadError::column() const
{
    return _column;
}

std::vector<Entity> readDefFile(const char* fileName)
{
    std::ifstream stream(fileName);
    
    if (!stream)
    {
        throw std::runtime_error("Could not open .def file for reading");
    }
    
    std::vector<Entity> toReturn;
    
    size_t lineNum = 0;
    std::string line;
    while(getline(stream, line))
    {
        lineNum++;
        
        if (line.empty())
            continue;
        
        std::string::iterator it = line.begin();
        std::string::iterator begin = line.begin();
        std::string::iterator end = line.end();
        
        if (*it == '/')
        {
            it++;
            if (*it == '/')
                continue; //skip comment
            else if (*it == '*')
            {
                it++;
                if (end - it > quakednum && std::equal(quakedstr, quakedstr+quakednum, it)) // /*QUAKED starts here
                {
                    Entity entity;
                    advance(it, quakednum);
                    it = skipSpaces(it, end);
                    std::string::iterator start = it;
                    it = skipAlpha(it, end);
                    entity.name = std::string(start, it);
                    
                    it = skipSpaces(it, end);
                    if (it != end)
                    {
                        if (*it == '(')
                        {
                            it++;
                            for (int i=0; i<3; ++i)
                            {
                                it = skipSpaces(it, end);
                                start = it;
                                it = skipFloat(it, end);
                                std::string numstr(start, it);
                                entity.color[i] = colorFromFloat(strtod(numstr.c_str(), NULL));
                            }
                            
                            while(*it != ')')
                            {
                                it++;
                                if (it == end) {
                                    throw DefReadError(unexpectedeof, lineNum, it-begin);
                                }
                            }
                            it++;
                            it = skipSpaces(it, end);
                            if (it != end)
                            {
                                if (*it == '?')
                                {
                                    entity.solid = true;
                                    it++;
                                }
                                else
                                {
                                    it = readBox(it, lineNum, begin, end, entity.box);
                                    it = skipSpaces(it, end);
                                    it = readBox(it, lineNum, begin, end, entity.box+3);
                                }
                                it = skipSpaces(it, end);
                                readFlags(it, lineNum, begin, end, entity.spawnflags);
                                
                                while(getline(stream, line))
                                {
                                    if (line.empty())
                                        continue;
                                    
                                    bool needbreak = false;
                                    if (line.size() > 1)
                                    {
                                        if (line[0] == '*' && line[1] == '/')
                                            break;
                                        if (line[line.size()-2] == '*' && line[line.size()-1] == '/')
                                        {
                                            line.erase(line.end()-2, line.end());
                                            needbreak = true;
                                        }
                                    }
                                    
                                    if (isalpha(line[0]) || line[0] == '_')
                                    {
                                        it = line.begin();
                                        begin = line.begin();
                                        end = line.end();
                                        start = it;
                                        it = skipAlpha(it, end);
                                        
                                        std::string keyname(start, it);
                                        it = skipSpaces(it, end);
                                        
                                        if (keyname == "model" && *it == '=')
                                        {
                                            it++;
                                            if (*it == '"')
                                            {
                                                it++;
                                                start = it;
                                                while(it != end && *it != '"')
                                                    it++;
                                                std::string modelname(start, it);
                                                entity.model = modelname;
                                            }
                                        }
                                        else
                                        {
                                            if (it == end  || *it != ':')
                                            {
                                                start = it;
                                                it = skipAlpha(it, end);
                                                if (std::string(start, it) == "OR") //check if this is the field with two names
                                                {
                                                    it = skipSpaces(it, end);
                                                    start = it;
                                                    it = skipAlpha(it, end);
                                                    keyname = std::string(start, it); //take the second name as a key name
                                                    it = skipSpaces(it, end);
                                                }
                                                else //description
                                                {
                                                    if (entity.description.empty())
                                                    {
                                                        entity.description = withoutQuotes(line);
                                                    }
                                                    else
                                                    {
                                                        entity.description += "\\n\\n";
                                                        entity.description += withoutQuotes(line);
                                                    }
                                                    it = end;
                                                }
                                            }
                                            if (it != end && *it == ':')
                                            {
                                                it++;
                                                it = skipSpaces(it, end);
                                                start = it;
                                                while(it != end)
                                                    it++;
                                                std::string description = withoutQuotes(std::string(start, it));
                                                std::string* found = std::find(entity.spawnflags, entity.spawnflags+Entity::SpawnFlagNum, keyname);
                                                
                                                if (found != entity.spawnflags+Entity::SpawnFlagNum) //it's spawnflag
                                                    entity.flagsdescriptions[found-entity.spawnflags] = description;
                                                else //it's field
                                                    entity.keys.push_back(Key(keyname, description));
                                            }
                                        }
                                        
                                        
                                        
                                    }
                                    if (needbreak)
                                        break;
                                }
                                
                                toReturn.push_back(entity);
                            }
                            else
                            {
                                throw DefReadError(unexpectedeof, lineNum, it-begin);
                            }
                        }
                    }
                    else
                    {
                        throw DefReadError(unexpectedeof, lineNum, it-begin);
                    }
                }
                else
                {
                    throw DefReadError("Expected QUAKED", lineNum, it-begin);
                }
            }
        }
    }
    stream.close();
    return toReturn;
}
