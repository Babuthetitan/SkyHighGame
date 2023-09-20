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
    TYPE_METEOR=3,
};

enum Agent8State
{
    STATE_APPEAR=0,
    STATE_FLY=1,
    STATE_LAUNCH=2,
};

struct GameState
{
    int score = 0;
    Agent8State agentState = STATE_APPEAR;
    bool isFlying = false; //is agent8 flying?   
};

GameState gameState;

//Width and height for agent8
const float AGENT8_WIDTH = 50.0f;
const float AGENT8_HEIGHT = 100.0f;

//Width and height for asteroids
const float ASTEROID_WIDTH = 60.0f;
const float ASTEROID_HEIGHT = 60.0f;

//Width and height for meteors
const float METEOR_WIDTH = 60.0f;
const float METEOR_HEIGHT = 60.0f;

//Agent8 move speed and mouse rotation
const float AGENT8_MOVESPEED = 200.0f; //speed of agent8
const float ROTATION_SPEED = 2.0f;

// Number of gems to create
const int NUM_GEMS = 5;
// Gem radius
const float GEM_RADIUS = 20.0f;
// Gem speed
const float GEM_SPEED = 2.0f;

//Number of Asteroids
const int NUM_ASTEROIDS = 3;
// Asteroid radius
const float ASTEROID_RADIUS = 30.0f;
// Asteroid speed
const float ASTEROID_SPEED = 1.0f;

//Number of Meteors
const int NUM_METEORS = 5;
//Meteors radius
const float METEOR_RADIUS = 30.0f;
//Meteor speed
const float METEOR_SPEED = 3.0f;

// function declaration
void GemCreation();
void AsteroidCreation();
void MeteorCreation();
void GemBehaviour();
void AsteroidBehaviour();
void Agent8FlightControls(float elapsedTime);
void CheckCollisions(float elapsedTime);
void Draw();

// The entry point for a Windows program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
    Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
    Play::CentreAllSpriteOrigins();
    Play::LoadBackground( "Data\\Backgrounds\\background1.png" );

    //agent8 creation
    Play::CreateGameObject(TYPE_AGENT8, { 85, 75 }, 50, "agent8_fly");
 
    GemCreation(); 
    AsteroidCreation();
    MeteorCreation();
}

// Called by the PlayBuffer once for each frame of the game (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
    GemBehaviour();
    AsteroidBehaviour();
    Agent8FlightControls(elapsedTime);
    CheckCollisions(elapsedTime);
    Draw();



    //End game if all gems are collected
    Play::CollectGameObjectIDsByType(TYPE_GEM);
    std::vector<int> GemIDs = Play::CollectGameObjectIDsByType(TYPE_GEM);

    if (GemIDs.empty())
    {
        Play::DrawFontText("64px", "Game Over! press esc", Point2D(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2), Play::CENTRE);
        Play::PresentDrawingBuffer();

        Sleep(200);

        if (Play::KeyDown(VK_ESCAPE))
        {
            return true; // Exit the game
        }
    }

    return false; //The game will continue running
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

    //Draw the meteors
    Play::CollectGameObjectIDsByType(TYPE_METEOR);
    std::vector<int> METEORIDs = Play::CollectGameObjectIDsByType(TYPE_METEOR);
    for (int i : METEORIDs)
    {
        Play::DrawObject(Play::GetGameObject(i));
        Play::GetGameObject(i);
    }

    //Draw agent8
    GameObject& agent8 = Play::GetGameObjectByType(TYPE_AGENT8);
    Play::DrawObjectRotated(agent8);

    //game controls and score
    int val{ 0 };
    Play::DrawFontText("64px", "Collect all the 5 Gems: " + std::to_string(gameState.score), Point2D(DISPLAY_WIDTH / 2, 40), Play::CENTRE);
    Play::DrawFontText("64px", "Use arrow keys and mouse to control Agent8 ", Point2D(DISPLAY_WIDTH / 2, 650), Play::CENTRE);

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

