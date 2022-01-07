#include "lab_m1/tema2/SurvivalMaze.h"
#include "lab_m1/tema2/helpers/dataStructures.h"


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


//bool intersect(, box) {
//    // get box closest point to sphere center by clamping
//    float x = max(box.minX, min(sphere.x, box.maxX));
//    float y = max(box.minY, min(sphere.y, box.maxY));
//    float z = max(box.minZ, min(sphere.z, box.maxZ));
//
//    // this is the same as isPointInsideSphere
//    float distance = sqrt((x - sphere.x) * (x - sphere.x) +
//        (y - sphere.y) * (y - sphere.y) +
//        (z - sphere.z) * (z - sphere.z));
//
//    return distance < sphere.radius;
//}

bool CheckSpheresCollision(custom_structs::sphere sphere1, custom_structs::sphere sphere2) {
    float distance = sqrt((sphere1.x - sphere2.x) * (sphere1.x - sphere2.x) +
        (sphere1.y - sphere2.y) * (sphere1.y - sphere2.y) +
        (sphere1.z - sphere2.z) * (sphere1.z - sphere2.z));
    return distance < (sphere1.radius + sphere2.radius);
}


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
        int way = wayV[i];

        if (isValidCell(x + di_next[way], y + dj_next[way], playground) && isValidWall(x + di[way], y + dj[way], x, y, playground)) {
            dfs(x + di[way], y + dj[way], playground);
        }
    }


}
void CreateRandomMaze(int n, int m, vector<vector<int>> &maze) {
    maze = vector<vector<int>>(n, vector<int>(m,    1));
    //dfs(rand() % 6 + 1, rand() % 6 + 1, maze);
    dfs(3, 3, maze);    //TODO make random starting algorithm point
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

    
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    player = Player(0, 0, 0);
    //box = Box(10.f);
    {
        test_sphere = Sphere({ 1.f,1.f,1.f }, 6.f);
        test_box = Box({ 0.f,0.f,0.f }, { 1.f,1.f,1.f });
        test_enemy = Enemy({ -3.f,1.f,-3.f }, { 1.f,1.f,1.f });
    }

    // playground maze creation
    {
        vector<vector<int>> playground_matrix;
        CreateRandomMaze(20, 20, playground_matrix);
        
       /* cout << "Nice matrix\n";
        for (int i = 0; i < playground_matrix.size(); i++) {
            for (int j = 0; j < playground_matrix[i].size(); j++) {
                cout << playground_matrix[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;*/
        vector <dimensionsTriplet> possible_enemy_locations;
        for (int i = 0; i < playground_matrix.size(); i++) {
            for (int j = 0; j < playground_matrix[i].size(); j++) {
                if (playground_matrix[i][j] == 1) {
                    playground.push_back(Box({ i + 1.f,1.5f,j + 1.f }, 
                                             { 1.f    ,3.f, 1.f }));
                }
                else {
                    possible_enemy_locations.push_back({ i+1.f,1.5f,j+1.f });
                }
            }
        }

        for (int i = 0; i < possible_enemy_locations.size() / 32; i++) {
            int random_enemy_location_index= rand() % (possible_enemy_locations.size() - 1);
            enemies.push_back(Enemy(possible_enemy_locations[random_enemy_location_index], { ENEMY_SIZE,ENEMY_SIZE,ENEMY_SIZE }));
        }
    }
    
    {
        enemies.push_back(test_enemy);
        enemies.push_back(Enemy({ -5.f,1.f,-5.f }, { 1.f,1.f,1.f }));
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

    {
        // TEST RENDERS
        //RenderMesh(meshes["box"], shaders["VertexNormal"], test_box.getModelMatrix());
        //RenderMesh(meshes["sphere"], shaders["VertexNormal"], test_sphere.getModelMatrix());
        //  RenderMesh(meshes["sphere"], shaders["VertexNormal"], test_enemy.body.getModelMatrix());
        //RenderMesh(meshes["sphere"], shaders["Simple"], test_enemy.left_eye.getModelMatrix());
        //RenderMesh(meshes["sphere"], shaders["Simple"], test_enemy.right_eye.getModelMatrix());

    }
    {
        // Check player-enemy collision
        for (int i = 0; i < enemies.size(); i++) {
            if (CheckSpheresCollision({ player.body.getPosition().x, player.body.getPosition().y, player.body.getPosition().z, PLAYER_HITBOX_RADIUS },
                                      { enemies[i].body.getPosition().x,enemies[i].body.getPosition().y ,enemies[i].body.getPosition().z,ENEMY_SIZE })) {
                player.modifyHealth(-20);
                player.Move(-player.getLastMove().x*2, -player.getLastMove().y*2, -player.getLastMove().z*2);
            }
        }
    }
    {
        // Check bullet-enemy collision
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < enemies.size(); j++) {
                if (CheckSpheresCollision({ bullets[i].getPosition().x,bullets[i].getPosition().y, bullets[i].getPosition().z,BULLET_RADIUS },
                                            {enemies[j].body.getPosition().x,enemies[j].body.getPosition().y,enemies[j].body.getPosition().z,ENEMY_SIZE-1.f})){
                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    break;
                }
            }
        }
    }
    {
        // Check player-maze collision
        for (int i = 0; i < playground.size(); i++) {
            if (CheckSpheresCollision({ player.body.getPosition().x, player.body.getPosition().y, player.body.getPosition().z, PLAYER_HITBOX_RADIUS },
                { playground[i].getPosition().x,playground[i].getPosition().y ,playground[i].getPosition().z,PLAYGROUND_BOX_HITBOX_RADIUS })) {
                cout << "COLLISION BOIIII " << endl;
                player.Move( -player.getLastMove().x, -player.getLastMove().y, -player.getLastMove().z );
            }
        }
       
    }

    {
        //BULLETS POSITION & LIFE 
        for (int i = 0; i < bullets.size(); i++) {
            
            if(bullets[i].getRemainingDistance() <= 0){
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].Move({5.f*deltaTimeSeconds,0.f,0.f });
                bullets[i].ModifyRemainingDistance(-5.f*deltaTimeSeconds    );
            }
        }

        // BULLETS-WALLS COLLISIONS CHECK
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < playground.size(); j++) {
                if (CheckSpheresCollision({ bullets[i].getPosition().x, bullets[i].getPosition().y, bullets[i].getPosition().z,bullets[i].getRadius() },
                                          { playground[j].getPosition().x, playground[j].getPosition().y,playground[j].getPosition().z, PLAYGROUND_BOX_HITBOX_RADIUS})) {
                    bullets.erase(bullets.begin() + i);
                    break;
                }
            }
        }
    }
    

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

    {
        // MAZE RENDERING
        for (int i = 0; i < playground.size(); i++) {
            RenderMesh(meshes["box"], shaders["VertexNormal"], playground[i].getModelMatrix());;  // body
        }
    }

    {
        // ENEMIES RENDER
        for (int i = 0; i < enemies.size(); i++) {
            RenderMesh(meshes["sphere"], shaders["VertexNormal"], enemies[i].body.getModelMatrix());
            RenderMesh(meshes["sphere"], shaders["Simple"], enemies[i].left_eye.getModelMatrix());
            RenderMesh(meshes["sphere"], shaders["Simple"], enemies[i].right_eye.getModelMatrix());
        }
    }


    {
        // BULLETS RENDER 
        for (int i = 0; i < bullets.size(); i++) {
            RenderMesh(meshes["sphere"], shaders["VertexNormal"], bullets[i].getModelMatrix());
        }
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

void SurvivalMaze::DoPlayerObjectsCollisions() {
    for (int i = 0; i < playground.size(); i++) {
        if (CheckSpheresCollision({ player.body.getPosition().x, player.body.getPosition().y, player.body.getPosition().z, PLAYER_HITBOX_RADIUS },
                                  { playground[i].getPosition().x,playground[i].getPosition().y ,playground[i].getPosition().z, PLAYGROUND_BOX_HITBOX_RADIUS })) {
            cout << "COLLISION BOIIII " << endl;
            player.Move(-player.getLastMove().x, -player.getLastMove().y, -player.getLastMove().z);
        }
    }
}

void SurvivalMaze::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
          // Check player-maze collision
  

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
            DoPlayerObjectsCollisions();
            translateZ += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_W)) {
            translateZ -= deltaTime;
            player.Move(0, 0, deltaTime);
            DoPlayerObjectsCollisions();

         
        }
        if (window->KeyHold(GLFW_KEY_A)) {

            player.Move(+deltaTime, 0, 0);
            DoPlayerObjectsCollisions();

            translateX -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            player.Move(-deltaTime, 0, 0);
            DoPlayerObjectsCollisions();

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
    if (key == GLFW_KEY_LEFT_CONTROL)
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

    else if (key == GLFW_KEY_SPACE)
    {
        bullets.push_back(Bullet({player.body.getPosition().x,player.body.getPosition().y,player.body.getPosition().z}, BULLET_RADIUS));
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
