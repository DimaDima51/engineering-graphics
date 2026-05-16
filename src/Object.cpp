#include "Object.h"
#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include "Helpers.h"
#include "PngLoader.h"
#include <string>

Object::Object()
    : position(vec3(0.0f)), rotationAxis(vec3(0.0f, 1.0f, 0.0f)), rotationSpeed(0.0f), scale(1.0f), textureId(0), specularEnabled(true)
{
    ownsTexture = false;
    emissiveEnabled = false;
    emissiveColor = vec3(0.0f);
    orbitalDistance = 0.0f;
    orbitalInclinationDeg = 0.0f;
    orbitalSpeed = 0.0f;
    orbitalPhase = 0.0f;
}

Object::Object(const vec3 &pos, const vec3 &rotAxis, float rotSpeed, float s, const char *textureFile, bool spec)
    : position(pos), rotationAxis(rotAxis), rotationSpeed(rotSpeed), scale(s), textureId(0), specularEnabled(spec), ownsTexture(false)
{
    orbitalDistance = 0.0f;
    orbitalInclinationDeg = 0.0f;
    orbitalSpeed = 0.0f;
    orbitalPhase = 0.0f;
    ImageData img = loadPngImage(textureFile);
    if (img.loaded)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, img.withAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img.data);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ownsTexture = true;
    }
}

// для вычисления вектора оси вращения из угла наклона в градусах
static vec3 computeAxisFromTilt(float tiltDeg)
{
    float r = glm::radians(tiltDeg);
    vec3 a = vec3(sinf(r), cosf(r), 0.0f);
    return normalize(a);
}

// конструктор для объектов с орбитальными параметрами
static vec3 computePosFromDistanceIncl(float distance, float inclDeg)
{
    // вычисляем позицию на орбите в плоскости XZ для угла 0 (т.е. на +X)
    // затем вращаем эту позицию вокруг X на угол наклона, чтобы получить начальную позицию в 3D пространстве
    float inclRad = glm::radians(inclDeg);
    glm::mat4 inclRot = glm::rotate(glm::mat4(1.0f), inclRad, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 basePos = glm::vec3(distance, 0.0f, 0.0f); // angle = 0 => +X
    return glm::vec3(inclRot * glm::vec4(basePos, 1.0f));
}

Object::Object(float distance, float tiltDeg, float inclDeg, float rotSpeed, float s, const char *textureFile, bool spec, const vec3 &emissive)
    : position(vec3(0.0f)), rotationAxis(vec3(0.0f, 1.0f, 0.0f)), rotationSpeed(rotSpeed), scale(s), textureId(0), specularEnabled(spec), ownsTexture(false)
{
    // хранение параметров орбиты для вычисления позиции в render
    orbitalDistance = distance;
    orbitalInclinationDeg = inclDeg;
    orbitalPhase = 0.0f;
    // простая формула для орбитальной скорости (обратная пропорциональность расстоянию для визуального распределения планет)
    orbitalSpeed = (distance != 0.0f) ? (0.5f / distance) : 0.0f;

    // вычисление начальной позиции и оси вращения на основе параметров орбиты
    position = computePosFromDistanceIncl(distance, inclDeg);
    rotationAxis = computeAxisFromTilt(tiltDeg);

    ImageData img = loadPngImage(textureFile);
    if (img.loaded)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, img.withAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img.data);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ownsTexture = true;
    }

    // включить эмиссив, если цвет не черный (0,0,0)
    emissiveEnabled = (emissive.x != 0.0f || emissive.y != 0.0f || emissive.z != 0.0f);
    emissiveColor = emissive;
}

Object::~Object()
{
    if (ownsTexture && textureId != 0)
    {
        GLuint id = textureId;
        glDeleteTextures(1, &id);
        textureId = 0;
    }
}

void Object::render(GLuint shaderProgram,
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
                    GLuint vertexCount) const
{
    mat4 model = mat4(1.0f);

    // если у объекта есть параметры орбиты, вычисляем позицию на основе этих параметров, иначе используем явную позицию
    vec3 worldPos;
    if (orbitalDistance != 0.0f)
    {
        float orbitAngle = static_cast<float>(time * orbitalSpeed + orbitalPhase);
        vec3 basePos = vec3(orbitalDistance * cosf(orbitAngle), 0.0f, orbitalDistance * sinf(orbitAngle));
        float inclRad = glm::radians(orbitalInclinationDeg);
        mat4 inclRot = glm::rotate(mat4(1.0f), inclRad, vec3(1.0f, 0.0f, 0.0f));
        worldPos = vec3(inclRot * vec4(basePos, 1.0f));
    }
    else
    {
        worldPos = position;
    }

    // сначала перемещаем модель в позицию на орбите, затем вращаем вокруг своей оси и масштабируем
    float angle = static_cast<float>(time * rotationSpeed);
    model = translate(model, worldPos);
    model = rotate(model, angle, rotationAxis);
    model = glm::scale(model, vec3(scale));

    mat4 mvp = projMat * viewMat * model;

    glUniformMatrix4fv(modelViewProjLoc, 1, GL_FALSE, value_ptr(mvp));
    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(model));

    glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

    // для планет с отключенным specularEnabled блики не будут вычисляться, так как в шейдере это условие используется для отключения specular компоненты
    glUniform1f(specularLoc, specularEnabled ? 1.0f : 0.0f);

    glUniform1f(emissiveEnabledLoc, emissiveEnabled ? 1.0f : 0.0f);
    glUniform3f(emissiveColorLoc, emissiveColor.x, emissiveColor.y, emissiveColor.z);

    // биндим текстуру, если она есть
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    int texLoc = glGetUniformLocation(shaderProgram, "uTexture");
    if (texLoc >= 0)
        glUniform1i(texLoc, 0);

    // отрисовка
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
