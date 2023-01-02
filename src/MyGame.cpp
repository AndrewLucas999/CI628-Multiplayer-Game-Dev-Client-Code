#include "MyGame.h"
#include <string>

#pragma region Message Receive Handler

/// <summary>
/// Get data from server and setups the data according to each item
/// </summary>
/// <param name="cmd">Message from server</param>
void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {

#pragma region Game Data Handler

    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 7) {
            // Get Player 1 X and Y Position
            game_data.player1X = stoi(args.at(0));
            game_data.player1Y = stoi(args.at(1));

            // Get Player 2 X and Y Position
            game_data.player2X = stoi(args.at(2));
            game_data.player2Y = stoi(args.at(3));

            game_data.ballX = stoi(args.at(4));
            game_data.ballY = stoi(args.at(5));
            game_data.ballRadius = stoi(args.at(6));
        }
    }

#pragma endregion

#pragma region Score Handler

    else if (cmd == "SCORES")
    {
        if (args.size() == 2) {
            // Stores player score
            game_data.player1Score = stoi(args.at(0));
            game_data.player2Score = stoi(args.at(1));
        }
    }

#pragma endregion

#pragma region Power Up Message Handling
    else if (cmd == "GAME_DATA_PU") {
        if (args.size() == 3) {
            game_data.powerUpX = stoi(args.at(0));
            game_data.powerUpY = stoi(args.at(1));
            game_data.powerUpRadius = stoi(args.at(2));
        }
    }
    else if (cmd == "POWERUP_CREATED") {
        powerUpCreated = true;
    }
    else if (cmd == "POWERUP_DESTROYED") {
        powerUpCreated = false;
    }
    // Ball collision with bat
    else if (cmd == "BALL_HIT_BAT1" || cmd == "BALL_HIT_BAT2" || cmd == "POWERUP_HIT_BAT1" || cmd == "POWERUP_HIT_BAT2") {
        Mix_PlayChannel(-1, bounceSound, 0);
        std::cout << "Sound Played" << std::endl;
    }
    // Ball collision with wall
    else if (cmd == "HIT_WALL_UP" || cmd == "HIT_WALL_DOWN" || 
        cmd == "HIT_WALL_LEFT" || cmd == "HIT_WALL_RIGHT" ||
        cmd == "POWERUP_HIT_WALL_UP" || cmd == "POWERUP_HIT_WALL_DOWN" ||
        cmd == "POWERUP_HIT_WALL_LEFT" || cmd == "POWERUP_HIT_WALL_RIGHT") {
        Mix_PlayChannel(-1, bounceSound, 0);
        std::cout << "Sound Played" << std::endl;
    }

    // When powerup spawn in the server spawn in the client

#pragma endregion

#pragma region Log Command received

    else {
        std::cout << "Received: " << cmd << std::endl;
    }

#pragma endregion
    
}

#pragma endregion

#pragma region Send Message to Server Function

/**
 *  \brief Sends data back to server
 *
 *  \param message message to send to server
 *
*/
/// <summary>
/// Sends Data back to the server
/// </summary>
/// <param name="message">message of the input placed</param>
void MyGame::send(std::string message) {
    messages.push_back(message);
}

#pragma endregion

#pragma region Key Input Handler

/**
 *  \brief Player Input
 *
 *  \param event Key Press event
 *
*/
void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        // Player 1
        // W Key -> Up
    case SDLK_w:
        send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
        break;

        // S key -> Down
    case SDLK_s:
        send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
        break;


        // Player 2
        // I key -> Up
    case SDLK_i:
        send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
        break;

        // S key -> Down
    case SDLK_k:
        send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
        break;

    }
}

#pragma endregion

#pragma region Update 

