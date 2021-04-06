/* Start Header -------------------------------------------------------
    Copyright (C) 2020 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
    File Name: ball.h
    Purpose: source for Game class
    Language: C++ MinGW
    Platform: Visual Studio for Windows 10
    Project: CS529_t.spalter_final
    Author: Thomas Spalter, t.spalter
    Creation date: 12/13/2020
- End Header --------------------------------------------------------*/

#include <string>
#include <sstream>

#include "game.h"
#include "resourceManager.h"
#include "sprite.h"
#include "ball.h"
#include "particle.h"
#include "postprocessor.h"
#include "text.h"


// Game-related State data
SpriteRenderer* Renderer;

// initial player vars
const glm::vec2 PLAYER_SIZE(100.0f, 20.f);
const float PLAYER_VELOCITY(500.0f);

GameObject* player;
GameObject* enemy;

// initial ball vars
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.f);
const float BALL_RADIUS = 12.5f;

BallObject* ball;

// initial particle generator
ParticleGenerator* particles;

// initial postprocessor
PostProcessor* effects;

// initial text renderer
TextRenderer* text;

float shakeTime = 0.0f;

bool goingRight;

Game::Game(unsigned int width, unsigned int height)
    : state(GAME_MENU), keys(), keysProcessed(), width(width), height(height), level(0), lives(3)
{

}

Game::~Game()
{
    delete Renderer;
    delete player;
    delete enemy;
    delete ball;
    delete particles;
    delete effects;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("shader.vert", "shader.frag", nullptr, "sprite");
    ResourceManager::LoadShader("particle.vert", "particle.frag", nullptr, "particle");
    ResourceManager::LoadShader("postprocess.vert", "postprocess.frag", nullptr, "postprocessing");
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // load textures
    ResourceManager::LoadTexture("background.jpg", false, "background");
    ResourceManager::LoadTexture("awesomeface.png", true, "face");
    ResourceManager::LoadTexture("block.png", false, "block");
    ResourceManager::LoadTexture("block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("paddle.png", true, "paddle");
    ResourceManager::LoadTexture("particle.png", true, "particle");
    ResourceManager::LoadTexture("powerup_speed.jpg", true, "powerup_speed");
    ResourceManager::LoadTexture("powerup_sticky.jpg", true, "powerup_sticky");
    ResourceManager::LoadTexture("powerup_increase.jpg", true, "powerup_increase");
    ResourceManager::LoadTexture("powerup_confuse.jpg", true, "powerup_confuse");
    ResourceManager::LoadTexture("powerup_chaos.jpg", true, "powerup_chaos");
    ResourceManager::LoadTexture("powerup_passthrough.jpg", true, "powerup_passthrough");

    // load levels
    GameLevel one; one.Load("level1.lvl", this->width, this->height / 2);
    this->levels.push_back(one);
    GameLevel two; two.Load("level2.lvl", this->width, this->height / 2);
    this->levels.push_back(two);
    GameLevel three; three.Load("level3.lvl", this->width, this->height / 2);
    this->levels.push_back(three);
    GameLevel four; four.Load("level4.lvl", this->width, this->height / 2);
    this->levels.push_back(four);
    this->level = 0;

    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    // initialize player
    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    // initialize enemy
    glm::vec2 enemyPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height / 2.0f + 50.0f);
    enemy = new GameObject(enemyPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    enemy->color = glm::vec3(1.0f, 0.0f, 0.0f);

    // initialize ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));

    // initialize particle generator
    particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);

    // initialize postproccesor
    effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->width, this->height);

    // initialize text renderer
    text = new TextRenderer(this->width, this->height);
    text->Load("ocraext.TTF", 24);
}

