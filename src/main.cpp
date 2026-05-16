// TODO: надо ли?
// #define GLFW_INCLUDE_GLCOREARB 1 // Tell GLFW to include the OpenGL core profile header
#define GLFW_INCLUDE_GLU
#define GLFW_INCLUDE_GL3
#define GLFW_INCLUDE_GLEXT
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <GL/glew.h>    // для поддержки расширений, шейдеров и так далее
#include <GLFW/glfw3.h> // Непосредственно сам GLFW
#include <glm.hpp>      // библиотека графической математики
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "PngLoader.h"
#include "Helpers.h"
#include "Vertex.h"
#include "Figures.h"
#include "Shaders.h"
#include "ObjLoader.h"
#include "Object.h"

// Документация
// https://www.opengl.org/sdk/docs/man/html/

using namespace std;
using namespace glm;

#define MATH_PI 3.14159265

// Текущие переменные для модели
bool leftButtonPressed = false;
bool rightPressed = false;
double lastCursorPosX = 0.0;
double lastCursorPosY = 0.0;
// Сферическая камера
float cameraYaw = 0.0f;   // горизонтальный угол
float cameraPitch = 0.0f; // вертикальный угол
float cameraDistance = 5.0f;

// текущий выбранный объект для фокусировки камеры: 0 - Солнце и т.д.
int selectedObject = 0;

void glfwErrorCallback(int error, const char *description)
{
    printf("OpenGL error = %d\n description = %s\n\n", error, description);
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // Выходим по нажатию Escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
    }
    // по пробелу включаем или выключаем вращение автоматом
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
    }
    // цифры 1..9 для выбора планеты для фокусировки камеры
    if (action == GLFW_PRESS)
    {
        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9)
        {
            selectedObject = key - GLFW_KEY_1; // 0..8
        }
    }
}

void glfwMouseButtonCallback(GLFWwindow *window, int button, int state, int mod)
{
    // обработка левой кнопки
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        if (state == GLFW_PRESS)
        {
            leftButtonPressed = true;
            // сохраняем позицию курсора при нажатии, чтобы потом вычислять приращение
            glfwGetCursorPos(window, &lastCursorPosX, &lastCursorPosY);
        }
        else
        {
            leftButtonPressed = false;
        }
    }
    // обработка правой кнопки
    if (button == GLFW_MOUSE_BUTTON_2)
    {
        if (state == GLFW_PRESS)
        {
            rightPressed = true;
            glfwGetCursorPos(window, &lastCursorPosX, &lastCursorPosY);
        }
        else
        {
            rightPressed = false;
        }
    }
}

void glfwCursorCallback(GLFWwindow *window, double x, double y)
{
    // приращение от последнего положения курсора
    double dx = x - lastCursorPosX;
    double dy = y - lastCursorPosY;

    // скорость вращения камеры и её зум
    const float rotSpeed = 0.005f;
    const float zoomSpeed = 0.02f;

    // ЛКМ - вращение камеры вокруг цели при движении по X и Y
    if (leftButtonPressed)
    {
        cameraYaw += static_cast<float>(dx) * rotSpeed;
        cameraPitch += static_cast<float>(dy) * rotSpeed;
        // необходимо ограничить вертикальный угол
        const float limit = glm::radians(89.0f);
        if (cameraPitch > limit)
            cameraPitch = limit;
        if (cameraPitch < -limit)
            cameraPitch = -limit;
    }

    // ПКМ - зум камеры при движении по Y
    if (rightPressed)
    {
        cameraDistance += static_cast<float>(dy) * zoomSpeed;
        if (cameraDistance < 1.0f)
            cameraDistance = 1.0f;
        if (cameraDistance > 1000.0f)
            cameraDistance = 1000.0f;
    }

    lastCursorPosX = x;
    lastCursorPosY = y;
}

void glfwScrollCallback(GLFWwindow *window, double scrollByX, double scrollByY)
{
}

