#include <iostream>
#include "game.hpp"
#include <fstream>

Game::Game()
{
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    InitGame();
}

Game::~Game() {
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

#pragma region Update&Draw Game&HandleInputs
void Game::Update() {
    if (run) {

        double currentTime = GetTime();
        if (currentTime - timeLastSpawn > mysteryShipSpawnInterval)
        {
            mysteryship.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
        }
        
        for (auto& laser: player.lasers)
        {
            laser.Update();
        }

        MoveAliens();

        AlienShooterLaser();

        for(auto& laser: aliensLaser) {
            laser.Update();
        }

        DeleteInactvieLasers();
        mysteryship.Update();
        CheckForCollision();
    } else {
        if (IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        } else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
            Reset();
            InitGame();
        }
        
    }
}

void Game::Draw() {
    player.Draw();

    for (auto& laser: player.lasers)
    {
        laser.Draw();
    }

    for (auto& obstacle: obstacles) {
        obstacle.Draw();
    }

    for (auto& alien: aliens) {
        alien.Draw();
    }

    for (auto& laser: aliensLaser) {
        laser.Draw();
    }

    mysteryship.Draw();
    
}

void Game::HandleInput() {
    if (run) {

        if (IsKeyDown(KEY_A) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.5f) 
        {
            player.MoveLeft();
        }

        else if (IsKeyDown(KEY_D) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.5f)
        {
            player.MoveRight();
        }

        else if (IsKeyDown(KEY_F) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
            player.FireLaser();
        }

    }
}
#pragma endregion

#pragma region DeleteInactvieLasers
void Game::DeleteInactvieLasers()
{
    for (auto it = player.lasers.begin(); it != player.lasers.end();)
    {
        if (!it -> active)
        {
            it = player.lasers.erase(it);
        } else
        {
            ++ it;
        }
    }

    for (auto it = aliensLaser.begin(); it != aliensLaser.end();)
    {
        if (!it -> active)
        {
            it = aliensLaser.erase(it);
        } else
        {
            ++ it;
        }
    }
    
}

#pragma endregion


#pragma region CreateObstacles
std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

    for (size_t i = 0; i < 4; i++)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float (GetScreenHeight() - 200)}));
    }

    return obstacles;
    
}
#pragma endregion

#pragma region Aliens
std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for (int row = 0; row < 5; row++)
    {
        for (int collumn = 0; collumn < 11; collumn++)
        {

            int alienType;
            if (row == 0)
            {
                alienType = 3;
            } else if (row == 1 || row == 2) {
                alienType = 2;
            } else {
                alienType = 1;
            }

            float x = 75 + collumn * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }

    return aliens;
    
}

void Game::MoveAliens()
{
    for (auto& alien: aliens)
    {
        if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25)
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        }

        if (alien.position.x < 25)
        {
            aliensDirection = 1;
            MoveDownAliens(4);
        }
        
        alien.Update(aliensDirection);
    }
    
}

void Game::MoveDownAliens(int distance)
{
    for (auto& alien: aliens) {
        alien.position.y += distance;
    }
}

void Game::AlienShooterLaser()
{
    double currentTime = GetTime();

    if (currentTime - timeLastAlienFired >= alienShootInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        aliensLaser.push_back(Laser({alien.position.x + alien.alienImages[alien.type -1].width/2, 
                                    alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
        timeLastAlienFired = GetTime();
    }
}
#pragma endregion

#pragma region CheckCollisions
void Game::CheckForCollision()
{
    // Spaceship Lasers

    for (auto& Laser: player.lasers) {
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            if (CheckCollisionRecs(it -> getRect(), Laser.getRect()))
            {
                
                PlaySound(explosionSound);
                if (it -> type == 1)
                {
                    score += 100;
                } else if (it -> type == 2) {
                    score += 200;
                } else if (it -> type == 3) {
                    score += 300;
                }

                CheckForHighScore();

                it = aliens.erase(it);
                Laser.active = false;
            } else {
                ++ it;
            }
        }

        for (auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it -> getRect(), Laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    Laser.active = false;
                } else {
                    ++ it;
                }
            }
        }

        if (CheckCollisionRecs(mysteryship.getRect(), Laser.getRect()))
        {
            mysteryship.alive = false;
            Laser.active = false;
            score += 500;

            CheckForHighScore();
            PlaySound(explosionSound);
        }

    }

    // Alien Lasers
    for(auto& laser: aliensLaser) {
        if (CheckCollisionRecs(laser.getRect(), player.getRect()))
        {
            laser.active = false;
            lives --;
            if (lives == 0)
            {
                GameOver();
            }
            
        }
        
        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it -> getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++ it;
                }
            }
            
        }
    }

    // Alien Collision with Obstacle

    for (auto& alien: aliens) {
        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it -> getRect(), alien.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                } else {
                    it ++;
                }
            }
        }

        if (CheckCollisionRecs(alien.getRect(), player.getRect()))
        {
            GameOver();
        }
        
    }
}

#pragma endregion

#pragma region GameOver&Reset&InitGame

void Game::GameOver()
{
    run = false;
}

void Game::Reset()
{
    player.Reset();
    aliens.clear();
    aliensLaser.clear();
    obstacles.clear();
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;
    timeLastSpawn = 0.0;
    lives = 4;
    score = 0;
    highScore = loadHighScoreFromFile();
    run = true;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
}

#pragma endregion

#pragma region HighScore

void Game::CheckForHighScore()
{
    if (score > highScore)
    {
        highScore = score;
        saveHighScoreToFile(highScore);
    }
    
}

int Game::loadHighScoreFromFile()
{
    int loadHighScore = 0;
    std::ifstream highScoreFile("highscore.txt");

    if (highScoreFile.is_open())
    {
        highScoreFile >> loadHighScore;
        highScoreFile.close();
    } else {
        std::cerr << "Failed to load high score from file." << std::endl;
    }
    return loadHighScore;
}

void Game::saveHighScoreToFile(int highScore)
{
    std::ofstream highScoreFile("highscore.txt");

    if (highScoreFile.is_open())
    {
        highScoreFile << highScore;
        highScoreFile.close();
    } else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
    
}

#pragma endregion