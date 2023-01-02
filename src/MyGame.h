#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"

#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

// Store game data received from server
static struct GameData {
    
    int player1X = 0;
    int player1Y = 0;

    int player2X = 0;
    int player2Y = 0;
    
    int ballX = 0;
    int ballY = 0;

    int powerUpX = 0;
    int powerUpY = 0;

    int player1Score = 0;
    int player2Score = 0;

    float ballRadius;
    float powerUpRadius;

} game_data;

class Particle {
    public:

        // position of particle X
        
        int x;
        // position of particle Y
        int y;

        // timer
        double t = 0.0;

    bool requireUpdate() {
        return t >= 0.1;
    }
};


class MyGame {
    
    private:
        
        // Player 1
        SDL_Rect player1 = { 0, 0, 20, 60 };
        
        // Player 2 
        SDL_Rect player2 = { 0, 0, 20, 60 };

        int player1Points;
        int player2Points;

        // Check if the powerup ball has been created.
        bool powerUpCreated;
        
        // Powerup ball
        SDL_Rect powerUp;

        // Main Ball
        SDL_Rect ball;

        // Player 1 & 2 Texture
        SDL_Texture* player1Texture;
        SDL_Texture* player2Texture;
        
        // Ball Texture
        SDL_Texture* ballTexture;

        // Power Up ball Texture
        SDL_Texture* powerUpTexture;

        // Loaded Font file
        TTF_Font* fontText;
        
        // Background of Game Size
        SDL_Rect background = {0,0,800,600};
        // Texture of Background Image
        SDL_Texture* backgroundTexture;

        // Loaded bounce sound file
        Mix_Chunk* bounceSound;

        // Ball Particles
        std::vector<Particle*> ballParticles;

        // Power Up Particles
        std::vector<Particle*> powerUpParticles;
    public:
        std::vector<std::string> messages;
        // Receive message from server
        void on_receive(std::string message, std::vector<std::string>& args);
        
        // Send message to sever
        void send(std::string message);
        
        // Key Input 
        void input(SDL_Event& event);
        
        // Update game
        void update();

        // Load Assets
        void loadAssets(SDL_Renderer* renderer);
        // Load Image
        SDL_Texture* loadImage(SDL_Renderer* renderer, char* fileName);
        // Load Sound
        Mix_Chunk* loadSound(SDL_Renderer* renderer, char* fileName);
        // Load Font
        TTF_Font* loadFont(SDL_Renderer* renderer, char* fileName);

        // Creating Particles
        void CreateBallParticles(int x, int y);
        void CreatePowerUpParticles(int x, int y);
        
        // Writes Score
        void writeScore(SDL_Renderer* renderer, TTF_Font* font);
        
        // Render Game
        void render(SDL_Renderer* renderer);

};

#endif