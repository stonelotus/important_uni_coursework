#include "SurvivalShooter.h"

#define BULLETS_SPEED 500
#define BULLETS_COOLDOWN 0.5
#define ENEMY_BODY_LENGTH 130.f
#define ENEMY_EYE_LENGTH 50.f
#define PLAYER_HITBOX 70.f
#define BULLET_HITBOX 30.f
#define HEALTH_PACK_SIZE 60.f
using namespace std;

SurvivalShooter::SurvivalShooter(){}
SurvivalShooter::~SurvivalShooter(){}

void SurvivalShooter::Init() 
{
    glm::ivec2 resolution = window->GetResolution();

    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    glDepthFunc(GL_ALWAYS);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 1000;   // logic width
    logicSpace.height = 1000;  // logic height
    length = 20;


    {
        // I should have just made a damn class for player.
        // But I really wanted to use structs to remember how to use them.
        // I will always regret this decision. Classes rule. 
        player.X = 500;
        player.Y = 500;
        player.angle = 0.f;
        player.health = 100;
        player.score = 10;

        Mesh* player_body = object2D::createCircle(50.f, 50.f, "player_body", glm::vec3(0.1,1,0.1), true);
        AddMeshToList(player_body);

        Mesh* player_eye = object2D::createCircle(30.f, 30.f, "player_eye", glm::vec4(255, 255, 0, 0), true);
        AddMeshToList(player_eye);
    }
    {
        // OBSTACLE SQUARE AND HEALTHBAR 
        Mesh* square1 = object2D::CreateSquare("square1", corner, 1800.f, glm::vec3(1, 0, 0), true);
        AddMeshToList(square1);
        Mesh* emptySquare = object2D::CreateSquare("emptySquare", corner, 100.f, glm::vec3(0.7f, 0.7f, 0.7f), false);
        AddMeshToList(emptySquare);
        Mesh* fullSquare = object2D::CreateSquare("fullSquare", corner, 100.f, glm::vec3(0.7f, 1, 0), true);
        AddMeshToList(fullSquare);
    }
    {
        // Enemy parts
        Mesh* enemyBody = object2D::CreateSquare("enemyBody", corner, ENEMY_BODY_LENGTH, glm::vec3(0,0.3f,0.5f), true);
        AddMeshToList(enemyBody);

        Mesh* enemyEye = object2D::CreateSquare("enemyEye", corner, ENEMY_EYE_LENGTH, glm::vec3(0,250,0), true);
        AddMeshToList(enemyEye);
    }

    {
        // BULLETS
        Mesh* bullet = object2D::CreateSquare("bullet", corner, 23.f, glm::vec3(255, 100, 13), true);
        AddMeshToList(bullet);

        bulletsCooldown = 0;
    }

    {
        // Obstacles
        obstacles.push_back(Rectangle(100.f, 400.f,0.1f , 0.1f));
        obstacles.push_back(Rectangle(200.f, 600.f, 0.3f, 0.05f));

        //walls of doom
        obstacles.push_back(Rectangle(-500.f, 0, 1.f, 0.08f));
        obstacles.push_back(Rectangle(-650.f, 0, 0.08f, 1.f));
        obstacles.push_back(Rectangle(1300.f, 0, 0.08f, 1.f));
        obstacles.push_back(Rectangle(-503.f, 1656, 1.f, 0.08f));
    }
    {
        // MISC
        // Health bonus 
        healthPacks.push_back(Rectangle(50.f, 200.f, 1.f, 1.f));
        healthPacks.push_back(Rectangle(1000.f, 180.f, 1.f, 1.f));
        healthPacks.push_back(Rectangle(1000.f, 1500, 1.f, 1.f));
        healthPacks.push_back(Rectangle(130.f, 860, 1.f, 1.f));

        Mesh* healthPack = object2D::CreateSquare("healthPack", corner, HEALTH_PACK_SIZE, glm::vec3(0.7f, 1, 0), true);
        AddMeshToList(healthPack);
    }

    {
        // Timer between enemy spawns
        seconds = time(NULL);
        lastEnemySpawnedMoment = seconds;
        enemies.push_back(Enemy(rand() % 1000 + 200, rand() % 1000 + 200, 1));

    }
}




// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 SurvivalShooter::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void SurvivalShooter::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void SurvivalShooter::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void SurvivalShooter::Update(float deltaTimeSeconds)
{
    insideUpdate(deltaTimeSeconds, 1, 1);
    // BONUS Minimap!
    // For it, I just change the viewSpace and reapply update! The minimap shall be in the bottom-left corner;
    insideUpdate(deltaTimeSeconds, 5, 5);
}

