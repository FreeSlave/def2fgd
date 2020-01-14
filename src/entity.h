//Copyright (c) 2015 Roman Chistokhodov

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <ostream>

unsigned colorFromFloat(float f);
std::string withoutQuotes(std::string str);

struct Key
{
    Key(const std::string& keyname, const std::string& keydescription);
    Key(const std::string& keyname, const std::string& keydescription, const std::string& keytype);
    std::string name;
    std::string description;
    std::string type;
};

struct Entity
{
    enum {SpawnFlagNum = 32};

    Entity();
    std::string name;
    std::string description;
    std::vector<Key> keys;
    std::string spawnflags[SpawnFlagNum];
    std::string flagsdescriptions[SpawnFlagNum];
    unsigned color[3];
    int box[6];
    bool solid;
    std::string model;

    bool hasKey(const std::string& name) const;
};

std::ostream& operator<<(std::ostream& out, const Entity& entity);

#endif
