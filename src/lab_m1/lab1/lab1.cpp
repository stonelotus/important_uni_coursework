#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

struct {
    float red = 0;
    float blue = 0;
    float green = 0;
    float alpha = 1;
}clear_color;
struct {
    float x=-1, y=1, z=-1;
}custom_object_position;
string current_shown_objects[4] = { "box", "quad", "teapot", "sphere"};
int current_shown_object_id;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
 
    current_shown_object_id = 0;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    Mesh* little_bunny = new Mesh("bunny");
    little_bunny->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
    meshes[little_bunny->GetMeshID()] = little_bunny;
    
    Mesh* teapot = new Mesh("teapot");
    teapot->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
    meshes[teapot->GetMeshID()] = teapot;

    Mesh* quad = new Mesh("quad");
    quad->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
    meshes[quad->GetMeshID()] = quad;
    
    Mesh* sphere = new Mesh("sphere");
    sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[sphere->GetMeshID()] = sphere;
    
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(clear_color.red, clear_color.green, clear_color.blue , clear_color.alpha);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 1), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes[current_shown_objects[current_shown_object_id]], glm::vec3(-1, 0.5f, 0));


    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    RenderMesh(meshes["bunny"], glm::vec3(custom_object_position.x, custom_object_position.y, custom_object_position.z), glm::vec3(0.04f));

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

    if (window->KeyHold(GLFW_KEY_W)) {
        custom_object_position.z+= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        custom_object_position.z -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        custom_object_position.x += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        custom_object_position.x -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        custom_object_position.y += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        custom_object_position.y -= deltaTime;
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        clear_color.blue = 0.4;

    }
    if (key == GLFW_KEY_N) {
        clear_color.blue = 0;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    if (key == GLFW_KEY_C) {
        current_shown_object_id++;
        if (current_shown_object_id > size(current_shown_objects)-1) {
            current_shown_object_id = 0;
        }
    }

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
