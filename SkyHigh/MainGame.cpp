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
const int NUM_GEMS = 3;
// Gem radius
const float GEM_RADIUS = 20.0f;
// Gem speed
const float GEM_SPEED = 1.0f;

//Number of Asteroids
const int NUM_ASTEROIDS = 3;
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
    Play::CollectGameObjectIDsByType(TYPE_GEM);
    std::vector<int> GemIDs = Play::CollectGameObjectIDsByType(TYPE_GEM);
    for (int i: GemIDs)
    {
        Play::DrawObject(Play::GetGameObject(i));
        Play::GetGameObject(i);
    }

    // Draw the asteroids
    Play::CollectGameObjectIDsByType(TYPE_ASTEROID);
    std::vector<int> AsteroidIDs = Play::CollectGameObjectIDsByType(TYPE_ASTEROID);
    for (int i :AsteroidIDs)
    {
        Play::DrawObject(Play::GetGameObject(i));
        Play::GetGameObject(i);
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
    std::vector<int> GemIDs = Play::CollectGameObjectIDsByType(TYPE_GEM);
    for (int i : GemIDs)
    {
        GameObject& obj_gem = Play::GetGameObject(i);
        obj_gem.pos.y += GEM_SPEED;

        //checking if Gems are out of the display area and reset their position
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
    std::vector<int> AsteroidIDs = Play::CollectGameObjectIDsByType(TYPE_ASTEROID);
    for (int i : AsteroidIDs)
    {
        GameObject& obj_asteroid = Play::GetGameObject(i);

        // Moving the asteroids upward
        obj_asteroid.pos.y -= ASTEROID_SPEED;

        // Wrapping the asteroid to the opposite display area at random positions if out of bounds
        if (obj_asteroid.pos.y < -ASTEROID_RADIUS)
        {
            obj_asteroid.pos.x = static_cast<float>(rand() % DISPLAY_WIDTH);
            obj_asteroid.pos.y = DISPLAY_HEIGHT + ASTEROID_RADIUS;
        }
    }
}

