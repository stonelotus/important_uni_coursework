#pragma once
#ifndef SURVIVAL_MAZE_H
#define SURVIVAL_MAZE_H
#include "components/simple_scene.h"
#include "lab_m1/tema2/entities/player/player.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <random>
#include "lab_m1/tema2/lab_camera.h"
#include "lab_m1/tema2/entities/sphere/sphere.h"
#include "lab_m1/tema2/entities/bullet/bullet.h"
#include "lab_m1/tema2/entities/enemy/enemy.h"
using namespace std;


    class SurvivalMaze : public gfxc::SimpleScene
    {
    public:
        SurvivalMaze();
        ~SurvivalMaze();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        void RenderEpicMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,float time);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat4 modelMatrix, wheelModelMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;
        int timpEnemy;
        //camera
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool isFirstPerson;

        float carTranslateX, carTranslateZ;
        float carAngularStepOZ;

        Player player;
        Box test_box;
        vector<Box> playground;
        Sphere test_sphere;
        Bullet test_bullet;
        Enemy test_enemy;
        void DoPlayerObjectsCollisions();
        vector<Bullet>bullets;
        vector<Enemy> enemies;
        vector<vector<int>> playground_matrix;
        float amazing_rotate_angle;
    };

#endif