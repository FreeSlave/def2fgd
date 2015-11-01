//Copyright (c) 2015 Roman Chistokhodov

#include <cctype>
#include <cstdlib>
#include <cstring>

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <iostream>


#include "entreader.h"
#include "rapidxml.hpp"

using namespace rapidxml;

namespace
{
    static bool hasName(xml_base<>* elem, const char* name)
    {
        return strncmp(elem->name(), name, elem->name_size()) == 0;
    }
    
    static std::string valueString(xml_base<>* elem)
    {
        return std::string(elem->value(), elem->value_size());
    }
    
    static void readColor(const char* begin, size_t size, unsigned* color)
    {
        const char* end = begin + size;
        const char* current = begin;
        for (size_t i=0; i<3 && current != end; ++i)
        {
            while(current != end && isspace(*current))
                current++;
            const char* start = current;
            while(current != end && (isdigit(*current) || *current == '.'))
                current++;
            color[i] = colorFromFloat(strtod(start, NULL));
        }
    }
    
    static void readBox(const char* begin, size_t size, int* box)
    {
        const char* end = begin + size;
        const char* current = begin;
        for (size_t i=0; i<6 && current != end; ++i)
        {
            while(current != end && isspace(*current))
                current++;
            const char* start = current;
            while(current != end && (isdigit(*current) || *current == '-'))
                current++;
            box[i] = static_cast<int>(strtol(start, NULL, 10));
        }
    }
    
    static const char* entTypeToFgdType(const char* type, size_t size)
    {
        if (strncmp(type, "angles", size) == 0)
            return "angle";
        if (strncmp(type, "real", size) == 0)
            return "float";
        if (strncmp(type, "integer", size) == 0)
            return "integer";
        if (strncmp(type, "sound", size) == 0)
            return "sound";
        if (strncmp(type, "model", size) == 0)
            return "studio";
        return "";
    }
}

std::vector<Entity> readEntFile(std::istream& stream)
{   
    std::vector<char> input((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    std::vector<Entity> toReturn;
    
    xml_document<> doc;
    doc.parse<parse_trim_whitespace|parse_normalize_whitespace>(&input[0]);
    
    xml_node<>* classes = doc.first_node("classes");
    if (!classes)
        throw std::runtime_error("No 'classes' entry found");
    
    for (xml_node<>* entityNode = classes->first_node(); entityNode; entityNode = entityNode->next_sibling())
    {
        Entity entity;
        
        entity.description = withoutQuotes(std::string(entityNode->value(), entityNode->value_size()));
        
        if (hasName(entityNode, "point"))
            entity.solid = false;
        else if (hasName(entityNode, "group"))
            entity.solid = true;
        
        xml_attribute<>* nameAttr = entityNode->first_attribute("name");
        if (!nameAttr)
            continue; // no name, nothing to do here
        
        entity.name = valueString(nameAttr);
        
        xml_attribute<>* colorAttr = entityNode->first_attribute("color");
        if (colorAttr)
            readColor(colorAttr->value(), colorAttr->value_size(), entity.color);
        
        xml_attribute<>* boxAttr = entityNode->first_attribute("box");
        if (boxAttr)
            readBox(boxAttr->value(), boxAttr->value_size(), entity.box);
        
        xml_attribute<>* modelAttr = entityNode->first_attribute("model");
        if (modelAttr) {
            entity.model = valueString(modelAttr);
        }
        
        for (xml_node<>* keyNode = entityNode->first_node(); keyNode; keyNode = keyNode->next_sibling())
        {
            if (hasName(keyNode, "flag"))
            {
                xml_attribute<>* keyAttr = keyNode->first_attribute("key");
                xml_attribute<>* bitAttr = keyNode->first_attribute("bit");
                
                if (keyAttr && bitAttr)
                {
                    size_t flagnum = static_cast<size_t>(strtol(bitAttr->value(), NULL, 10));
                    if (flagnum >= 0 && flagnum < Entity::SpawnFlagNum)
                    {
                        entity.spawnflags[flagnum] = valueString(keyAttr);
                        entity.flagsdescriptions[flagnum] = withoutQuotes(valueString(keyNode));
                    }
                }
            }
            else
            {
                xml_attribute<>* keyAttr = keyNode->first_attribute("key");
                if (keyAttr)
                {
                    entity.keys.push_back(Key(
                        valueString(keyAttr), 
                        withoutQuotes(valueString(keyNode)), 
                        entTypeToFgdType(keyNode->name(), keyNode->name_size())
                    ));
                }
            }
            
            
            xml_node<>* lastNode = entityNode->last_node();
            if (lastNode && entity.model.empty())
            {
                std::string val = valueString(lastNode);
                size_t pos = val.find("modeldisabled=");
                if (pos != std::string::npos)
                {
                    std::string::iterator it = val.begin() + pos + strlen("modeldisabled=");
                    if (*it == '"')
                    {
                        it++;
                        std::string::iterator start = it;
                        while(it != val.end() && *it != '"')
                            it++;
                        std::string modelname(start, it);
                        entity.model = modelname;
                    }
                }
            }
        }
        
        toReturn.push_back(entity);
    }
    
    return toReturn;
}
