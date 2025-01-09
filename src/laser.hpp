#pragma once
#include "raylib.h"

class Laser
{
private:
    Vector2 position;
    int speed;
    
public:
    Laser(Vector2 postion, int speed);
    ~Laser();
    void Update();
    void Draw();
    Rectangle getRect();
    bool active;
};