void MeteorCreation() 
{
    // Creating the meteors going into entry
    for (int i = 0; i < NUM_METEORS; i++)
    {
        float xPos = static_cast<float>(rand() % DISPLAY_WIDTH);
        float yPos = static_cast<float>(rand() % DISPLAY_HEIGHT);
        Play::CreateGameObject(TYPE_METEOR, { xPos, yPos }, GEM_RADIUS, "meteor_2");
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

void Agent8FlightControls(float elapsedTime)
{
    GameObject& agent8 = Play::GetGameObjectByType(TYPE_AGENT8);

    //Mouse position current
    Point2D mousePosition = Play::GetMousePos();
    float mouseX = mousePosition.x;
    float mouseY = mousePosition.y;

    //agent 8 to mouse cursor
    float dx = mouseX - agent8.pos.x;
    float dy = mouseY - agent8.pos.y;

    float angle = atan2(dy, dx);
    angle *= ROTATION_SPEED;
    agent8.rotation = angle;


    
    if (Play::KeyDown(VK_LEFT))
    {
        agent8.pos.x -= AGENT8_MOVESPEED * elapsedTime;
    }

    if (Play::KeyDown(VK_RIGHT))
    {
        agent8.pos.x += AGENT8_MOVESPEED * elapsedTime;
    }

    if (Play::KeyDown(VK_UP))
    {
        agent8.pos.y -= AGENT8_MOVESPEED * elapsedTime;
    }

    if (Play::KeyDown(VK_DOWN))
    {
        agent8.pos.y += AGENT8_MOVESPEED * elapsedTime;
    }

    //wrap around the screen
    if (agent8.pos.x < 0)
    {
        agent8.pos.x = DISPLAY_WIDTH;
    }
    else if (agent8.pos.x > DISPLAY_WIDTH)
    {
        agent8.pos.x = 0;
    }
    if (agent8.pos.y < 0)
    {
        agent8.pos.y = DISPLAY_HEIGHT;
    }
    else if (agent8.pos.y > DISPLAY_HEIGHT)
    {
        agent8.pos.y = 0;
    }
}

bool CheckAABBCollision(const GameObject& obj1, const GameObject& obj2, float obj1Width, float obj1Height,float obj2Width, float obj2Height)
{
    //half width & half height of the bounding boxes
    float obj1HalfWidth = obj1Width / 2.0f;
    float obj1HalfHeight = obj1Height / 2.0f;
    float obj2HalfWidth = obj2Width / 2.0f;
    float obj2HalfHeight = obj2Height / 2.0f;

    //centers of the bounding boxes
    float obj1CenterX = obj1.pos.x + obj1HalfWidth;
    float obj1CenterY = obj1.pos.y + obj1HalfHeight;
    float obj2CenterX = obj2.pos.x + obj2HalfWidth;
    float obj2CenterY = obj2.pos.y + obj2HalfHeight;

    //distances between the centers of the bounding boxes
    float dx = fabs(obj1CenterX - obj2CenterX);
    float dy = fabs(obj1CenterY - obj2CenterY);

    //checking for the collision
    if (dx < obj1HalfWidth + obj2HalfWidth && dy < obj1HalfHeight + obj2HalfHeight)
    {
        return true; //collided
    }

    return false; //No collision
}

void CheckCollisions(float elapsedTime)
{
    GameObject& agent8 = Play::GetGameObjectByType(TYPE_AGENT8);

    //Iteration through asteroids and check for the collisions with agent 8
    Play::CollectGameObjectIDsByType(TYPE_ASTEROID);
    std::vector<int>AsteroidIDs = Play::CollectGameObjectIDsByType(TYPE_ASTEROID);

    for (int asteroidID : AsteroidIDs)
    {
        GameObject& asteroid = Play::GetGameObject(asteroidID);

        // Check for collision 
        if (CheckAABBCollision(agent8, asteroid, AGENT8_WIDTH, AGENT8_HEIGHT, ASTEROID_WIDTH, ASTEROID_HEIGHT))
        {

            //agent 8 position on top of the asteroid + standing upright
            agent8.pos.y = asteroid.pos.y - AGENT8_HEIGHT;
            agent8.rotation = 0.0f;

            agent8.spriteId = Play::GetSpriteId("agent8_left_7");
            agent8.spriteId = Play::GetSpriteId("agent8_right_7");
        }
    }

    
    Play::CollectGameObjectIDsByType(TYPE_GEM);
    std::vector<int> GemIDs = Play::CollectGameObjectIDsByType(TYPE_GEM);

    for (int gemID : GemIDs)
    {
        GameObject& gem = Play::GetGameObject(gemID);

        // Check for collision 
        if (CheckAABBCollision(agent8, gem, AGENT8_WIDTH, AGENT8_HEIGHT, GEM_RADIUS * 2, GEM_RADIUS * 2))
        {
            // Collision occurred, increases the score and removes the gem
            gameState.score++;
            Play::DestroyGameObject(gemID);

            // You can add any other logic or effects here when Agent 8 collects a gem
        }
    }
}