void SurvivalShooter::insideUpdate(float deltaTimeSeconds, float resolutionDividerX, float resolutionDividerY)
{
    seconds = time(NULL);
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x/resolutionDividerX, resolution.y/ resolutionDividerY);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    {
        // Here we place the Player... Noobly, without class :'( 
        playerMatrix = glm::mat3(1);
        playerMatrix *= visMatrix * transform2D::Translate(player.X, player.Y);
        playerMatrix *= transform2D::Rotate(player.angle);
        RenderMesh2D(meshes["player_body"], shaders["VertexColor"], playerMatrix);

        playerMatrix *= transform2D::Translate(40, 50);
        RenderMesh2D(meshes["player_eye"], shaders["VertexColor"], playerMatrix);

        playerMatrix *= transform2D::Translate(-80, 0);
        RenderMesh2D(meshes["player_eye"], shaders["VertexColor"], playerMatrix);

        // Health testing 
        if (player.health <= 0) {
            cout << "You finally lost!" << endl;
            cout << "You got score: " << player.score << endl;
            system("pause");
            cout << "=================\n";
            cout << "=================\n";
            cout << "=================\n";
            cout << "Bye!!!";
            exit(1);
        }
    }

    {
        // OBSTACLES 
        for (int i = 0; i < obstacles.size(); i++)
        {
            RenderMesh2D(meshes["square1"], shaders["VertexColor"], visMatrix * obstacles[i].getModelMatrix());
        }

    }
    {
        /// ENEMIES
        // Setting enemies movement
        for (int i = 0; i < enemies.size(); i++)
        {
            float angle;
            if (enemies[i].getX() <= player.X) {
                angle = atan((enemies[i].getY() - player.Y) / (enemies[i].getX() - player.X));
            }
            else {
                angle = 3.14 + atan((enemies[i].getY() - player.Y) / (enemies[i].getX() - player.X));
            }

            float newEnemyX = enemies[i].getX() + cos(angle) * enemies[i].getSpeed() * deltaTimeSeconds;
            float newEnemyY = enemies[i].getY() + sin(angle) * enemies[i].getSpeed() * deltaTimeSeconds;
            enemies[i].setModelMatrix(newEnemyX, newEnemyY, angle);

            RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], visMatrix * enemies[i].getModelMatrix());
            RenderMesh2D(meshes["enemyEye"], shaders["VertexColor"], visMatrix * enemies[i].eye1.getModelMatrix());
            RenderMesh2D(meshes["enemyEye"], shaders["VertexColor"], visMatrix * enemies[i].eye2.getModelMatrix());
        }

        {
            // Spawning enemies after 3 seconds
            if (seconds - lastEnemySpawnedMoment >= 3)
            {
                lastEnemySpawnedMoment = seconds;
                enemies.push_back(Enemy(rand() % 1000 + 200, rand() % 1000 + 200, 1));
            }
        }
    }

    {
        // HEALTH PACKS
        for (int i = 0; i < healthPacks.size(); i++) {
            RenderMesh2D(meshes["healthPack"], shaders["VertexColor"], visMatrix * healthPacks[i].getModelMatrix());
        }
    }
    {
        //BULLETS
        if (bulletsCooldown > 0) {
            bulletsCooldown -= deltaTimeSeconds;
        }
        for (int i = 0; i < activeBullets.size(); i++) {
            float newBulletX, newBulletY;
            newBulletX = activeBullets[i].getCurrentX() + cos(activeBullets[i].getShootAngle()) * BULLETS_SPEED * deltaTimeSeconds;
            newBulletY = activeBullets[i].getCurrentY() + sin(activeBullets[i].getShootAngle()) * BULLETS_SPEED * deltaTimeSeconds;
            activeBullets[i].setX(newBulletX);
            activeBullets[i].setY(newBulletY);

            modelMatrix = glm::mat3(1) * visMatrix;  // Should have set it with the class ... but no more time
            modelMatrix *= transform2D::Translate(activeBullets[i].getCurrentX(), activeBullets[i].getCurrentY());
            modelMatrix *= transform2D::Rotate(activeBullets[i].getShootAngle());
            modelMatrix *= transform2D::Translate(0, 0);

            activeBullets[i].setRemainingDistance(activeBullets[i].getRemainingDistance() - BULLETS_SPEED * deltaTimeSeconds);  // checking if bullet still has range in it
            if (activeBullets[i].getRemainingDistance() > 0) {
                RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
            }
            else {
                activeBullets.erase(activeBullets.begin() + i);
            }

        }
        // Collisions between bullets and obstacles (including walls, as they're obstacles!)
        for (int i = 0; i < activeBullets.size(); i++)
        {
            for (int j = 0; j < obstacles.size(); j++)
            {
                Collision coll = CheckCollision(1800 * obstacles[j].getSizeX(), 1800 * obstacles[j].getSizeY(), obstacles[j].getX(), obstacles[j].getY(), activeBullets[i].getCurrentX(), activeBullets[i].getCurrentY(), BULLET_HITBOX);
                if (std::get<0>(coll))
                {
                    activeBullets.erase(activeBullets.begin() + i);
                    break;
                }
            }

        }

        for (int i = 0; i < enemies.size(); i++)
        {
            Collision coll = CheckCollision(ENEMY_BODY_LENGTH, ENEMY_BODY_LENGTH, enemies[i].getX(), enemies[i].getY(), player.X, player.Y, PLAYER_HITBOX);
            if (std::get<0>(coll))
            {
                player.health -= 33;
                enemies.erase(enemies.begin() + i);
            }
        }

        for (int i = 0; i < activeBullets.size(); i++)
        {
            for (int j = 0; j < enemies.size(); j++)
            {
                Collision coll = CheckCollision(ENEMY_BODY_LENGTH, ENEMY_BODY_LENGTH, enemies[j].getX(), enemies[j].getY(), activeBullets[i].getCurrentX(), activeBullets[i].getCurrentY(), BULLET_HITBOX);
                if (std::get<0>(coll))
                {
                    player.score += 10;
                    activeBullets.erase(activeBullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    break;
                }
            }
        }
    }
    {
        // Health pack collisions with player
        // BONUS
        for (int i = 0; i < healthPacks.size(); i++)
        {
            Collision coll = CheckCollision(HEALTH_PACK_SIZE, HEALTH_PACK_SIZE, healthPacks[i].getX(), healthPacks[i].getY(), player.X, player.Y, PLAYER_HITBOX);
            if (std::get<0>(coll) && player.health < 100)
            {
                player.health += 15;
                if (player.health > 100) {
                    player.health = 100;
                }
                healthPacks.erase(healthPacks.begin() + i);
                break;
            }
        }
    }
    {
        //obstacles & walls collisions
        for (int i = 0; i < obstacles.size(); i++)
        {
            DoCollision(1800 * obstacles[i].getSizeX(), 1800 * obstacles[i].getSizeY(), obstacles[i].getX(), obstacles[i].getY(), PLAYER_HITBOX);
        }
    }

    {
        // HEALTH BAR
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(player.X + 400, player.Y + 350);
        modelMatrix *= transform2D::Scale(4.f, 1.2f);
        RenderMesh2D(meshes["fullSquare"], shaders["VertexColor"], visMatrix * modelMatrix * transform2D::Scale((float)player.health / 100, 1.f));
        RenderMesh2D(meshes["emptySquare"], shaders["VertexColor"], visMatrix * modelMatrix);
    }
}
void SurvivalShooter::FrameEnd()
{
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void SurvivalShooter::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) 
    {
        player.Y += deltaTime * 530;
        logicSpace.y += deltaTime * 530;
        
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        player.Y -= deltaTime * 530;
        logicSpace.y -= deltaTime * 530;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        player.X -= deltaTime * 530;
        logicSpace.x -= deltaTime * 530;
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        player.X += deltaTime * 530;
        logicSpace.x += deltaTime * 530;
    }
}


