#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game {
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int lives;
        int score;
        int highScore;
        Music music;
    private:
        void DeleteInactvieLasers();
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShooterLaser();
        void CheckForCollision();
        void GameOver();
        void Reset();
        void InitGame();
        void CheckForHighScore();
        void saveHighScoreToFile(int highScore);
        int loadHighScoreFromFile();
        Spaceship player;
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        int aliensDirection;
        std::vector<Laser> aliensLaser;
        constexpr static float alienShootInterval = 0.35;
        float timeLastAlienFired;
        Mysteryship mysteryship;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        Sound explosionSound;
};
