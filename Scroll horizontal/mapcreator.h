#ifndef MAP_CREATOR
#define MAP_CREATOR

#include "block.h"

#include <string>

bool toFile(Block ***m, int mX, int mY,
            Vec2d player, Vec2d camera, std::string fileName);

bool fromFile(Block ***&m, int &mX, int &mY,
              Vec2d &player, Vec2d &camera, std::string fileName);

#endif // MAP_CREATOR
