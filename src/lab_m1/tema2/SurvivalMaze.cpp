#include "lab_m1/tema2/SurvivalMaze.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema2/transform3D.h"

using namespace std;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


SurvivalMaze::SurvivalMaze()
{
}


SurvivalMaze::~SurvivalMaze()
{
}
void SurvivalMaze::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    Mesh* mesh2 = new Mesh("quad");
    mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
    meshes[mesh2->GetMeshID()] = mesh2;
    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    player = Player(0, 0, 0);
    //box = Box(10.f);

    
}


void SurvivalMaze::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void SurvivalMaze::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    

    {   //PLAYER RENDER
        RenderMesh(meshes["box"], shaders["VertexNormal"], player.body.getModelMatrix());   // body
        RenderMesh(meshes["box"], shaders["VertexNormal"], player.head.getModelMatrix());   // head
        RenderMesh(meshes["box"], shaders["VertexNormal"], player.left_leg.getModelMatrix());    // left_leg
        RenderMesh(meshes["box"], shaders["VertexNormal"], player.right_leg.getModelMatrix());   // right_leg
        RenderMesh(meshes["box"], shaders["VertexNormal"], player.left_arm.getModelMatrix());    // left_arm
        RenderMesh(meshes["box"], shaders["VertexNormal"], player.right_arm.getModelMatrix());   // right_arm
        RenderMesh(meshes["box"], shaders["Simple"], player.left_palm.getModelMatrix());   // right_arm
        RenderMesh(meshes["box"], shaders["Simple"], player.right_palm.getModelMatrix());   // right_arm
    }

}


void SurvivalMaze::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void SurvivalMaze::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            carTranslateX += deltaTime;
            carAngularStepOZ -= deltaTime * 2;
        }
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            carTranslateX -= deltaTime;
            carAngularStepOZ += deltaTime * 2;

        }
        if (window->KeyHold(GLFW_KEY_UP)) {
            carTranslateZ -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
            carTranslateZ += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            player.Move(0, 0, -deltaTime);
            translateZ += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_W)) {
            translateZ -= deltaTime;
            player.Move(0, 0, deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            player.Move(+deltaTime, 0, 0);

            translateX -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            player.Move(-deltaTime, 0, 0);

            translateX += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_R)) {
            translateY += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_F)) {
            translateY -= deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += deltaTime;
        scaleY += deltaTime;
        scaleZ += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= deltaTime;
        scaleY -= deltaTime;
        scaleZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= deltaTime;
    }

}


void SurvivalMaze::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void SurvivalMaze::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void SurvivalMaze::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void SurvivalMaze::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void SurvivalMaze::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void SurvivalMaze::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void SurvivalMaze::OnWindowResize(int width, int height)
{
}