void Game::Update(float dt)
{
    ball->Move(dt, this->width);

    this->DoCollisions();

    particles->Update(dt, *ball, 2, glm::vec2(ball->radius / 2.0f));

    this->UpdatePowerUps(dt);

    if (ball->position.y >= this->height) {
        --this->lives;

        if (this->lives == 0) {
            this->ResetLevel();
            this->state = GAME_MENU;
        }
        this->ResetPlayer();
    }

    // win condition
    if (this->state == GAME_ACTIVE && this->levels[this->level].isCompleted())
    {
        this->ResetLevel();
        this->ResetPlayer();
        effects->chaos = true;
        this->state = GAME_WIN;
    }

    if (shakeTime > 0.0f) {
        shakeTime -= dt;
        if (shakeTime <= 0.0f) {
            effects->shake = false;
        }
    }

    this->EnemyMovement(dt);
}

void Game::ProcessInput(float dt)
{
    if (this->state == GAME_MENU) {
        if (this->keys[GLFW_KEY_ENTER] && !this->keysProcessed[GLFW_KEY_ENTER]) {
            this->state = GAME_ACTIVE;
            this->keysProcessed[GLFW_KEY_ENTER] = true;
        }
        if (this->keys[GLFW_KEY_W] && !this->keysProcessed[GLFW_KEY_W]) {
            this->level = (this->level + 1) % 4;
            this->keysProcessed[GLFW_KEY_W] = true;
        }
        if (this->keys[GLFW_KEY_S] && !this->keysProcessed[GLFW_KEY_S]) {
            if (this->level > 0) {
                --this->level;
            } 
            else {
                this->level = 3;
            }
            this->keysProcessed[GLFW_KEY_S] = true;
        }
    }
    if (this->state == GAME_WIN) {
        if (this->keys[GLFW_KEY_ENTER]) {
            this->keysProcessed[GLFW_KEY_ENTER] = true;
            effects->chaos = false;
            this->state = GAME_MENU;
        }
    }
    if (this->state == GAME_ACTIVE) {
        float velocity = PLAYER_VELOCITY * dt;

        // player controller
        if (this->keys[GLFW_KEY_A]) {
            if (player->position.x >= 0.0f) {
                player->position.x -= velocity;
                if (ball->stuck) {
                    ball->position.x -= velocity;
                }
            }
        }
        if (this->keys[GLFW_KEY_D]) {
            if (player->position.x <= this->width - player->size.x) {
                player->position.x += velocity;
                if (ball->stuck) {
                    ball->position.x += velocity;
                }
            }
        }
        if (this->keys[GLFW_KEY_SPACE]) {
            ball->stuck = false;
        }
    }
}