void SurvivalShooter::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void SurvivalShooter::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void SurvivalShooter::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();

    mouseY = resolution.y - mouseY;

    float middleResolutionX = resolution.x / 2;
    float middleResolutionY = resolution.y / 2;

    if (mouseX <= resolution.x / 2) {
        player.angle = atan((mouseY - middleResolutionY) / (mouseX - middleResolutionX));
    }
    else {
        player.angle = 3.14 + atan((mouseY - middleResolutionY) / (mouseX - middleResolutionX));
    }
    player.angle += M_PI / 2;
}


SurvivalShooter::Collision SurvivalShooter::CheckCollision(float sizeX, float sizeY, float posX, float posY, float playerX, float playerY,float hitBoxRadius)
{
    // get center point for player
    glm::vec2 center(playerX, playerY);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(sizeX / 2.0f, sizeY / 2.0f);
    glm::vec2 aabb_center(
        posX + aabb_half_extents.x,
        posY + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    if (glm::length(difference) < hitBoxRadius) {
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else {
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
    }
}

SurvivalShooter::Direction SurvivalShooter::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}


void SurvivalShooter::DoCollision(float sizeX, float sizeY, float X, float Y, float hitBoxRadius)
{
    Collision collision = CheckCollision(sizeX, sizeY, X, Y, player.X, player.Y,hitBoxRadius);
    if (std::get<0>(collision)) 
    {
        Direction dir = std::get<1>(collision);
        glm::vec2 diff_vector = std::get<2>(collision);

        if (dir == LEFT || dir == RIGHT) 
        {
            float penetration = hitBoxRadius - std::abs(diff_vector.x);
            if (dir == LEFT) {
                player.X += penetration; 
                logicSpace.x += penetration;
            }
            else {
                player.X -= penetration; 
                logicSpace.x -= penetration;
            }
        }
        else {
            float penetration = hitBoxRadius - std::abs(diff_vector.y);
            if (dir == UP) {
                player.Y -= penetration; 
                logicSpace.y -= penetration;
            }
            else {
                player.Y += penetration; 
                logicSpace.y += penetration;
            }

        }
    }

}


void SurvivalShooter::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && bulletsCooldown <= 0 )
    {
        Bullet bullet =  Bullet(player.X, player.Y, player.angle + M_PI/2);
        activeBullets.push_back(bullet);
        bulletsCooldown = BULLETS_COOLDOWN;
    }
}


void SurvivalShooter::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void SurvivalShooter::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void SurvivalShooter::OnWindowResize(int width, int height)
{
}
