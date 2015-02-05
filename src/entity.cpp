//Copyright (c) 2015 Roman Chistokhodov

#include <cmath>

#include <algorithm>

#include "entity.h"

unsigned colorFromFloat(float f)
{
    unsigned u = static_cast<unsigned>(ceil(f*255));
    if (u > 255)
        u = 255;
    return u;
}

std::string withoutQuotes(std::string str)
{
    return std::string(str.begin(), std::remove(str.begin(), str.end(), '"'));
}

Key::Key(const std::string& keyname, const std::string& keydescription) : name(keyname), description(keydescription)
{
    
}

Key::Key(const std::string& keyname, const std::string& keydescription, const std::string& keytype) : name(keyname), description(keydescription), type(keytype)
{
    
}

Entity::Entity() : solid(false)
{
    for (size_t i=0; i<6; ++i)
        box[i] = 0;
    color[0] = 200;
    color[1] = 0;
    color[2] = 200;
}

std::ostream& operator<<(std::ostream& out, const Entity& entity)
{
    out << entity.name << (entity.solid ? " solid" : " point");
    for (size_t i=0;i<6;++i)
        out << " " << entity.box[i];
    out << std::endl;
    for (size_t i=0; i<entity.keys.size(); ++i)
    {
        out << entity.keys[i].name << " ";
    }
    if (!entity.model.empty())
        out << "model=" <<entity.model;
    out << std::endl;
    return out;
}
