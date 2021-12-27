#include "lab_m1/tema2/SurvivalMaze.h"


#include "lab_m1/tema2/transform3D.h"
#include "lab_m1/tema2/helpers/maze.cpp"


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


SurvivalMaze::SurvivalMaze()
{
}
//////////////// MAZE_FUNCTIONS //////////////////////////////////////
int di[] = { 0,1,0,-1 };
int dj[] = { 1,0,-1,0 };
int di_next[] = { 0,2,0,-2 };
int dj_next[] = { 2,0,-2,0 };


bool isValidWall(int x, int y, int coming_x, int coming_y, vector<vector<int>> playground) {
    for (int i = 0; i < 4; i++) {
        if (x + di[i] != coming_x && 
            y + dj[i] != coming_y && 
            x+di[i] < playground.size() && 
            y+dj[i] < playground[i].size()) {
            if (playground[x + di[i]][y + dj[i]] == 0) return false;
        }
    }
    return true;
}

bool isValidCell(int x, int y, vector<vector<int>> playground) {
    if ( x >= 0 && x < playground.size() && y >= 0 && y < playground[x].size() && playground[x][y] == 1) return true;
    else return false;
}

void dfs(int x, int y, vector<vector<int>> &playground) {
    playground[x][y] = 0;

    vector<int> wayV;
    wayV.push_back(0);
    wayV.push_back(1);
    wayV.push_back(2);
    wayV.push_back(3);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(wayV.begin(), wayV.end(), default_random_engine(seed));

    for (int i = 0; i < wayV.size(); i++) {
        cout << wayV[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < wayV.size(); i++) {
        int way = wayV[i];


        if (isValidCell(x + di_next[way], y + dj_next[way], playground) && isValidWall(x + di[way], y + dj[way], x, y, playground)) {
            dfs(x + di[way], y + dj[way], playground);
        }
    }


}
void CreateRandomMaze(int n, int m, vector<vector<int>> &maze) {
    maze = vector<vector<int>>(n, vector<int>(m, 1));
    //dfs(rand() % 6 + 1, rand() % 6 + 1, maze);
    dfs(3, 3, maze);
}
///////////////////////////////


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

    // playground maze creation

    vector<vector<int>> playground;
    CreateRandomMaze(8, 8, playground);
    cout << "NICE " << endl;
    for (int i = 0; i < playground.size(); i++) {
        for (int j = 0; j < playground[0].size(); j++) {
            cout << playground[i][j] << " ";
        }
        cout << endl;
    }
    
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
