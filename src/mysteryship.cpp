#include "mysteryship.hpp"

Mysteryship::Mysteryship()
{
    image = LoadTexture("Graphics/mystery.png");
    alive = false;
}

Mysteryship::~Mysteryship()
{
    UnloadTexture(image);
}

void Mysteryship::Update()
{
    if (alive)
    {
        position.x += speed;
        if (position.x > GetScreenWidth() - image.width - 25 || position.x < 25)
        {
            alive = false;
        }
        
    }
    
}

void Mysteryship::Draw()
{
    if (alive)
    {
        DrawTextureV(image, position, WHITE);
    }
    
}

void Mysteryship::Spawn()
{
    position.y = 90;
    int side = GetRandomValue(0, 1);

    if (side == 0)
    {
        position.x = 25;
        speed = 4;
    } else {
        position.x = GetScreenWidth() - image.width - 25;
        speed = -4;
    }
    alive = true;   
}

Rectangle Mysteryship::getRect()
{
    if (alive)
    {
        return {position.x, position.y, image.width, image.height};
    } else {
        return {position.x, position.y, 0, 0};
    }
    
}