void Game::Render()
{
    if (state == GAME_ACTIVE || state == GAME_MENU || state == GAME_WIN) {
        effects->BeginRender();
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        // draw level
        this->levels[this->level].Draw(*Renderer);
        // draw player
        player->Draw(*Renderer);
        // draw enemy
        enemy->Draw(*Renderer);
        // draw particles
        particles->Draw();
        // draw ball
        ball->Draw(*Renderer);
        // draw powerups
        for (PowerUp& powerUp : this->powerUps) {
            if (!powerUp.Destroyed) {
                powerUp.Draw(*Renderer);
            }
        }

        effects->EndRender();
        effects->Render(glfwGetTime());

        std::stringstream ss; ss << this->lives;
        text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
    }
    if (this->state == GAME_MENU) {
        text->RenderText("Press ENTER to start", 250.0f, this->height / 2.0f, 1.0f);
        text->RenderText("Press W or S to select level", 245.0f, this->height / 2.0f + 20.0f, 0.75f);
        text->RenderText("HOW TO PLAY", 245.0f, this->height / 2.0f + 60.0f, 0.75f);
        text->RenderText("Press A and D to move, Press Space to shoot ball", 245.0f, this->height / 2.0f + 80.0f, 0.75f);
    }
    if (this->state == GAME_WIN) {
        text->RenderText("You WON!!!", 320.0f, this->height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}

void Game::EnemyMovement(float dt) {
    if (enemy->position.x >= this->width - enemy->size.x) {
        goingRight = false;
    }
    if (enemy->position.x <= 0.0f) {
        goingRight = true;
    }
    if (goingRight) {
        enemy->position.x += PLAYER_VELOCITY * dt;
    }
    else {
        enemy->position.x -= PLAYER_VELOCITY * dt;
    }
}

bool Game::CheckCollision(GameObject& one, GameObject& two) {
    bool collisionX = one.position.x + one.size.x >= two.position.x && two.position.x + two.size.x >= one.position.x;
    bool collisionY = one.position.y + one.size.y >= two.position.y && two.position.y + two.size.y >= one.position.y;

    return collisionX && collisionY;
}

Collision Game::CheckCollision(BallObject& one, GameObject& two) {
    // circle info
    glm::vec2 center(one.position + one.radius);

    // AABB info
    glm::vec2 aabbHalfExtents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabbCenter(two.position.x + aabbHalfExtents.x, two.position.y + aabbHalfExtents.y);

    // find difference
    glm::vec2 difference = center - aabbCenter;
    glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);

    glm::vec2 closest = aabbCenter + clamped;
    difference = closest - center;
    if (glm::length(difference) <= one.radius) {
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else {
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
    }
}

void Game::DoCollisions() {
    for (GameObject& box : this->levels[this->level].bricks) {
        if (!box.Destroyed) {
            Collision collision = CheckCollision(*ball, box);
            if (std::get<0>(collision)) {
                if (!box.isSolid) {
                    box.Destroyed = true;
                    this->SpawnPowerUps(box);
                }
                else {
                    shakeTime = 0.05f;
                    effects->shake = true;
                }
                Direction dir = std::get<1>(collision);
                glm::vec2 diffVector = std::get<2>(collision);
                if (!(ball->passthrough && !box.isSolid)) {
                    if (dir == LEFT || dir == RIGHT) { // horizontal collision
                        ball->velocity.x = -ball->velocity.x;
                        float penetration = ball->radius - std::abs(diffVector.x);
                        if (dir == LEFT) {
                            ball->position.x += penetration;
                        }
                        else {
                            ball->position.x -= penetration;
                        }
                    }
                    else { // vertical collision
                        ball->velocity.y = -ball->velocity.y;
                        float penetration = ball->radius - std::abs(diffVector.y);
                        if (dir == UP) {
                            ball->position.y -= penetration;
                        }
                        else {
                            ball->position.y += penetration;
                        }
                    }
                }  
            }
        }
    }

    // player-ball collision
    Collision result = CheckCollision(*ball, *player);
    if (!ball->stuck && std::get<0>(result)) {
        // check where paddle was hit, change velocity based on where
        float centerPaddle = player->position.x + player->size.x / 2.0f;
        float distance = (ball->position.x + ball->radius) - centerPaddle;
        float percentage = distance / (player->size.x / 2.0f);

        // move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ball->velocity;
        ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->velocity.y = -1.0f * abs(ball->velocity.y);
        ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
        ball->stuck = ball->sticky;
    }

    // enemy-ball collision
    Collision enemyResult = CheckCollision(*ball, *enemy);
    if (!ball->stuck && std::get<0>(enemyResult)) {
        // check where paddle was hit, change velocity based on where
        float centerPaddle = enemy->position.x + enemy->size.x / 2.0f;
        float distance = (ball->position.x + ball->radius) - centerPaddle;
        float percentage = distance / (enemy->size.x / 2.0f);

        // move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ball->velocity;
        ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->velocity.y = abs(ball->velocity.y);
        ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
    }

    // player-powerup collision
    for (PowerUp& powerUp : this->powerUps) {
        if (!powerUp.Destroyed) {
            if (powerUp.position.y >= this->height) {
                powerUp.Destroyed = true;
            }
            if (CheckCollision(*player, powerUp)) {
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.activated = true;
            }
        }
    }
}

Direction Game::VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };
    float max = 0.0f;
    unsigned int bestMatch = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dotProduct = glm::dot(glm::normalize(target), compass[i]);
        if (dotProduct > max) {
            max = dotProduct;
            bestMatch = i;
        }
    }
    return (Direction)bestMatch;
}

