#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace glm;

class Object {
public:
    vec3 position;              // позиция в мире
    vec3 rotationAxis;          // ось вращения
    float rotationSpeed;        // угол поворота будет вычисляться как time * rotationSpeed
    float scale;                // масштаб
    GLuint textureId;           // OpenGL texture ID
    bool specularEnabled;       // для бликов
    bool ownsTexture;           // текстура была загружена этим объектом и должна быть удалена при уничтожении
    bool emissiveEnabled;       // для свечения
    glm::vec3 emissiveColor;    // для свечения

    // параметры орбиты для объектов, которые должны двигаться по орбите вокруг центра координат (например, планеты)
    float orbitalDistance;
    float orbitalInclinationDeg;
    float orbitalSpeed;
    float orbitalPhase;

    Object();

    // Конструктор объекта. Загружает текстуру из файла. Если загрузка не удалась, создается белая текстура.
    Object(const vec3 &pos, const vec3 &rotAxis, float rotSpeed, float s, const char* textureFile, bool spec);
    // Конструктор для объектов с орбитальными параметрами. Загружает текстуру из файла. Если загрузка не удалась, создается белая текстура.
    Object(float distance, float tiltDeg, float inclDeg, float rotSpeed, float s, const char* textureFile, bool spec, const vec3 &emissive = vec3(0.0f));

    // отрисовка объекта. Вызывающий код должен был уже привязать VBO и включить атрибуты вершин.
    void render(GLuint shaderProgram,
                int modelViewProjLoc,
                int modelMatLoc,
                int viewPosLoc,
                int specularLoc,
                int emissiveEnabledLoc,
                int emissiveColorLoc,
                const mat4 &viewMat,
                const mat4 &projMat,
                const vec3 &viewPos,
                double time,
                GLuint vertexCount) const;
    ~Object();
};

#endif
