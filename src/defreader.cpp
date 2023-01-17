//Copyright (c) 2015 Roman Chistokhodov

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>

#include <stdexcept>
#include "defreader.h"
#include "translate.h"

namespace
{
    const char* const quakedstr = "QUAKED";
    size_t quakednum = 6;
    std::string unexpectedEndOfFile() {
        return translate("Unexpected end of line");
    };

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

    static std::string::iterator readBox(std::string::iterator it, size_t lineNum, const std::string::iterator& begin, const std::string::iterator& end, int* size, int defaultSize)
    {
        if (*it == '(')
        {
            it++;
            for (int i=0; i<3; ++i)
            {
                it = skipSpaces(it, end);
                if (it == end)
                    throw DefReadError(unexpectedEndOfFile(), lineNum, it-begin);
                std::string::iterator start = it;
                if (*it == '-')
                    it++;
                if (it == end)
                    throw DefReadError(unexpectedEndOfFile(), lineNum, it-begin);
                it = skipDigits(it, end);
                if (it == end)
                    throw DefReadError(unexpectedEndOfFile(), lineNum, it-begin);
                std::string numstr(start, it);
                size[i] = static_cast<int>(strtol(numstr.c_str(), NULL, 10));
            }
            if (*it == ')')
            {
                it++;
                return it;
            }
        }
        else
        {
            size[0] = size[1] = size[2] = defaultSize;
            return it;
        }
        throw DefReadError(translate("Unexpected symbol"), lineNum, it-begin);
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

    static void stripRight(std::string& str)
    {
        if (str.size() && str[str.size()-1] == '\r') {
            str.resize(str.size()-1);
        }
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

std::vector<Entity> readDefFile(std::istream& stream)
{
    std::vector<Entity> toReturn;

    bool inKeys = false;
    bool shouldPush = false;
    size_t lineNum = 0;
    std::string line;

    Entity entity;

    while(getline(stream, line))
    {
        stripRight(line);
        lineNum++;

        if (line.empty()) {
            continue;
        }

        if (shouldPush) {
            toReturn.push_back(entity);
            entity = Entity();
            inKeys = false;
            shouldPush = false;
        }

        if (inKeys) {
            if (line.size() > 1)
            {
                if (line[0] == '*' && line[1] == '/') {
                    shouldPush = true;
                } else if (line[line.size()-2] == '*' && line[line.size()-1] == '/') {
                    line.resize(line.size()-2);
                    shouldPush = true;
                }
            }
        }

        std::string::iterator it = line.begin();
        std::string::iterator begin = line.begin();
        std::string::iterator end = line.end();
        std::string::iterator start = it;


        if (inKeys) {
            if (isalpha(line[0]) || line[0] == '_' || line[0] == '\"')
            {
                if (it != end && *it == '\"') {
                    it++;
                }

                start = it;

                std::string keyname;
                if (*begin == '\"') {
                    while(it != end && *it != '\"') {
                        it++;
                    }
                    if (it == end) {
                        throw DefReadError(translate("Expected pair quote"), lineNum, it-begin);
                    } else {
                        keyname = std::string(start, it);
                        it++;
                    }
                } else {
                    it = skipAlpha(it, end);
                    keyname = std::string(start, it);
                }

                it = skipSpaces(it, end);
                std::string* found = std::find(entity.spawnflags, entity.spawnflags+Entity::SpawnFlagNum, keyname);
                if (found != entity.spawnflags+Entity::SpawnFlagNum) {

                    if (it != end && *it == ':') {
                        it++;
                        it = skipSpaces(it, end);
                    }

                    entity.flagsdescriptions[found-entity.spawnflags] = withoutQuotes(std::string(it, end));
                    continue;
                }

                if (keyname == "model" && *it == '=')
                {
                    it++;
                    if (*it == '"') {
                        it++;
                        start = it;
                        while(it != end && *it != '"') {
                            it++;
                        }
                        std::string modelname(start, it);
                        entity.model = modelname;
                    }
                }
                else
                {
                    if ((it == end  || *it != ':') && *begin != '\"' )
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
                            if (entity.description.empty()) {
                                entity.description = withoutQuotes(line);
                            } else {
                                entity.description += "\\n\\n";
                                entity.description += withoutQuotes(line);
                            }
                            continue;
                        }
                    }
                    if (it != end && (*it == ':' || *begin == '\"'))
                    {
                        if (*it == ':') {
                            it++;
                        }
                        it = skipSpaces(it, end);

                        std::string description = withoutQuotes(std::string(it, end));

                        entity.keys.push_back(Key(keyname, description));
                    }
                }
            } else if (line[0] == ' ') {
                if (entity.description.empty()) {
                    entity.description = withoutQuotes(line);
                } else {
                    entity.description += "\\n";
                    entity.description += withoutQuotes(line);
                }
            }
        } else {
            if (*it == '/')
            {
                it++;
                if (*it == '/') {
                    continue; //skip comment
                } else if (*it == '*') {
                    it++;
                    if ( static_cast<size_t>(end - it) > quakednum && std::equal(quakedstr, quakedstr+quakednum, it)) // /*QUAKED starts here
                    {
                        advance(it, quakednum);
                        it = skipSpaces(it, end);
                        start = it;
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
                                    entity.color[i] = colorFromFloat(static_cast<float>(strtod(numstr.c_str(), NULL)));
                                }

                                while(*it != ')')
                                {
                                    it++;
                                    if (it == end) {
                                        throw DefReadError(unexpectedEndOfFile(), lineNum, it-begin);
                                    }
                                }
                                it++;
                                it = skipSpaces(it, end);
                                if (it != end)
                                {
                                    if (*it == '?') {
                                        entity.solid = true;
                                        it++;
                                    } else {
                                        it = readBox(it, lineNum, begin, end, entity.box, -8);
                                        it = skipSpaces(it, end);
                                        it = readBox(it, lineNum, begin, end, entity.box+3, 8);
                                    }
                                    it = skipSpaces(it, end);
                                    readFlags(it, lineNum, begin, end, entity.spawnflags);

                                    inKeys = true;
                                }
                                else
                                {
                                    throw DefReadError(unexpectedEndOfFile(), lineNum, it-begin);
                                }
                            }
                        }
                        else
                        {
                            throw DefReadError(unexpectedEndOfFile(), lineNum, it-begin);
                        }
                    }
                    else
                    {
                        throw DefReadError(translate("Expected QUAKED"), lineNum, it-begin);
                    }
                }
            }
        }
    }

    if (shouldPush) {
        toReturn.push_back(entity);
        entity = Entity();
    }

    return toReturn;
}
