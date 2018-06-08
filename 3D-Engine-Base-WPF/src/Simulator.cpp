#include "core/Application.h"
#include <iostream>
#include "core/Camera.h"
#include <vector>
#include  <cstdlib>

using namespace std;

struct Particule
{
    vec4 position;
    vec4 velocity;
};

class Simulator: public Application
{
private:
    Program* compute = NULL;
    Program* compute_gravity = NULL;
    Program* compute_boundary = NULL;
    Program* compute_position = NULL;
    Program* program = NULL;
    Renderer* renderer = NULL;
    Computer* computer = NULL;
    Computer* computer_gravity = NULL;
    Computer* computer_boundary = NULL;
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
    computer_gravity->compute(10, 1, 1);
    computer_boundary->compute(10, 1, 1);
    computer_position->compute(10, 1, 1);
    //computer->compute(10, 1, 1);
    camera->deplacer(input);
}

void Simulator::render()
{
    camera->lookAt(modelview);
    renderer->setMatrix("projectionMatrix", projection);
    renderer->setMatrix("modelViewMatrix", modelview);
    renderer->setMatrix("lightPosition", vec3(20.0, 20.0, 20.0));
    renderer->render(PRIMITIVE_POINTS, 10000);
}

void Simulator::setup()
{    
    setClearColor(0.95f, 0.95f, 0.95f, 1.0f); 

    float x = 0;
    float z = 0;
    float y = 7;
    float pas = 0.2;

    Particule pp[10000];

    for(int i(0); i < 10000; i++)
    {
        float x = (rand() % 20) - 10; 
        float y = (rand() % 15); 
        float z = (rand() % 20) - 10; 

        float vx = (rand() % 20) - 10; 
        float vy = (rand() % 20) - 10; 
        float vz = (rand() % 20) - 10; 

        pp[i].position = vec4(x, y, z, 1.0f);
        pp[i].velocity = vec4(vx, vy, vz, 1.0);
    }


    particules = new Buffer(pp, sizeof(pp));

    program = new Program();
    program->addShader(Shader::fromFile("shaders/perspective.vert"));
    program->addShader(Shader::fromFile("shaders/black.frag"));
    program->link();

    renderer = program->createRenderer();

    renderer->setVertexData("vertex", particules, TYPE_FLOAT, 0, 3, sizeof(Particule));

    input.displayPointer(true);
    input.capturePointer(false);

    renderer->setMatrix("projectionMatrix", projection);
    renderer->setMatrix("modelViewMatrix", modelview);
    
    compute_gravity = new Program();
    compute_gravity->addShader(Shader::fromFile("shaders/gravity.comp"));
    compute_gravity->link();
    computer_gravity = compute_gravity->createComputer();
    
    computer_gravity->setData(1, particules);

    compute_boundary = new Program();
    compute_boundary->addShader(Shader::fromFile("shaders/boundary.comp"));
    compute_boundary->link();
    computer_boundary = compute_boundary->createComputer();
    computer_boundary->setData(1, particules);

    compute_position = new Program();
    compute_position->addShader(Shader::fromFile("shaders/position.comp"));
    compute_position->link();
    computer_position = compute_position->createComputer();
    computer_position->setData(1, particules);
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