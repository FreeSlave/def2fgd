//Copyright (c) 2015 Roman Chistokhodov

#ifndef ENTREADER_H
#define ENTREADER_H

#include <fstream>
#include <vector>
#include "entity.h"

std::vector<Entity> readEntFile(std::istream& stream);

#endif
