#include "core/Application.h"
#include <iostream>
#include "core/Camera.h"
#include <vector>

using namespace std;

struct Particule
{
    vec4 position;
    vec4 velocity;
};

struct Voxel
{
    vec4 velocity;
    vec4 staggeredVelocity;
    vec4 newStaggeredVelocity;
};

class Simulator: public Application
{
private:
    Program* compute = NULL;
    Program* compute_empty = NULL;
    Program* compute_grid = NULL;
    Program* compute_staggeredGrid = NULL;
    Program* compute_gravity = NULL;
    Program* compute_boundary = NULL;
    Program* compute_updateVelocity = NULL;
    Program* compute_position = NULL;
    Program* program = NULL;
    Renderer* renderer = NULL;
    Computer* computer = NULL;
    Computer* computer_empty = NULL;
    Computer* computer_grid = NULL;
    Computer* computer_staggeredGrid = NULL;
    Computer* computer_gravity = NULL;
    Computer* computer_boundary = NULL;
    Computer* computer_updateVelocity = NULL;
    Computer* computer_position = NULL;
    Buffer* particules = NULL;
    Buffer* voxels = NULL;
    Camera* camera;
    mat4 modelview;
    mat4 projection;
    
public:
    Simulator(int argc, char* argv[]);
    void update(int time);
    void render();
    void setup();
    void teardown();
};

Simulator::Simulator(int argc, char* argv[]) : Application(argc, argv)
{   
    camera = new Camera(vec3(0, 5, 15), vec3(0, 3, 0), vec3(0, 1, 0));
    modelview = lookat(vec3(0, 5, 15), vec3(0, 3, 0), vec3(0, 1, 0));
    projection =   perspective(70.0, 640.0 / 480.0, 1.0, 100.0);

}

void Simulator::update(int elapsedTime)
{
	computer_empty->compute(1, 30, 1);

    
	computer_grid->compute(8, 1, 1);
	computer_staggeredGrid->compute(1, 30, 1);
	computer_gravity->compute(1, 30, 1);
    computer_boundary->compute(1, 30, 1);
    computer_updateVelocity->compute(1, 30, 1);
    computer_position->compute(8, 1, 1);
    //computer->compute(10, 1, 1);
    camera->deplacer(input);
}

void Simulator::render()
{
    camera->lookAt(modelview);
    renderer->setMatrix("projectionMatrix", projection);
    renderer->setMatrix("modelViewMatrix", modelview);
    renderer->render(PRIMITIVE_POINTS, 8000);
}

void Simulator::setup()
{    
    setClearColor(0.95f, 0.95f, 0.95f, 1.0f); 

    float x = 0;
    float z = 0;
    float y = 7;
    float pas = 0.2;

    Particule pp[8000];
    Voxel vo[27000];

    for(int i(0); i < 8000; i++)
    {
        vec3 position = vec3(x, y, z);
        if(i % 20 == 0 )
        {
            z += pas;
            x = 0;
        }
        if(i % 400 == 0)
        {
            x = 0;
            z = 0;
            y -= pas;
        }

        pp[i].position = vec4(x, y, z, 1.0f);
        pp[i].velocity = vec4(0.0);

        x += pas;

        std::cout << i << std::endl;
    }

    for(int j(0); j < 27000; j++)
    {
    	vo[j].velocity = vec4(0.0);
    	vo[j].staggeredVelocity = vec4(0.0);
    	vo[j].newStaggeredVelocity = vec4(0.0);
    }



    particules = new Buffer(pp, sizeof(pp));  
    voxels = new Buffer(vo, sizeof(vo));

    program = new Program();
    program->addShader(Shader::fromFile("shaders/perspective.vert"));
    program->addShader(Shader::fromFile("shaders/black.frag"));
    program->link();

    renderer = program->createRenderer();

    renderer->setVertexData("vertex", particules, TYPE_FLOAT, 0, 3, sizeof(Particule));

    input.displayPointer(false);
    input.capturePointer(true);

    renderer->setMatrix("projectionMatrix", projection);
    renderer->setMatrix("modelViewMatrix", modelview);

    compute_empty = new Program();
    compute_empty->addShader(Shader::fromFile("shaders/emptyGrid.comp"));
    compute_empty->link();
    computer_empty = compute_empty->createComputer();
    computer_empty->setData(2, voxels);

    compute_grid = new Program();
    compute_grid->addShader(Shader::fromFile("shaders/grid.comp"));
    compute_grid->link();
    computer_grid = compute_grid->createComputer();
    computer_grid->setData(1, particules);
    computer_grid->setData(2, voxels);

    compute_staggeredGrid = new Program();
    compute_staggeredGrid->addShader(Shader::fromFile("shaders/staggeredGrid.comp"));
    compute_staggeredGrid->link();
    computer_staggeredGrid = compute_staggeredGrid->createComputer();
    computer_staggeredGrid->setData(2, voxels);

    compute_gravity = new Program();
    compute_gravity->addShader(Shader::fromFile("shaders/gravity.comp"));
    compute_gravity->link();
    computer_gravity = compute_gravity->createComputer();
    computer_gravity->setData(2, voxels);

    compute_boundary = new Program();
    compute_boundary->addShader(Shader::fromFile("shaders/boundary.comp"));
    compute_boundary->link();
    computer_boundary = compute_gravity->createComputer();
    computer_boundary->setData(2, voxels);

    compute_updateVelocity = new Program();
    compute_updateVelocity->addShader(Shader::fromFile("shaders/updateVelocity.comp"));
    compute_updateVelocity->link();
    computer_updateVelocity = compute_gravity->createComputer();
    computer_updateVelocity->setData(2, voxels);

    compute_position = new Program();
    compute_position->addShader(Shader::fromFile("shaders/position.comp"));
    compute_position->link();
    computer_position = compute_position->createComputer();
    computer_position->setData(1, particules);
    computer_position->setData(2, voxels);
}

void Simulator::teardown()
{
   
}

int main(int argc, char** argv)
{
    try 
    {
        Simulator app = Simulator(argc, argv); 
	    return app.run();
    }
	catch(Exception e)
    {
        cout << e.getMessage() << endl;
        return 1;
    }
}