

#include "MyGame.h"




MyGame::MyGame(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{}


void MyGame::Init()
{
    // Initialise Camera
    camera = Camera(glm::vec3(0.0f, 1.0f, 20.0f));

    // Init Projection Matrix
    projection = glm::perspective(glm::radians(45.0f), (float)MyGame::Width / (float)MyGame::Height, 0.1f, 100.0f);

    light = new Light(glm::vec3(0.0f, 13.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f));



    // Initialise skybox
    skybox = Skybox(view, projection, faces);

    floor = PlaneGround(view, projection, grass);
    floor.setMatrices(projection);
    floor.translate(glm::vec3(0.0f, 0.0f, -5.0f));
    // floor.rotate(0.76, glm::vec3(1.0f, 0.0f, 0.0f));
    floor.scale(100, 100, 100);


    // ObjMesh - Mr. Monkey Head ;)
    monkey = new Model(monkeyPath, red_web);
    monkey->setMatrices(projection);
    monkey->translate(glm::vec3(0.0f, 8.0f, -5.0f));
    monkey->scale(2.0f, 2.0f, 2.0f);
    

    myCube = Mesh(Shapes::cubeVertices, Shapes::cubeVertSize, glm::vec4(0.0f, 1.0f, 1.0f, 0.5f));
    myCube.setMatrices(projection);
    myCube.translate(glm::vec3(-3.0f, 3.0f, 0.0f));
    myCube.scale(1.5f, 1.5f, 1.5f);


    lamp = Sphere(view, projection);
    lamp.setMatrices(projection);
    lamp.setColor(1.0f, 1.0f, 1.0f, 1.0f);
    lamp.translate(glm::vec3(0.0f, 13.0f, -5.0f));

    dome = Dome(view, projection);
    dome.setMatrices(projection);
    dome.setColor(0.5f, 0.5f, 0.5f, 0.5f);
    dome.scale(3.0f, 3.0f, 3.0f);
}


void MyGame::Update(float dt)
{
    // Set View Matrix
    view = camera.GetViewMatrix();

    // Rotate cube
    myCube.rotate(0.02, glm::vec3(0.0f, 0.4f, 0.0));

    // Rotate monkey head
    monkey->rotate(0.02, glm::vec3(0.0f, 0.4f, 0.0f));

    dome.rotate(0.02, glm::vec3(0.0f, 0.4f, 0.0f));


    cout << 1000/dt << "\n";
}

void MyGame::Render()
{
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    skybox.Draw(camera);

    lamp.Draw(camera);

    floor.Draw(camera, light, ambience);

    myCube.Draw(camera, light, ambience);

    monkey->Draw(camera, light, ambience);

    dome.Draw(camera, light, ambience);
}

void MyGame::ProcessInput(float dt, int xpos, int ypos)
    {}

MyGame::~MyGame()
{
    delete monkey;
    delete light;
}









void MyGame::InitPhysics()
    {}