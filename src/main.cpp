
//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#define SDL_MAIN_HANDLED
#include <SDL2\SDL.h>

#include <gl\glew.h>
#include <SDL2\SDL_opengl.h>
#include <gl\glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <jenesis/input.h>

#include "MyGame.h"

void processKeyboardInput();
void SDLSetMousePosition(int x, int y);
void processMouseMovement();

//Screen dimension constants
unsigned int SCREEN_WIDTH = 640;
unsigned int SCREEN_HEIGHT = 480;

MyGame game(SCREEN_WIDTH, SCREEN_HEIGHT);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

float dt, t1, t2 = 0;

//OpenGL context
SDL_GLContext gContext;

// Main loop flag
bool quit = false;


bool mouseLocked = false;


int main(int argc, char* args[])
{
    // Initialize SDL
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER );

    SDL_GameController *controller = nullptr;

    for(int i=0 ; i<SDL_NumJoysticks() ; i++)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
        }
    }

    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Create window
    gWindow = SDL_CreateWindow
    (
        "Rotating Cube",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    //Create context
    gContext = SDL_GL_CreateContext( gWindow );

    //Initialize GLEW
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    
    //Use Vsync
    SDL_GL_SetSwapInterval( 1 );

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    game.Init();

    //Event handler
    // SDL_Event event;

    //While application is running
    while( !quit )
    {
        t1 = SDL_GetTicks();
        dt = t1 - t2;
        t2 = t1;

        game.Update(dt);
        game.Render();

        processKeyboardInput();
        processMouseMovement();

    
        Input::Update();
        
        //Update screen
        SDL_GL_SwapWindow( gWindow );
    }

    //Disable text input
    SDL_StopTextInput();

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    if(controller != NULL)
    {
        SDL_GameControllerClose(controller);
    }

    controller = nullptr;

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

// process all input: query SDL whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processKeyboardInput()
{
    //User requests quit
    if(Input::GetKey(Input::KEY_Q))
        quit = true;

    // Free Movement
    if(Input::GetKey(Input::KEY_W))
        game.camera.ProcessKeyboard(FORWARD, dt/100);
    if(Input::GetKey(Input::KEY_S))
        game.camera.ProcessKeyboard(BACKWARD, dt/100);
    if(Input::GetKey(Input::KEY_A))
        game.camera.ProcessKeyboard(LEFT, dt/100);
    if(Input::GetKey(Input::KEY_D))
        game.camera.ProcessKeyboard(RIGHT, dt/100);



    // if(Input::GetKey(Input::KEY_UP))
    //     game.camera.Pitch += 1.0f;
    // if(Input::GetKey(Input::KEY_DOWN))
    //     game.camera.Pitch -= 1.0f;
    // if(Input::GetKey(Input::KEY_LEFT))
    //     game.camera.Yaw -= 1.0f;
    // if(Input::GetKey(Input::KEY_RIGHT))
    //     game.camera.Yaw += 1.0f;
}

void processMouseMovement()
{
    // Mouse Movement
    if(Input::GetKey(Input::KEY_ESCAPE))
    {
        Input::SetCursor(true);
        mouseLocked = false;
    }

    if(mouseLocked)
    {
        glm::vec2 centerPosition = glm::vec2((float)(SCREEN_WIDTH/2.0f), (float)(SCREEN_HEIGHT/2.0f));
        glm::vec2 deltaPos = Input::GetMousePosition() - centerPosition;
        
        bool rotY = deltaPos.x != 0;
        bool rotX = deltaPos.y != 0;

        game.camera.ProcessMouseMovement(deltaPos.x * 0.5f, -deltaPos.y * 0.5f);

        // std::cout << "|" << Input::GetMousePosition().x << " , " << Input::GetMousePosition().y << "|";

        if(rotY || rotX)
            SDLSetMousePosition((int)centerPosition.x, (int)centerPosition.y);
    }

    if(Input::GetMouseDown(Input::LEFT_MOUSE))
    {
        glm::vec2 centerPosition = glm::vec2((float)SCREEN_WIDTH/2.0f, (float)SCREEN_HEIGHT/2.0f);
        Input::SetCursor(false);
        SDLSetMousePosition((int)centerPosition.x, (int)centerPosition.y);

        mouseLocked = true;
    }
}


void SDLSetMousePosition(int x, int y)
{
    SDL_WarpMouseInWindow(gWindow, x, y);
}