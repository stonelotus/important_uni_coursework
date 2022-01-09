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

    // make exit 
    bool has_exit = false;
    while (!has_exit) {
        int exit_position = rand() % (maze[0].size() - 2) + 2;
        if(maze[1][exit_position] == EMPTY_SPACE_MARKER){
            maze[0][exit_position] = EMPTY_SPACE_MARKER;
            has_exit = true;
        }
    }


}
///////////////////////////////
void LimitedFillerDFS(vector<vector<int>>& playground_matrix, int x, int y, int remaining_depth)
{
    if (playground_matrix[x][y] == 0) {
        playground_matrix[x][y] = ENEMY_PATH_MARKER;
    }
    if (remaining_depth <= 0 || playground_matrix[x][y] == WALL_MARKER) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (x+di[i] < playground_matrix.size()-1 && y+dj[i] < playground_matrix.size() - 1 && x+di[i] >=0 && y+dj[i] >= 0) {
            LimitedFillerDFS(playground_matrix, x + di[i], y + dj[i], remaining_depth - 1);
        }
    }
}
void SetupEnemiesRoutes(vector<vector<int>>& playground_matrix, int path_depth)
{
    for (int i = 0; i < playground_matrix.size(); i++) {
        for (int j = 0; j < playground_matrix[i].size(); j++) {
            if (playground_matrix[i][j] == ENEMY_MARKER) {
                LimitedFillerDFS(playground_matrix, i, j, path_depth);
            }
        }
    }
   
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
    timpEnemy = 10000;
    
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //Simple Shader 
    {
        Shader* shader = new Shader("MazeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        // EPIC SHADER
        Shader* shader2 = new Shader("EpicShader");
        shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader2.glsl"), GL_VERTEX_SHADER);
        shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader2.glsl"), GL_FRAGMENT_SHADER);
        shader2->CreateAndLink();
        shaders[shader2->GetName()] = shader2;
    }
    
    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    //box = Box(10.f);
    {
        //test_sphere = Sphere({ 1.f,1.f,1.f }, 6.f);
        //test_box = Box({ 0.f,0.f,0.f }, { 1.f,1.f,1.f });
        //test_enemy = Enemy({ -3.f,1.f,-3.f }, { 1.f,1.f,1.f });
    }
    //player = Player(-5.f, 1.f, -5.f);


    {  
        amazing_rotate_angle = 0.f;
        isFirstPerson = false;
        camera = new implemented::Camera();
        camera->Set(glm::vec3(player.getPosition().x, player.getPosition().y, player.getPosition().z), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        projectionMatrix = glm::perspective(RADIANS(55.0f), window->props.aspectRatio, 0.01f, 200.0f);
    }

    // playground maze creation
    {
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
                    playground.push_back(Box({ i + PLAYGROUND_MATRIX_OFFSET,1.5f,j + PLAYGROUND_MATRIX_OFFSET },
                                             { 1.f    ,3.f, 1.f }));
                    float c1 = (rand() % 1000) / 1000.f;
                    float c2 = (rand() % 1000) / 1000.f;
                    float c3 = (rand() % 1000) / 1000.f;
                    cout << c1 << " " << c2 << " " << c3 << endl;
                    playground[playground.size() - 1].setShaderColor(glm::vec3(c1, c2, c3));
                }
                else {
                   /* playground.push_back(Box({ i + 1.f,0.f,j + 1.f },
                                             { 1.f    ,0.2f, 1.f }));
                    playground[playground.size() - 1].setShaderName("Simple");*/
                    possible_enemy_locations.push_back({ i+ PLAYGROUND_MATRIX_OFFSET,1.f,j+ PLAYGROUND_MATRIX_OFFSET });
               
 }
            }
        }

        for (int i = 0; i < possible_enemy_locations.size() / 32; i++) {
            int random_enemy_location_index= rand() % (possible_enemy_locations.size() - 1);
            enemies.push_back(Enemy(possible_enemy_locations[random_enemy_location_index], { ENEMY_SIZE,ENEMY_SIZE,ENEMY_SIZE }));
          
            int position_i = possible_enemy_locations[random_enemy_location_index].x - PLAYGROUND_MATRIX_OFFSET;
            int position_j = possible_enemy_locations[random_enemy_location_index].z - PLAYGROUND_MATRIX_OFFSET;
            cout << "Enemy added on: " << possible_enemy_locations[random_enemy_location_index].x << " " << possible_enemy_locations[random_enemy_location_index].z << endl;
            playground_matrix[position_i][position_j] = ENEMY_MARKER;
      
        }

        SetupEnemiesRoutes(playground_matrix, ENEMY_ROUTES_DEPTH);

        for (int i = 0; i < playground_matrix.size(); i++) {
            for (int j = 0; j < playground_matrix[i].size(); j++) {
                if (playground_matrix[i][j] == ENEMY_MARKER || playground_matrix[i][j] == ENEMY_PATH_MARKER) {
                    playground.push_back(Box({ i + PLAYGROUND_MATRIX_OFFSET,0.f, j + PLAYGROUND_MATRIX_OFFSET },
                        { 1.f    ,0.2f, 1.f }));
                 
                    playground[playground.size() - 1].setShaderColor(glm::vec3(0.f,0.f,0.f));
                }
            }
            cout << endl;
        }

      
        bool player_positioned = false;
        while (player_positioned == false) {
            int random_player_entry_i = rand() % (playground_matrix.size() - 2) + 2;
            int random_player_entry_j = rand() % (playground_matrix.size() - 2) + 2;

            if (playground_matrix[random_player_entry_i][random_player_entry_j] == 0) {
                player_positioned = true;
                player = Player(random_player_entry_i + PLAYGROUND_MATRIX_OFFSET, 1.f, random_player_entry_j + PLAYGROUND_MATRIX_OFFSET);
            }
        }
    }
    
    {
        enemies.push_back(Enemy({ -2.f,1.f,-2.f }, { 1.f,1.f,1.f }));
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
    
    if (isFirstPerson) {
        camera->position = glm::vec3(player.body.getPosition().x, player.body.getPosition().y, player.body.getPosition().z);
    }
    else {
        camera->position = glm::vec3(player.body.getPosition().x, player.body.getPosition().y+5.f, player.body.getPosition().z - 3.5);
    }
    

    {
        // TEST RENDERS
        //RenderSimpleMesh(meshes["box"], shaders["VertexNormal"], test_box.getModelMatrix(),glm::vec3(1,0,0));
       // RenderMesh(meshes["sphere"], shaders["VertexNormal"], test_sphere.getModelMatrix());
        //  RenderMesh(meshes["sphere"], shaders["VertexNormal"], test_enemy.body.getModelMatrix());
        //RenderMesh(meshes["sphere"], shaders["Simple"], test_enemy.left_eye.getModelMatrix());
        //RenderMesh(meshes["sphere"], shaders["Simple"], test_enemy.right_eye.getModelMatrix());

    }
    {
        // CHECK player win or lose
        int player_matrix_i = (int)round(player.body.getPosition().x - PLAYGROUND_MATRIX_OFFSET);
        int player_matrix_j = (int)round(player.body.getPosition().z - PLAYGROUND_MATRIX_OFFSET);

        if (player_matrix_i == 0 || player_matrix_i == playground_matrix.size() - 1 || player_matrix_j == 0 || player_matrix_i == 0) {
            if (playground_matrix[player_matrix_i][player_matrix_j] == 0) {
                cout << "You somehow won!!!";
                exit(0);
            }
        }

        if (player.getHealth() <= 0) {
            cout << "You lost :( ";
            exit(0);
        }
    }
    {
        // Check player-enemy collision
        for (int i = 0; i < enemies.size(); i++) {
            if (CheckSpheresCollision({ player.body.getPosition().x, player.body.getPosition().y, player.body.getPosition().z, PLAYER_HITBOX_RADIUS },
                                      { enemies[i].body.getPosition().x,enemies[i].body.getPosition().y ,enemies[i].body.getPosition().z,ENEMY_SIZE })) {
                player.modifyHealth(-20);
                player.Move(-player.getLastMove().x, -player.getLastMove().y, -player.getLastMove().z);
            }
        }
    }

    {
        // Check bullet-enemy collision
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < enemies.size(); j++) {
                if (CheckSpheresCollision(
                    { bullets[i].getPosition().x,bullets[i].getPosition().y, bullets[i].getPosition().z,BULLET_RADIUS },
                    {enemies[j].body.getPosition().x,enemies[j].body.getPosition().y,enemies[j].body.getPosition().z,ENEMY_SIZE-1.f})){
                    

                    enemies[j].setCountdown(enemies[j].getCountdown() - deltaTimeSeconds * 2);
                    bullets.erase(bullets.begin() + i);
                    break;
                  
                }
            }
        }
    }
    {
        // Check player-maze collision
        for (int i = 0; i < playground.size(); i++) {
            if (CheckSpheresCollision(
                { player.body.getPosition().x, player.body.getPosition().y, player.body.getPosition().z, PLAYER_HITBOX_RADIUS },
                { playground[i].getPosition().x,playground[i].getPosition().y ,playground[i].getPosition().z,PLAYGROUND_BOX_HITBOX_RADIUS })) {
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
                bullets[i].Move({sin(bullets[i].getAngle()) * deltaTimeSeconds*5,0.f,cos(bullets[i].getAngle()) * deltaTimeSeconds * 5 });
                bullets[i].ModifyRemainingDistance(-5.f*deltaTimeSeconds);
            }
        }


        {
            ////ENEMIES MOVE
            for (int i = 0; i < enemies.size(); i++) {
                dimensionsTriplet enemy_position = enemies[i].body.getPosition();
                //cout << enemy_position.x << " " << enemy_position.z << endl;
                int enemy_matrix_i = (int)round(enemy_position.x - PLAYGROUND_MATRIX_OFFSET);
                int enemy_matrix_j = (int)round(enemy_position.z - PLAYGROUND_MATRIX_OFFSET);

                int current_enemy_direction = enemies[i].getDirection();
                if (enemy_matrix_i + di[current_enemy_direction]  < playground_matrix.size() && enemy_matrix_i + di[current_enemy_direction] >= 0 &&
                    enemy_matrix_j + dj[current_enemy_direction]  < playground_matrix[enemy_matrix_i + di[current_enemy_direction]].size() -1 &&
                    enemy_matrix_j + dj[current_enemy_direction] >= 0 &&
                    (
                    playground_matrix[enemy_matrix_i + di[current_enemy_direction]][enemy_matrix_j + dj[current_enemy_direction]] == ENEMY_MARKER 
                    || playground_matrix[enemy_matrix_i + di[current_enemy_direction]][enemy_matrix_j + dj[current_enemy_direction]] == ENEMY_PATH_MARKER) ){
                    enemies[i].Move({ deltaTimeSeconds * di[current_enemy_direction], 0.f, deltaTimeSeconds * dj[current_enemy_direction] });

                }
                else {
                    enemies[i].setDirection(current_enemy_direction + 1);
                }
  
                
            }
        }

        // BULLETS-WALLS COLLISIONS CHECK
    /*    for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < playground.size(); j++) {
                if (CheckSpheresCollision({ bullets[i].getPosition().x, bullets[i].getPosition().y, bullets[i].getPosition().z,bullets[i].getRadius() },
                                          { playground[j].getPosition().x, playground[j].getPosition().y,playground[j].getPosition().z, PLAYGROUND_BOX_HITBOX_RADIUS})) {
                    bullets.erase(bullets.begin() + i);
                    break;
                }
            }
        }*/
    }
    

    {   //PLAYER RENDER
        if (!isFirstPerson) {
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.body.getModelMatrix(), glm::vec3(1, 0, 1));   // body
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.head.getModelMatrix(), glm::vec3(1, 0.5, 1));   // head
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.left_leg.getModelMatrix(), glm::vec3(0, 0, 1));    // left_leg
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.right_leg.getModelMatrix(), glm::vec3(0.1, 0, 1));   // right_leg
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.left_arm.getModelMatrix(), glm::vec3(0.2, 0, 1));    // left_arm
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.right_arm.getModelMatrix(), glm::vec3(0.3, 0, 1));   // right_arm
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.left_palm.getModelMatrix(), glm::vec3(0.4, 0, 1));   // right_arm
            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], player.right_palm.getModelMatrix(), glm::vec3(0.5, 0, 1));   // right_arm

        }
    }

    {
        // MAZE RENDERING
        for (int i = 0; i < playground.size(); i++) {
            //RenderMesh(meshes["box"], shaders[playground[i].getShaderName()], playground[i].getModelMatrix());;  // body
             RenderSimpleMesh(meshes["box"], shaders["MazeShader"], playground[i].getModelMatrix(),playground[i].getShaderColor());  // body

        }
    }

    {
        // ENEMIES RENDER
        for (int i = 0; i < enemies.size(); i++) {
            if (enemies[i].getCountdown() <= 0) {
                enemies.erase(enemies.begin() + i);
                break;
            }
            else if (enemies[i].getCountdown() == 2) {
                RenderSimpleMesh(meshes["sphere"], shaders["VertexNormal"], enemies[i].body.getModelMatrix(), glm::vec3(0.5f, 0.5f, 0.5f));
                RenderSimpleMesh(meshes["sphere"], shaders["Simple"], enemies[i].left_eye.getModelMatrix(), glm::vec3(0.5f, 0.5f, 0.5f));
                RenderSimpleMesh(meshes["sphere"], shaders["Simple"], enemies[i].right_eye.getModelMatrix(), glm::vec3(0.5f, 0.5f, 0.5f));
            }
            else {

                RenderEpicMesh(meshes["sphere"], shaders["EpicShader"], enemies[i].body.getModelMatrix(), enemies[i].getCountdown());
                RenderEpicMesh(meshes["sphere"], shaders["EpicShader"], enemies[i].left_eye.getModelMatrix(), enemies[i].getCountdown());
                RenderEpicMesh(meshes["sphere"], shaders["EpicShader"], enemies[i].right_eye.getModelMatrix(), enemies[i].getCountdown());
                enemies[i].setCountdown(enemies[i].getCountdown() - deltaTimeSeconds * 2);
            }
            
        }
    }


    {
        // BULLETS RENDER 
        for (int i = 0; i < bullets.size(); i++) {
            RenderSimpleMesh(meshes["sphere"], shaders["VertexNormal"], bullets[i].getModelMatrix(), glm::vec3(0.5f, 0.5f, 0.5f));
        }
    }

  
    

}


