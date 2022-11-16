#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <random>
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;

float cx1, cy1, cx2, cy2, cxG, cyG;
float rotateTime1, rotateTime2;
float initialAngle;
float normalAngle;
float circleX, circleY;
float circleX1, circleY1;
float resolutionX;
float resolutionY;
float secondsPassed;
float speed;
float scale;
float latura = 100;
float initialPos;

int nrDucks;
int lives;
int points;
int bullets;

bool isEscaped;
bool isShot;
bool isMissed;

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}



void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    isEscaped = false;
    isShot = false;
    isMissed = false;

    secondsPassed = 0;
    
    float latura = 100;
    cx1 = 3.7 * latura / 10;
    cy1 = 11 * latura / 10;
    cx2 = 6.3 * latura / 10;
    cy2 = 11 * latura / 10;

    cxG = 0;
    cyG = 0;

    resolutionX = resolution.x;
    resolutionY = resolution.y;

    circleX, circleX1 = latura / 2;
    circleY, circleY1 = 1.7 * latura;

    angularStep = 0;
    rotation = 0;
    speed = 150;
    rotateTime1 = 0;
    rotateTime2 = 0;
    scale = 0;

    nrDucks = 1;
    lives = 3;
    points = 0;
    bullets = 3;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> randomizeInPos(100, 1180);
    initialPos = randomizeInPos(mt);

    uniform_real_distribution<float> randomizeAngle(10, 170);
    normalAngle = randomizeAngle(mt);
    initialAngle = (normalAngle - 90) * M_PI / 180;
   
    AddMeshToList(object2D::CreateTriangle("corp", corner, 0, 0, latura, 0, latura / 2, 1.5 * latura, glm::vec3(0, 1, 0)));
    AddMeshToList(object2D::CreateTriangle("aripaS", corner, 0.01 * latura / 10, 11 * latura / 10, 1.5 * latura / 10, 6.1 * latura / 10, 3.7 * latura / 10, 11 * latura / 10, glm::vec3(1,1,0)));
    AddMeshToList(object2D::CreateTriangle("aripaD", corner, 9.99 * latura / 10, 11 * latura / 10, 6.3 * latura / 10, 11 * latura / 10, 8.5 * latura / 10, 6.1 * latura / 10, glm::vec3(1, 1, 0)));
    AddMeshToList(object2D::CreateTriangle("cioc", glm::vec3(latura / 2.4, 1.92 * latura, 0), 0, 0, 20 * latura / 100, 0, (20 * latura / 100) / 2, (20 * latura / 100) * 1.5, glm::vec3(1,0,0)));
    AddMeshToList(object2D::CreateCircle("cap", corner, 30, latura, glm::vec3(0, 0, 1), true));
    AddMeshToList(object2D::CreateCircle("viata", corner, 12, latura, glm::vec3(1, 0.1, 0), true));
    AddMeshToList(object2D::CreateRectangle("scor", corner, 0, 0, 250, 40, glm::vec3(0.90, 0, 0), true));
    AddMeshToList(object2D::CreateRectangle("pointsRectangle", corner, 0, 0, 25, 40, glm::vec3(0,0,0.8),false));
    AddMeshToList(object2D::CreateRectangle("bullet", corner, 0, 0, 10, 35, glm::vec3(0.64, 0.16, 0.16), false));
    AddMeshToList(object2D::CreateRectangle("bulletTop", corner, 0, 0, 10, 5, glm::vec3(0.85, 0.85, 0.10), false));
    AddMeshToList(object2D::CreateRectangle("iarba", corner, 0, 0, resolutionX, 100, glm::vec3(0, 0.7, 0), false));
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.7, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    random_device rd;
    mt19937 mt(rd());

    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["iarba"], shaders["VertexColor"], modelMatrix);

    if (lives > 0) {

        if (bullets > 0) {
            //bulletTop
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(53, 635);
            RenderMesh2D(meshes["bulletTop"], shaders["VertexColor"], modelMatrix);

            //bullet
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(53, 600);
            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
        }
        if (bullets > 1) {
            //bulletTop
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(85, 635);
            RenderMesh2D(meshes["bulletTop"], shaders["VertexColor"], modelMatrix);

            //bullet
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(85, 600);
            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
        }
        if (bullets > 2) {
            //bulletTop
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(115, 635);
            RenderMesh2D(meshes["bulletTop"], shaders["VertexColor"], modelMatrix);

            //bullet
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(115, 600);
            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
        }

        //scor
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1000, 645);
        RenderMesh2D(meshes["scor"], shaders["VertexColor"], modelMatrix);

        //pointsRectangle
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1000, 645);
        if (scale < 11)
            modelMatrix *= transform2D::Scale(scale, 1);
        else
            modelMatrix *= transform2D::Scale(10, 1);
        RenderMesh2D(meshes["pointsRectangle"], shaders["VertexColor"], modelMatrix);

        //vieti
        if (lives > 0) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(10, 500);
            RenderMesh2D(meshes["viata"], shaders["VertexColor"], modelMatrix);
        }
        if (lives > 1) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(40, 500);
            RenderMesh2D(meshes["viata"], shaders["VertexColor"], modelMatrix);
        }
        if (lives > 2) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(70, 500);
            RenderMesh2D(meshes["viata"], shaders["VertexColor"], modelMatrix);
        }

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(initialPos, 0);
        modelMatrix *= transform2D::Translate(cxG, cyG);
        modelMatrix *= transform2D::Translate(circleX1, circleY1);
        modelMatrix *= transform2D::Rotate(initialAngle);
        modelMatrix *= transform2D::Translate(-circleX1, -circleY1);

        RenderMesh2D(meshes["cadru"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cap"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cioc"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["corp"], shaders["VertexColor"], modelMatrix);

        //aripa1
        angularStep += deltaTimeSeconds * 0.5;
        rotateTime1 += deltaTimeSeconds * 0.2;

        if (rotateTime1 < 0.25) {
            modelMatrix *= transform2D::Translate(cx1, cy1);
            modelMatrix *= transform2D::Rotate(-angularStep);
            modelMatrix *= transform2D::Translate(-cx1, -cy1);
            rotateTime1 += deltaTimeSeconds * 0.2;
        }
        else { //reset
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(initialPos, 0);
            modelMatrix *= transform2D::Translate(cxG, cyG);
            modelMatrix *= transform2D::Translate(circleX1, circleY1);
            modelMatrix *= transform2D::Rotate(initialAngle);
            modelMatrix *= transform2D::Translate(-circleX1, -circleY1);
            rotateTime1 = 0;
            angularStep = 0;
        }
        RenderMesh2D(meshes["aripaS"], shaders["VertexColor"], modelMatrix);

        //aripa2
        rotateTime2 += deltaTimeSeconds * 0.2;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(initialPos, 0);
        modelMatrix *= transform2D::Translate(cxG, cyG);
        modelMatrix *= transform2D::Translate(circleX1, circleY1);
        modelMatrix *= transform2D::Rotate(initialAngle);
        modelMatrix *= transform2D::Translate(-circleX1, -circleY1);

        if (rotateTime2 < 0.25) {
            modelMatrix *= transform2D::Translate(cx2, cy2);
            modelMatrix *= transform2D::Rotate(+angularStep);
            modelMatrix *= transform2D::Translate(-cx2, -cy2);
            rotateTime2 += deltaTimeSeconds * 0.2;
        }
        else { //reset
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(initialPos, 0);
            modelMatrix *= transform2D::Translate(cxG, cyG);
            modelMatrix *= transform2D::Translate(circleX1, circleY1);
            modelMatrix *= transform2D::Rotate(initialAngle);
            modelMatrix *= transform2D::Translate(-circleX1, -circleY1);
            rotateTime2 = 0;
            angularStep = 0;
        }
        RenderMesh2D(meshes["aripaD"], shaders["VertexColor"], modelMatrix);

        //updating coordinates for movement logistics

        cxG += deltaTimeSeconds * speed * cos(initialAngle + M_PI / 2);
        cyG += deltaTimeSeconds * speed * sin(initialAngle + M_PI / 2);

        circleX += deltaTimeSeconds * speed * cos(initialAngle + M_PI / 2);
        circleY += deltaTimeSeconds * speed * sin(initialAngle + M_PI / 2);

        //latura / 2
        if (circleY > resolutionY - 150 || circleY + 150 < 0) {
            if (isEscaped) {
                cxG = 0;
                cyG = 0;
                circleX = 0;
                circleY = 0;

                uniform_real_distribution<float> randomizeAngle2(10, 170);
                normalAngle = randomizeAngle2(mt);
                initialAngle = (normalAngle - 90) * M_PI / 180;

                isEscaped = false;
                secondsPassed = 0;
                nrDucks++;
                if (nrDucks % 5 == 0) {
                    speed += 50;
                }

                uniform_real_distribution<float> randomizePos(100, 1180);
                initialPos = randomizePos(mt);
            }
            else if (isShot) {
                cxG = 0;
                cyG = 0;
                circleX = 0;
                circleY = 0;
                uniform_real_distribution<float> randomizeAngle3(10, 170);
                normalAngle = randomizeAngle3(mt);
                initialAngle = (normalAngle - 90) * M_PI / 180;

                isShot = false;
                secondsPassed = 0;
                nrDucks++;
                if (nrDucks % 5 == 0) {
                    speed += 50;
                }

                scale++;
                uniform_real_distribution<float> randomizePos2(100, 1180);
                initialPos = randomizePos2(mt);
            }
            else {
                normalAngle = 360 - normalAngle;
                initialAngle = (normalAngle - 90) * M_PI / 180;
            }
        }

        if (circleX > resolutionX - (initialPos + 50) || circleX + initialPos + 50 < 0) {
            normalAngle = 180 - normalAngle;
            initialAngle = (normalAngle - 90) * M_PI / 180;
        }

        //verifica evadarea
        secondsPassed += deltaTimeSeconds;
        if (secondsPassed >= 10 || bullets == 0) {
            normalAngle = 90;
            initialAngle = (normalAngle - 90) * M_PI / 180;
            isEscaped = true;
            lives--;
            secondsPassed = 0;
            bullets = 3;
        }
        //verifica impuscarea
        if (isShot == true) {
            normalAngle = 270;
            initialAngle = (normalAngle - 90) * M_PI / 180;
            secondsPassed = 0; //reset
            bullets = 3;
        }
    }
    else {
    }
}


void Tema1::FrameEnd()
{
}




void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}   

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (!isEscaped && !isShot) {
        mouseY = resolutionY - mouseY;
            
        unsigned char data[3] = {0, 0, 0};
        glReadPixels(mouseX, mouseY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &data);
        
        if ( (data[0] == 0 && data[1] == 255 && data[2] == 0)
           ||(data[0] == 255 && data[1] == 255 && data[2] == 0)
           ||(data[0] == 255 && data[1] == 0 && data[2] == 0) 
           ||(data[0] == 0 && data[1] == 0 && data[2] == 255)) {
            
            isShot = true;
        }
        bullets--;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
