#pragma once

#include "raylib.h"

class Block
{
private:
    Vector2 position;
public:
    Block(Vector2 position);
    ~Block();
    void Draw();
    Rectangle getRect();
};