void SurvivalMaze::FrameEnd()
{
    //DrawCoordinateSystem();

    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void SurvivalMaze::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    shader->Use();

    GLint object_clr = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(object_clr, 1, glm::value_ptr(color));

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    // TODO camera
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    //glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    //TODO camera
    //glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void SurvivalMaze::RenderEpicMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,float time)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"

    int model_location = glGetUniformLocation(shader->GetProgramID(), "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix

    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"

    GLint location_View = glGetUniformLocation(shader->GetProgramID(), "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();

    glUniformMatrix4fv(location_View, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"

    GLint location_Projection = glGetUniformLocation(shader->GetProgramID(), "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    //glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();

    glUniformMatrix4fv(location_Projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "EnemyTime");

    // Set shader uniform "Time" to elapsed time
    glUniform1f(timeLocation, time);


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
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
    deltaTime = deltaTime * 3;

    //if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
        }
        if (window->KeyHold(GLFW_KEY_LEFT)) {

        }
        if (window->KeyHold(GLFW_KEY_UP)) {
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            player.Move(0, 0, -deltaTime);
            DoPlayerObjectsCollisions();
        }
        if (window->KeyHold(GLFW_KEY_W)) {
            player.Move(0, 0, deltaTime);
            DoPlayerObjectsCollisions();

         
        }
        if (window->KeyHold(GLFW_KEY_A)) {

            player.Move(+deltaTime, 0, 0);
            DoPlayerObjectsCollisions();

        }
        if (window->KeyHold(GLFW_KEY_D)) {
            player.Move(-deltaTime, 0, 0);
            DoPlayerObjectsCollisions();

        }
        if (window->KeyHold(GLFW_KEY_R)) {
        }
        if (window->KeyHold(GLFW_KEY_F)) {
        }
    //}
   
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
        if (isFirstPerson) {
        float bullet_angle = amazing_rotate_angle + M_PI/4 + 0.25f;
            
            if (bullet_angle > M_PI*2) {
                bullet_angle = bullet_angle - M_PI*2;
            }
            bullets.push_back(Bullet({ player.body.getPosition().x,1.f,player.body.getPosition().z }, BULLET_RADIUS, bullet_angle));
            //cout << amazing_rotate_angle << endl;
        }
    }

    if (key == GLFW_KEY_RIGHT_CONTROL) {
        //TODO change it a lil bit
        isFirstPerson = isFirstPerson ? false : true;
    }
}


void SurvivalMaze::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void SurvivalMaze::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensitivityOX = 0.001f;
    float sensitivityOY = 0.001f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        cout << amazing_rotate_angle << endl;
        
        switch (isFirstPerson) {
        case true:
            camera->RotateFirstPerson_OX(sensitivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensitivityOY * -deltaX);
            amazing_rotate_angle += sensitivityOY * -deltaX;
            if (amazing_rotate_angle >= M_PI * 2) {
                amazing_rotate_angle -= M_PI*2 ;
            }
            else if (amazing_rotate_angle < 0) {
                amazing_rotate_angle +=  M_PI * 2;
            }
            break;
        case false:
            camera->RotateThirdPerson_OX(sensitivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensitivityOY * -deltaX);
            amazing_rotate_angle += sensitivityOY * -deltaX;
            if (amazing_rotate_angle >= M_PI * 2) {
                amazing_rotate_angle -= 0;
            }
            else if (amazing_rotate_angle < 0) {
                amazing_rotate_angle += M_PI * 2;
            }
            break;
        default:
            break;
        }
        
        player.setAngle(amazing_rotate_angle);
    }
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
