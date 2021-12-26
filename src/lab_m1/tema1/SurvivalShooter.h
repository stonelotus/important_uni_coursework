#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include <lab_m1/tema1/object2D.h>
#include <lab_m1/tema1/transform2D.h>
#include "bullet.h"
#include "Rectangle.h"
#include "enemy.h"
#include <time.h>


#define TWO_PI			(6.28318530718f)
#define NUM_TRIANGLES	(50)

using namespace std;
class SurvivalShooter : public gfxc::SimpleScene
{
public:
    time_t seconds;
    time_t lastEnemySpawnedMoment;
    enum Direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };
    typedef std::tuple<bool, Direction, glm::vec2> Collision;

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

    SurvivalShooter();
    ~SurvivalShooter();

    void Init() override;

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;


    Collision CheckCollision(float sizeX, float sizeY, float posX, float posY, float playerX, float playerY, float hitBoxRadius);
    Direction VectorDirection(glm::vec2 target);
    void DoCollision(float sizeX, float sizeY, float X, float Y, float hitBoxRadius);
    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;
    void insideUpdate(float, float, float);
    
    glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

    void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);


protected:
    glm::mat3 playerMatrix;
    struct {
        float X, Y, angle;
        int health,score;
    }player;

    float length;
    float mouseCrtX, mouseCrtY;
    ViewportSpace viewSpace;
    LogicSpace logicSpace;
    glm::mat3 modelMatrix, visMatrix;
    
    vector<Bullet> activeBullets;
    vector<Rectangle> obstacles,healthPacks;
    vector<Enemy> enemies;
    float bulletsCooldown;
};
