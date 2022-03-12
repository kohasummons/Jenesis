

#pragma once



//  Mathematics library
#include <glm/glm.hpp>


//  Engine Headers
#include <jenesis/Camera.h>
#include <jenesis/geometry/Mesh.h>
#include <jenesis/geometry/Shapes.h>
#include <jenesis/geometry/PlaneGround.h>
#include <jenesis/geometry/Sphere.h>
#include <jenesis/geometry/Dome.h>
#include <jenesis/Skybox.h>
#include <jenesis/ModelMesh.h>

#include <jenesis/Light.h>




// Represents the current state of the game
enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class MyGame 
{
    public:

        float ambience = 0.5f;

        const char* monkeyPath = "res/monkey3.obj";

        const char* red_web    = "res/textures/purple.png";
        const char* bricks     = "res/textures/bricks.jpg";
        const char* grass      = "res/textures/metal.jpg";
        const char* rocks      = "res/textures/rock.png";

        glm::mat4 view;
        glm::mat4 projection;


        // constructor/destructor
        MyGame(unsigned int width, unsigned int height);
        ~MyGame();

        // initialize game state (load all shaders/textures/levels)
        void Init();
        
        // game loop
        void ProcessInput(float dt, int xpos, int ypos);
        void Update(float dt);
        void Render();

        void InitPhysics();





        // game state
        GameState               State;

        bool                    Keys[1024];
        unsigned int            Width, Height;


        // Images that Make up the Skybox(Order is very important!)
        vector<std::string> faces
        {
            // X-AXIS
            "res/sky/right.jpg",
            "res/sky/left.jpg",

            // Y-AXIS
            "res/sky/top.jpg",
            "res/sky/bottom.jpg",

            // Z-AXIS
            "res/sky/front.jpg",
            "res/sky/back.jpg"
        };

        Skybox skybox;

        Light *light;

        PlaneGround floor;

        // camera
        Camera camera;
        

        Mesh myCube;
        Mesh cub;

        Sphere lamp;
        Dome dome;

        ModelMesh* monkey;
};