void Game::ResetLevel() {
    effects->chaos = false;
    effects->confuse = false;
    if (this->level == 0) {
        this->levels[0].Load("level1.lvl", this->width, this->height / 2);
    }
    else if (this->level == 1) {
        this->levels[1].Load("level2.lvl", this->width, this->height / 2);
    }
    else if (this->level == 2) {
        this->levels[2].Load("level3.lvl", this->width, this->height / 2);
    }
    else if (this->level == 3) {
        this->levels[3].Load("level4.lvl", this->width, this->height / 2);
    }
    this->lives = 3;
}

void Game::ResetPlayer() {
    // reset player and ball
    player->size = PLAYER_SIZE;
    player->position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    ball->Reset(player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

bool Game::ShouldSpawn(unsigned int chance) {
    unsigned int random = rand() % chance;
    return random == 0;
}

void Game::SpawnPowerUps(GameObject& block) {
    if (ShouldSpawn(75)) { // chance: 1/75
        this->powerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position, ResourceManager::GetTexture("powerup_speed")));
    }
    if (ShouldSpawn(75)) { 
        this->powerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.position, ResourceManager::GetTexture("powerup_sticky")));
    }
    if (ShouldSpawn(75)) { 
        this->powerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position, ResourceManager::GetTexture("powerup_passthrough")));
    }
    if (ShouldSpawn(75)) { 
        this->powerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.position, ResourceManager::GetTexture("powerup_increase")));
    }
    if (ShouldSpawn(15)) { 
        this->powerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position, ResourceManager::GetTexture("powerup_confuse")));
    }
    if (ShouldSpawn(15)) {
        this->powerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position, ResourceManager::GetTexture("powerup_chaos")));
    }
}

void Game::ActivatePowerUp(PowerUp& powerUp) {
    if (powerUp.type == "speed") {
        ball->velocity *= 1.2;
    }
    else if (powerUp.type == "sticky") {
        ball->sticky = true;
        player->color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.type == "pass-through") {
        ball->passthrough = true;
        ball->color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.type == "pad-size-increase") {
        player->size.x += 50;
    }
    else if (powerUp.type == "confuse") {
        if (!effects->chaos) {
            effects->confuse = true;
        }
    }
    else if (powerUp.type == "chaos") {
        if (!effects->confuse) {
            effects->chaos = true;
        }
    }
}

void Game::UpdatePowerUps(float dt) {
    for (PowerUp& powerUp : this->powerUps) {
        powerUp.position += powerUp.velocity * dt;
        if (powerUp.activated) {
            powerUp.duration -= dt;
            if (powerUp.duration <= 0.0f) {
                // remove powerup from list
                powerUp.activated = false;

                // deactivate
                if (powerUp.type == "sticky") {
                    if (!IsOtherPowerUpActive(this->powerUps, "sticky")) {
                        ball->sticky = false;
                        player->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.type == "pass-through") {
                    if (!IsOtherPowerUpActive(this->powerUps, "pass-through")) {
                        ball->passthrough = false;
                        ball->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.type == "confuse") {
                    if (!IsOtherPowerUpActive(this->powerUps, "confuse")) {
                        effects->confuse = false;
                    }
                }
                else if (powerUp.type == "chaos") {
                    if (!IsOtherPowerUpActive(this->powerUps, "chaos")) {
                        effects->chaos = false;
                    }
                }
            }
        }
    }
    // erase all inactives
    this->powerUps.erase(std::remove_if(this->powerUps.begin(), this->powerUps.end(),
        [](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.activated; }
    ), this->powerUps.end());
}

bool Game::IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type) {
    for (const PowerUp& powerUp : powerUps) {
        if (powerUp.activated) {
            if (powerUp.type == type) {
                return true;
            }
        }
    }
    return false;
}