/**
 *  \brief Update Loop
*/
void MyGame::update() {
    // Player 1
    player1.x = game_data.player1X;
    player1.y = game_data.player1Y;
    // Player 2
    player2.x = game_data.player2X;
    player2.y = game_data.player2Y;

    // Ball Position
    ball.x = game_data.ballX;
    ball.y = game_data.ballY;

    // Ball Width and height - Multiplied 2 by the radius to get circumference
    ball.w = game_data.ballRadius * 2;
    ball.h = game_data.ballRadius * 2;

    // Power Up Position
    powerUp.x = game_data.powerUpX;
    powerUp.y = game_data.powerUpY;

    // Power Up Width and Height - Multiplied 2 by the radius to get circumference
    powerUp.w = game_data.powerUpRadius * 2;
    powerUp.h = game_data.powerUpRadius * 2;

    // Player score
    player1Points = game_data.player1Score;
    player2Points = game_data.player2Score;

    #pragma region Ball Particles

    // Particles for Game ball
    for (auto p : ballParticles) {
        p->t += 0.009;
        if (p->requireUpdate()) {

            // 0.5 balances [-0.5 - 0.5]
            double randomVal = rand() * 1.0 / RAND_MAX - 0.5;
            if (randomVal > 0) {
                p->t = 0.0;
                randomVal = rand() * 1.0 / RAND_MAX - 0.5;
            }
            // Game Ball
            // Distance between each particle
            p->x = game_data.ballX + randomVal * 10;
            p->y = game_data.ballY + randomVal * 10;
          

        }
    }

    #pragma endregion

    #pragma region Power Up Particles

    // Particles for Game Power Up Ball
    for (auto p : powerUpParticles) {
        p->t += 0.009;
        if (p->requireUpdate()) {

            // 0.5 balances [-0.5 - 0.5]
            double randomVal = rand() * 1.0 / RAND_MAX - 0.5;
            if (randomVal > 0) {
                p->t = 0.0;
                randomVal = rand() * 1.0 / RAND_MAX - 0.5;
            }
            // Powerup  Ball
            p->x = game_data.powerUpX + randomVal * 10;
            p->y = game_data.powerUpY + randomVal * 10;

        }
    }

    #pragma endregion
}

#pragma endregion

#pragma region Load Particles

/**
 *  \brief Create Main Ball particles
 *
 *  \param posX Main ball position x
 *  \param posY Main ball position y
 *
*/
void MyGame::CreateBallParticles(int posX, int posY) {
    for (int i = 0; i < 10; i++) {
        Particle* p = new Particle();
        p->x = posX;
        p->y = posY;

        ballParticles.push_back(p);
    }
}
/**
 *  \brief Create Powerup particles
 *
 *  \param posX Powerup ball position x
 *  \param posY Powerup ball position y
 *
*/
void MyGame::CreatePowerUpParticles(int posX, int posY) {
    for (int i = 0; i < 10; i++) {
        Particle* p = new Particle();
        p->x = posX;
        p->y = posY;

        powerUpParticles.push_back(p);
    }
}

#pragma endregion

#pragma region Initialise Assets

/**
 *  \brief Loading All Assets.
 *
 *  \param renderer The renderer.
 *
*/
void MyGame::loadAssets(SDL_Renderer* renderer) {
    ballTexture = loadImage(renderer, "Assets/ball.png");
    powerUpTexture = loadImage(renderer, "Assets/powerupball.png");
    player1Texture = loadImage(renderer, "Assets/bat1.png");
    player2Texture = loadImage(renderer, "Assets/bat2.png");
    backgroundTexture = loadImage(renderer, "Assets/background.png");
    bounceSound = loadSound(renderer, "Assets/bounce.wav");
    fontText = loadFont(renderer, "Assets/stentiga.ttf");

    CreateBallParticles(game_data.ballX, game_data.ballY);
    CreatePowerUpParticles(game_data.powerUpX, game_data.powerUpY);
}

/**
 *  \brief Loading Image.
 *
 *  \param renderer The renderer.
 *  \param fileName File name of texture to be loaded.
 *
 *  \return Texture for the image loaded
 *
*/
SDL_Texture* MyGame::loadImage(SDL_Renderer* renderer, char* fileName) {
    SDL_Surface* surface = IMG_Load(fileName);
    // Check if file exists or not
    if (surface != nullptr) {
        std::cout << "Loaded" << std::endl;
    }
    else {
        std::cout << "Not Loaded" << std::endl;
    }

    // Create texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Free data when not using
    SDL_FreeSurface(surface);
    return texture;
}

TTF_Font* MyGame::loadFont(SDL_Renderer* renderer, char* fileName) {
    TTF_Font* font = TTF_OpenFont(fileName, 25);
    return font;
}

