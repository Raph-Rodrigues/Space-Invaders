#pragma once
#include <raylib.h>
#include "laser.hpp"
#include <vector>

class Spaceship
{
    private:
        Texture2D texture;
        Vector2 position;
        double lastFireTime;
        Sound laserSound;
    
    public:
        Spaceship();
        ~Spaceship();
        void Draw();
        void MoveLeft();
        void MoveRight();
        void FireLaser();
        void Reset();
        Rectangle getRect();
        std::vector<Laser> lasers;
};