int main(int argc, char *argv[])
{

    // окно
    GLFWwindow *window = 0;

    // обработчик ошибок
    glfwSetErrorCallback(glfwErrorCallback);

    // инициализация GLFW
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // создание окна
#ifdef __APPLE__
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(1024, 768, "Солнечная система", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // вертикальная синхронизация
    CHECK_GL_ERRORS();

    // Обработка клавиш и прочего
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, glfwCursorCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);

    // инициализация расширений
    glewExperimental = GL_TRUE;
    glewInit();
    CHECK_GL_ERRORS();

    // Инициализация отладки
    if (glDebugMessageCallback)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        // Коллбек ошибок OpenGL
        glDebugMessageCallback((GLDEBUGPROC)glDebugOut, 0);

        // Более высокий уровень отладки
        // GLuint unusedIds = 0;
        // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
    }
    CHECK_GL_ERRORS();

    const unsigned char *version = glGetString(GL_VERSION);
    printf("OpenGL version = %s\n", version);

    // оотношение сторон
    int width = 0;
    int height = 0;
    // Размер буффера кадра
    glfwGetFramebufferSize(window, &width, &height);
    CHECK_GL_ERRORS();

    // задаем отображение
    glViewport(0, 0, width, height);
    CHECK_GL_ERRORS();

    // Шейдеры
    GLuint shaderProgram = createShader();
    GLuint lineShader = createLineShader();
    CHECK_GL_ERRORS();

    // аттрибуты вершин шейдера
    int posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    int colorAttribLocation = glGetAttribLocation(shaderProgram, "aColor");
    int texCoordAttribLocation = glGetAttribLocation(shaderProgram, "aTexCoord");
    int normalAttribLocation = glGetAttribLocation(shaderProgram, "aNormal");
    // аттрибуты для шейдера линий
    int posAttribLocationLine = glGetAttribLocation(lineShader, "aPos");
    int colorAttribLocationLine = glGetAttribLocation(lineShader, "aColor");
    int mvpLineLocation = glGetUniformLocation(lineShader, "uModelViewProjMat");
    CHECK_GL_ERRORS();

    // юниформы шейдера
    int modelViewProjMatrixLocation = glGetUniformLocation(shaderProgram, "uModelViewProjMat");
    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    int modelMatLoc = glGetUniformLocation(shaderProgram, "uModelMat");
    int specularEnabledLoc = glGetUniformLocation(shaderProgram, "uSpecularEnabled");
    int emissiveEnabledLoc = glGetUniformLocation(shaderProgram, "uEmissiveEnabled");
    int emissiveColorLoc = glGetUniformLocation(shaderProgram, "uEmissiveColor");
    CHECK_GL_ERRORS();

    // VBO, данные о вершинах
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, triangleVertexCount * sizeof(Vertex), triangleVertexes, GL_STATIC_DRAW); // Треугольник
    // glBufferData(GL_ARRAY_BUFFER, squareVertexCount * sizeof(Vertex), squareVertexes, GL_STATIC_DRAW); // Квадрат
    // glBufferData(GL_ARRAY_BUFFER, cubeVertexCount * sizeof(Vertex), cubeVertexes, GL_STATIC_DRAW); // Куб

    vector<Vertex> sphereVertexes;
    loadObjModel("res/sphere.obj", sphereVertexes);

    glBufferData(GL_ARRAY_BUFFER,
                 sphereVertexes.size() * sizeof(Vertex),
                 &sphereVertexes[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // VBO для орбит планет
    GLuint orbitPosVBO = 0;
    GLuint orbitColorVBO = 0;
    glGenBuffers(1, &orbitPosVBO);
    glGenBuffers(1, &orbitColorVBO);
    CHECK_GL_ERRORS();

    // отключаем отображение задней части полигонов
    // glEnable(GL_CULL_FACE);
    // отбрасываться будут задние грани
    glCullFace(GL_BACK);
    // Определяем, в каком направлении должный обходиться вершины, для передней части (против часовой стрелки?)
    // задняя часть будет отбрасываться
    glFrontFace(GL_CCW);
    CHECK_GL_ERRORS();

    // проверка глубины
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    CHECK_GL_ERRORS();

    // текущее время
    double time = glfwGetTime();

    // солнце
    Object sun(vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, "res/Sun.png", false);

    // структура планет
    struct PlanetDesc
    {
        float dist;
        float tilt;
        float incl;
        float rotSpeed;
        float scale;
        const char *tex;
        vec3 emissive;
    };

    vector<PlanetDesc> planetDesc = {
        {2.0f, 0.0f, 25.0f, 2.5f, 0.1f, "res/Mercury.png", vec3(0.3f, 0.3f, 0.3f)},
        {4.0f, 177.0f, 12.0f, -0.2f, 0.275f, "res/Venus.png", vec3(0.3f, 0.3f, 0.3f)},
        {6.0f, 23.0f, 6.0f, 1.0f, 0.3f, "res/Earth.png", vec3(0.3f, 0.3f, 0.3f)},
        {9.0f, 25.0f, 18.0f, 0.7f, 0.25f, "res/Mars.png", vec3(0.3f, 0.3f, 0.3f)},
        {24.0f, 3.0f, 8.0f, 0.3f, 1.25f, "res/Jupiter.png", vec3(0.3f, 0.3f, 0.3f)},
        {40.0f, 26.0f, 12.0f, 0.25f, 1.1f, "res/Saturn.png", vec3(0.3f, 0.3f, 0.3f)},
        {64.0f, 97.0f, 20.0f, 0.2f, 0.6f, "res/Uranus.png", vec3(0.3f, 0.3f, 0.3f)},
        {80.0f, 28.0f, 14.0f, 0.1f, 0.6f, "res/Neptune.png", vec3(0.3f, 0.3f, 0.3f)}};

    // создаем объекты планет по описанию
    vector<Object> planets;
    planets.reserve(planetDesc.size());
    for (const auto &d : planetDesc)
    {
        planets.emplace_back(d.dist, d.tilt, d.incl, d.rotSpeed, d.scale, d.tex, true, d.emissive);
    }

    double initTime = glfwGetTime();
    for (auto &p : planets)
    {
        p.orbitalPhase = static_cast<float>(-initTime * p.orbitalSpeed);
    }

    for (auto &p : planets)
    {
        if (p.orbitalDistance != 0.0f)
        {
            float inclRad = glm::radians(p.orbitalInclinationDeg);
            glm::mat4 invIncl = glm::rotate(glm::mat4(1.0f), -inclRad, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::vec3 base = glm::vec3(invIncl * glm::vec4(p.position, 1.0f));
            float angle = atan2f(base.z, base.x);
            p.orbitalPhase = angle - static_cast<float>(initTime * p.orbitalSpeed);
        }
    }

    // скайбокс
    Object sky(vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f, 1000.0f, "res/Stars.png", false);

    // свечение для солнца - включаем и задаем цвет
    sun.emissiveEnabled = true;
    sun.emissiveColor = vec3(2.0f, 2.0f, 2.0f);

    while (!glfwWindowShouldClose(window))
    {
        // приращение времени
        double newTime = glfwGetTime();
        // double timeDelta = newTime - time;
        time = newTime;

        // wipe the drawing surface clear
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // матрица модель-вид-проекция
        mat4 modelViewProjMatrix = mat4(1.0f);

        mat4 modelMatrix = mat4(1.0f);
        mat4 viewMatrix = mat4(1.0f);
        mat4 projectionMatrix = mat4(1.0f);

        // Модель - поворачиваем куб
        // modelMatrix = glm::rotate(modelMatrix, static_cast<float>(time * 2.0f), glm::vec3(0.1f, 0.1f, 0.1f));

        modelMatrix = glm::rotate(modelMatrix,
                                  glm::radians(23.5f),
                                  glm::vec3(0.0f, 0.0f, 1.0f));

        modelMatrix = glm::rotate(modelMatrix,
                                  static_cast<float>(time * 0.5f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));

        // вид - вычисляем цель обзора (центр внимания) и позицию камеры вокруг этой цели
        vec3 lookTarget = vec3(0.0f);
        if (selectedObject >= 0)
        {
            if (selectedObject == 0)
            {
                lookTarget = sun.position;
            }
            else
            {
                int idx = selectedObject - 1;
                if (idx >= 0 && idx < static_cast<int>(planets.size()))
                {
                    const Object &p = planets[idx];
                    if (p.orbitalDistance != 0.0f)
                    {
                        float orbitAngle = static_cast<float>(time * p.orbitalSpeed + p.orbitalPhase);
                        vec3 basePos = vec3(p.orbitalDistance * cosf(orbitAngle), 0.0f, p.orbitalDistance * sinf(orbitAngle));
                        float inclRad = glm::radians(p.orbitalInclinationDeg);
                        mat4 inclRot = glm::rotate(mat4(1.0f), inclRad, vec3(1.0f, 0.0f, 0.0f));
                        lookTarget = vec3(inclRot * vec4(basePos, 1.0f));
                    }
                    else
                    {
                        lookTarget = p.position;
                    }
                }
            }
        }

        // для вычисления позиции камеры вокруг цели используем сферические координаты, так камера будет следовать за движущимися целями
        vec3 offset;
        offset.x = cameraDistance * sinf(cameraYaw) * cosf(cameraPitch);
        offset.y = cameraDistance * sinf(cameraPitch);
        offset.z = cameraDistance * cosf(cameraYaw) * cosf(cameraPitch);
        vec3 camPos = lookTarget + offset;

        viewMatrix = glm::lookAt(camPos, lookTarget, vec3(0.0f, 1.0f, 0.0f));

        // Проекция - создаем перспективу
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

        // Итоговая матрица
        modelViewProjMatrix = projectionMatrix * viewMatrix * modelMatrix;

        // выставляем матрицу трансформации в пространство OpenGL
        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, false, glm::value_ptr(modelViewProjMatrix));

        // sizeof(Vertex) - размер блока данных о вершине
        // OFFSETOF(Vertex, color) - смещение от начала
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Позиции
        if (posAttribLocation >= 0)
        {
            glEnableVertexAttribArray(posAttribLocation);
            glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, pos));
        }

        // Цвет вершин
        if (colorAttribLocation >= 0)
        {
            glEnableVertexAttribArray(colorAttribLocation);
            glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, color));
        }

        // Координаты текстур
        if (texCoordAttribLocation >= 0)
        {
            glEnableVertexAttribArray(texCoordAttribLocation);
            glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, texCoord));
        }

        // Нормали (для бликов)
        if (normalAttribLocation >= 0)
        {
            glEnableVertexAttribArray(normalAttribLocation);
            glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, normal));
        }

        CHECK_GL_ERRORS();

        // отрисовка скайбокса
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        sky.position = camPos;
        sky.scale = 1000.0f;
        sky.specularEnabled = false;
        sky.emissiveEnabled = true;
        sky.emissiveColor = vec3(1.0f, 1.0f, 1.0f);
        sky.render(shaderProgram, modelViewProjMatrixLocation, modelMatLoc, viewPosLoc, specularEnabledLoc, emissiveEnabledLoc, emissiveColorLoc, viewMatrix, projectionMatrix, camPos, time, static_cast<GLuint>(sphereVertexes.size()));
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);

        vec3 cameraPos = camPos;

        glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
        sun.render(shaderProgram, modelViewProjMatrixLocation, modelMatLoc, viewPosLoc, specularEnabledLoc, emissiveEnabledLoc, emissiveColorLoc, viewMatrix, projectionMatrix, cameraPos, time, static_cast<GLuint>(sphereVertexes.size()));

        // орбиты
        const int orbitSegments = 128;
        for (size_t pi = 0; pi < planetDesc.size(); ++pi)
        {
            float dist = planetDesc[pi].dist;
            float inclDeg = planetDesc[pi].incl;
            std::vector<glm::vec3> orbitPoints;
            std::vector<glm::vec3> orbitColors;
            orbitPoints.reserve(orbitSegments);
            orbitColors.reserve(orbitSegments);
            float inclRad = glm::radians(inclDeg);
            glm::mat4 inclRot = glm::rotate(glm::mat4(1.0f), inclRad, glm::vec3(1.0f, 0.0f, 0.0f));
            for (int s = 0; s < orbitSegments; ++s)
            {
                float ang = static_cast<float>(s) / static_cast<float>(orbitSegments) * 2.0f * MATH_PI;
                glm::vec3 basePos = glm::vec3(dist * cosf(ang), 0.0f, dist * sinf(ang));
                glm::vec3 pnt = glm::vec3(inclRot * glm::vec4(basePos, 1.0f));
                orbitPoints.push_back(pnt);
                orbitColors.push_back(glm::vec3(0.6f, 0.6f, 0.6f));
            }

            // обновление данных орбит в VBO для отрисовки линий (чтобы не трогать основные атрибуты шейдера, которые используются для планет)
            glUseProgram(lineShader);
            glm::mat4 orbitMVP = projectionMatrix * viewMatrix * glm::mat4(1.0f);
            if (mvpLineLocation >= 0)
                glUniformMatrix4fv(mvpLineLocation, 1, GL_FALSE, glm::value_ptr(orbitMVP));

            // позиции
            glBindBuffer(GL_ARRAY_BUFFER, orbitPosVBO);
            glBufferData(GL_ARRAY_BUFFER, orbitPoints.size() * sizeof(glm::vec3), &orbitPoints[0], GL_DYNAMIC_DRAW);
            if (posAttribLocationLine >= 0)
            {
                glEnableVertexAttribArray(posAttribLocationLine);
                glVertexAttribPointer(posAttribLocationLine, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }

            // цвета
            glBindBuffer(GL_ARRAY_BUFFER, orbitColorVBO);
            glBufferData(GL_ARRAY_BUFFER, orbitColors.size() * sizeof(glm::vec3), &orbitColors[0], GL_DYNAMIC_DRAW);
            if (colorAttribLocationLine >= 0)
            {
                glEnableVertexAttribArray(colorAttribLocationLine);
                glVertexAttribPointer(colorAttribLocationLine, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }

            glLineWidth(1.0f);
            glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(orbitSegments));

            if (posAttribLocationLine >= 0)
                glDisableVertexAttribArray(posAttribLocationLine);
            if (colorAttribLocationLine >= 0)
                glDisableVertexAttribArray(colorAttribLocationLine);

            // изменение шейдера для отрисовки планет обратно на основной, так как line shader использовался для отрисовки орбит и он изменил состояние атрибутов
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glUseProgram(shaderProgram);
        }

        // после отрисовки орбит необходимо заново выставить указатели атрибутов для основного шейдера
        // так как шейдер для линий изменил состояние атрибутов (те же индексы)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (posAttribLocation >= 0)
        {
            glEnableVertexAttribArray(posAttribLocation);
            glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, pos));
        }
        if (colorAttribLocation >= 0)
        {
            glEnableVertexAttribArray(colorAttribLocation);
            glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, color));
        }
        if (texCoordAttribLocation >= 0)
        {
            glEnableVertexAttribArray(texCoordAttribLocation);
            glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, texCoord));
        }
        if (normalAttribLocation >= 0)
        {
            glEnableVertexAttribArray(normalAttribLocation);
            glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, normal));
        }

        // после отрисовки орбит необходимо заново выставить указатели атрибутов для основного шейдера
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (posAttribLocation >= 0)
        {
            glEnableVertexAttribArray(posAttribLocation);
            glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, pos));
        }
        if (colorAttribLocation >= 0)
        {
            glEnableVertexAttribArray(colorAttribLocation);
            glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, color));
        }
        if (texCoordAttribLocation >= 0)
        {
            glEnableVertexAttribArray(texCoordAttribLocation);
            glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, texCoord));
        }
        if (normalAttribLocation >= 0)
        {
            glEnableVertexAttribArray(normalAttribLocation);
            glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSETOF(Vertex, normal));
        }

        // для каждой планеты вычисляем позицию на орбите по её параметрам и текущему времени, а также вращение вокруг своей оси, и отрисовываем её
        for (const auto &p : planets)
        {
            glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
            p.render(shaderProgram, modelViewProjMatrixLocation, modelMatLoc, viewPosLoc, specularEnabledLoc, emissiveEnabledLoc, emissiveColorLoc, viewMatrix, projectionMatrix, cameraPos, time, static_cast<GLuint>(sphereVertexes.size()));
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteProgram(lineShader);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &orbitPosVBO);
    glDeleteBuffers(1, &orbitColorVBO);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
