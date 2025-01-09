#include <raylib.h>
#include "game.hpp"
#include <string>

// Função para formatar números com zeros à esquerda
std::string FormatWithLeadingZeros(int number, int width) {
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    return std::string(leadingZeros, '0') + numberText;
}

int main() {
    // Cores
    Color grey = {29, 29, 27, 255};
    Color yellow = {243, 216, 63, 255};

    // Tamanho da janela fixo
    int windowWidth = 800;
    int windowHeight = 660;

    InitWindow(windowWidth, windowHeight, "C++ Space Invaders");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        // Atualiza o jogo
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();

        BeginDrawing();
        ClearBackground(grey);

        // HUD
        DrawRectangleRoundedLines({10, 10, 780, 580}, 0.18f, 20, yellow); // Borda da tela
        DrawLineEx({25, 550}, {775, 550}, 3, yellow); // Linha inferior

        if (game.run) {
            DrawTextEx(font, "LEVEL 01", {(float)windowWidth - 200, 560}, 34, 2, yellow);
        } else {
            DrawTextEx(font, "GAME OVER", {(float)windowWidth - 200, 560}, 28, 2, yellow);
        }

        // Vidas do jogador
        float x = 50.0f;
        for (int i = 0; i < game.lives; i++) {
            DrawTextureV(spaceshipImage, {x, 565}, WHITE);
            x += 50;
        }

        // Score (esquerda)
        DrawTextEx(font, "SCORE", {50, 15}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 50}, 34, 2, yellow);

        // High Score (direita)
        DrawTextEx(font, "HIGH-SCORE", {(float)windowWidth - 220, 15}, 30, 2, yellow);
        std::string highscoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highscoreText.c_str(), {(float)windowWidth - 150, 50}, 34, 2, yellow);

        // Desenha o jogo
        game.Draw();

        EndDrawing();
    }

    // Finaliza o programa
    CloseWindow();
    CloseAudioDevice();
}
