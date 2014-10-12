#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

struct Key
{
    Key(const std::string& keyname, const std::string& keydescription);
    std::string name;
    std::string description;
};

struct Entity
{
    Entity();
    std::string name;
    std::string description;
    std::vector<Key> keys;
    std::string spawnflags[32];
    std::string flagsdescriptions[32];
    unsigned color[3];
    int size[6];
    bool solid;
    std::string model;
};

#endif
