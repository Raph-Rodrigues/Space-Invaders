#include "spaceship.hpp"

Spaceship::Spaceship()
{
    texture = LoadTexture("Graphics/spaceship.png");
    position.x = (GetScreenWidth() - texture.width) / 2;
    position.y = GetScreenHeight() - texture.height - 100;
    lastFireTime = 0.0;
    laserSound = LoadSound("Sounds/laser.ogg");
}

Spaceship::~Spaceship() {
    UnloadTexture(texture);
    UnloadSound(laserSound);
}

void Spaceship::Draw() {
    DrawTextureV(texture, position, WHITE);
}

void Spaceship::MoveLeft() {
    position.x -= 10;

    if (position.x < 25)
    {
        position.x = 25;
    }
    
}

void Spaceship::MoveRight() {
    position.x += 6;

    if (position.x > GetScreenWidth() - texture.width - 25)
    {
        position.x = GetScreenWidth() - texture.width - 25;
    }
}

void Spaceship::FireLaser() {
    if (GetTime() - lastFireTime >= 0.15)
    {
        lasers.push_back(Laser({position.x + texture.width / 2 - 2, position.y}, -8));
        lastFireTime = GetTime();
        PlaySound(laserSound);
    }
    
}

void Spaceship::Reset()
{
    position.x = (GetScreenWidth() - texture.width) / 2;
    position.y = GetScreenHeight() - texture.height - 100;
    lasers.clear();
}

Rectangle Spaceship::getRect()
{
    return {position.x, position.y, float(texture.width), float(texture.height)};
}
