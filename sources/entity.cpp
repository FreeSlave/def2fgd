#include "entity.h"

Key::Key(const std::string& keyname, const std::string& keydescription) : name(keyname), description(keydescription)
{
    
}

Entity::Entity() : solid(false)
{
    for (size_t i=0; i<6; ++i)
        size[i] = 0;
    color[0] = 200;
    color[1] = 0;
    color[2] = 200;
}