Mix_Chunk* MyGame::loadSound(SDL_Renderer* renderer, char* fileName) {
    return Mix_LoadWAV(fileName);
}

#pragma endregion


#pragma region Display Score

/// <summary>
/// Writing score onto GUI
/// </summary>
/// <param name="renderer"></param>
void MyGame::writeScore(SDL_Renderer* renderer, TTF_Font* font)
{

    // Player 1 Score
    // Converts string into char pointer so that it is in a format to display text as TTF_RenderText_Solid uses Char for the second parameter
    std::string score_text1 = std::to_string(player1Points);
    // Player 2 Score
    std::string score_text2 = std::to_string(player2Points);

    SDL_Color white = { 255,255,255 };
    SDL_Surface* surface1 = TTF_RenderText_Solid(font, score_text1.c_str(), white);
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, score_text2.c_str(), white);

    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);

    int textW = 0;
    int textH = 0;

    // Player 1 Score location
    SDL_QueryTexture(texture1, NULL, NULL, &textW, &textH);
    SDL_Rect dstrect1 = { 50, 50, textW, textH };
    SDL_RenderCopy(renderer, texture1, NULL, &dstrect1);

    // Player 2 Score Location
    SDL_QueryTexture(texture2, NULL, NULL, &textW, &textH);
    SDL_Rect dstrect2 = { 725, 50, textW, textH };
    SDL_RenderCopy(renderer, texture2, NULL, &dstrect2);

    // Player 1 Destroy
    SDL_FreeSurface(surface1);
    SDL_DestroyTexture(texture1);

    // Player 2 Destroy
    SDL_FreeSurface(surface2);
    SDL_DestroyTexture(texture2);

}

#pragma endregion

#pragma region Render

void MyGame::render(SDL_Renderer* renderer) {
    // Create the background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &background);

    // Creates ball
    SDL_RenderCopy(renderer, ballTexture, NULL, &ball);

    #pragma region Draw Ball Particles

    // Auto - for any data type
    // Drawing ball texture for each particle
    for (int i = 0; i < ballParticles.size(); i++) {
        // between 0 - 1
        double alphaRatio = i * 1.0 / ballParticles.size();
       
        // Blends the particles together by adding the particle RGB colours together
        SDL_SetTextureBlendMode(ballTexture, SDL_BLENDMODE_ADD);
        // Changing the ratio from 0-1 to 0-255
        SDL_SetTextureAlphaMod(ballTexture, alphaRatio * 255);

        // Gets each particles
        auto p = ballParticles.at(i);
        SDL_Rect ballParticle = { p->x , p->y, ball.w, ball.h };
        SDL_RenderCopy(renderer, ballTexture, NULL, &ballParticle);
    }

    #pragma endregion

    #pragma region Draw Power Up Particles

    // Power Up 
    if (powerUpCreated == true) {
        // Auto - for any data type
        // Actual / Current location of powerup 
        SDL_RenderCopy(renderer, powerUpTexture, NULL, &powerUp);

        for (int i = 0; i < powerUpParticles.size(); i++) {
            // between 0 - 1
            double alphaRatio = i * 1.0 / powerUpParticles.size();
            // Changing the ratio from 0-1 to 0-255
            // Blends the particles together by adding the particle colours together
            // I think 
            SDL_SetTextureBlendMode(powerUpTexture, SDL_BLENDMODE_ADD);
            SDL_SetTextureAlphaMod(powerUpTexture, alphaRatio * 255);

            // Gets each particles
            auto p = powerUpParticles.at(i);
            SDL_Rect powerUpParticle = { p->x , p->y, powerUp.w, powerUp.h };
            SDL_RenderCopy(renderer, powerUpTexture, NULL, &powerUpParticle);

        }
    }

    #pragma endregion

    // Player bat textures
    // Created two player textures inorder to load a flipped texture of the bat as 
    // it will look like the bat is facing at the wrong direction
    SDL_RenderCopy(renderer, player1Texture, NULL, &player1);
    SDL_RenderCopy(renderer, player2Texture, NULL, &player2);

    // Write Score onto screen
    writeScore(renderer, fontText);


}

#pragma endregion




