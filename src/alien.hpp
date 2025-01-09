#pragma once
#include "raylib.h"

class Alien
{
private:
    
public:
    Alien(int type, Vector2 position);
    ~Alien();
    void Update(int direction);
    void Draw();
    int GetType();
    Rectangle getRect();
    static void UnloadImages();
    static Texture2D alienImages[3];
    Vector2 position;
    int type;
};
