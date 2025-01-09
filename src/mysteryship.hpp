#pragma once
#include <raylib.h>

class Mysteryship
{
    private:
        Vector2 position;
        Texture2D image;
        int speed;
    
    public:
        Mysteryship();
        ~Mysteryship();
        void Update();
        void Draw();
        void Spawn();
        Rectangle getRect();
        bool alive;
};