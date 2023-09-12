#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

const int DISPLAY_WIDTH = 1280;
const int DISPLAY_HEIGHT = 720;
const int DISPLAY_SCALE = 1;

// enum for game object types
enum GameObjectType
{
    TYPE_GEM = 0,
    TYPE_ASTEROID = 1,
    TYPE_AGENT8= 2,
};

// Number of gems to create
const int NUM_GEMS = 6;
// Gem radius
const float GEM_RADIUS = 20.0f;
// Gem speed
const float GEM_SPEED = 1.0f;

//Number of Asteroids
const int NUM_ASTEROIDS = 2;
// Asteroid radius
const float ASTEROID_RADIUS = 30.0f;
// Asteroid speed
const float ASTEROID_SPEED = 1.0f;

// function declaration
void GemCreation();
void AsteroidCreation();
void GemBehaviour();
void AsteroidBehaviour();
void Draw();

// The entry point for a Windows program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
    Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
    Play::CentreAllSpriteOrigins();
    Play::LoadBackground( "Data\\Backgrounds\\background1.png" );
    GemCreation();
    AsteroidCreation();
}

// Called by the PlayBuffer once for each frame of the game (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
    GemBehaviour();
    AsteroidBehaviour();

    Draw();

    return Play::KeyDown( VK_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
    Play::DestroyManager();
    return PLAY_OK;
}

void Draw()
{
    
    Play::DrawBackground();
    
    // Draw the Gems
    for (int i = 0; i < NUM_GEMS; i++)
    {
        Play::DrawObject(Play::GetGameObjectByType(TYPE_GEM));
    }

    // Draw the asteroids
    for (int i = 0; i < NUM_ASTEROIDS; i++)
    {
        Play::DrawObject(Play::GetGameObjectByType(TYPE_ASTEROID));
    }

    Play::PresentDrawingBuffer();

};

void GemCreation()
{
    // Creating the gems going into entry
    for (int i = 0; i < NUM_GEMS; i++)
    {
        float xPos = static_cast<float>(rand() % DISPLAY_WIDTH);
        float yPos = static_cast<float>(rand() % DISPLAY_HEIGHT);
        Play::CreateGameObject(TYPE_GEM, { xPos, yPos }, GEM_RADIUS, "gem");
    }
}

void AsteroidCreation()
{
    //Creating the asteroids going into entry
    for (int i = 0; i < NUM_ASTEROIDS; i++)
    {
        float xPos = static_cast<float>(rand() % DISPLAY_WIDTH);
        float yPos = static_cast<float>(rand() % DISPLAY_HEIGHT);
        Play::CreateGameObject(TYPE_ASTEROID, { xPos,yPos }, ASTEROID_RADIUS, "asteroid_2");
    }
}

void GemBehaviour()
{
    // Gem Behaviour going into update
    for (int i = 0; i < NUM_GEMS; i++)
    {
        GameObject& obj_gem = Play::GetGameObjectByType(TYPE_GEM);
        obj_gem.pos.y += GEM_SPEED;

        //checking if Gem is out of the display area and reset its position
        if (obj_gem.pos.y > DISPLAY_HEIGHT + GEM_RADIUS)
        {
            obj_gem.pos.y = -GEM_RADIUS;
            obj_gem.pos.x = static_cast<float>(rand() % DISPLAY_WIDTH);
        }
    }
}

void AsteroidBehaviour()
{
    //Asteroid Behaviour going into update
    for (int i = 0; i < NUM_ASTEROIDS; i++)
    {
        GameObject& obj_asteroid = Play::GetGameObjectByType(TYPE_ASTEROID);

        // Move the asteroid in its current direction
        obj_asteroid.pos.x += ASTEROID_SPEED * cos(obj_asteroid.rotation);
        obj_asteroid.pos.y += ASTEROID_SPEED * sin(obj_asteroid.rotation);

        // Wrapping the asteroid to the opposite display area if out of bounds
        if (obj_asteroid.pos.x < -ASTEROID_RADIUS)
        {
            obj_asteroid.pos.x = DISPLAY_WIDTH + ASTEROID_RADIUS;
        }
        else if (obj_asteroid.pos.x > DISPLAY_WIDTH + ASTEROID_RADIUS)
        {
            obj_asteroid.pos.x = -ASTEROID_RADIUS;
        }

        if (obj_asteroid.pos.y < -ASTEROID_RADIUS)
        {
            obj_asteroid.pos.y = DISPLAY_HEIGHT + ASTEROID_RADIUS;
        }
        else if (obj_asteroid.pos.y > DISPLAY_HEIGHT + ASTEROID_RADIUS)
        {
            obj_asteroid.pos.y = -ASTEROID_RADIUS;
        }
    }
}