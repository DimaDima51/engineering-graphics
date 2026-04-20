#ifndef FIGURES_H
#define FIGURES_H

#include <glm.hpp>
#include "Vertex.h"

using namespace glm;

int cubeVertexCount = 36;

// Определяем нормали для каждой грани
vec3 nFront  = vec3( 0.0f,  0.0f,  1.0f);
vec3 nBack   = vec3( 0.0f,  0.0f, -1.0f);
vec3 nLeft   = vec3(-1.0f,  0.0f,  0.0f);
vec3 nRight  = vec3( 1.0f,  0.0f,  0.0f);
vec3 nBottom = vec3( 0.0f, -1.0f,  0.0f);
vec3 nTop    = vec3( 0.0f,  1.0f,  0.0f);

Vertex cubeVertexes[] = {
    // Передняя грань (Z = 1.0)
    Vertex(vec3(-1.0f, -1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), nFront),
    Vertex(vec3( 1.0f, -1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), nFront),
    Vertex(vec3(-1.0f,  1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), nFront),
    
    Vertex(vec3( 1.0f, -1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), nFront),
    Vertex(vec3( 1.0f,  1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), nFront),
    Vertex(vec3(-1.0f,  1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), nFront),
    
    // Задняя грань (Z = -1.0)
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f, 0.5f, 0.0f), vec2(0.0f, 0.0f), nBack),
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.0f), vec2(1.0f, 0.0f), nBack),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f), vec2(0.0f, 1.0f), nBack),
    
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.0f), vec2(1.0f, 0.0f), nBack),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), nBack),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f), vec2(0.0f, 1.0f), nBack),
    
    // Левая грань (X = -1.0)
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.5f), vec2(0.0f, 0.0f), nLeft),
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.0f, 1.0f, 0.5f), vec2(1.0f, 0.0f), nLeft),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f), vec2(0.0f, 1.0f), nLeft),
    
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.0f, 1.0f, 0.5f), vec2(1.0f, 0.0f), nLeft),
    Vertex(vec3(-1.0f,  1.0f,  1.0f), vec3(0.5f, 1.0f, 0.0f), vec2(1.0f, 1.0f), nLeft),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f), vec2(0.0f, 1.0f), nLeft),
    
    // Правая грань (X = 1.0)
    Vertex(vec3( 1.0f, -1.0f,  1.0f), vec3(1.0f, 0.5f, 0.5f), vec2(0.0f, 0.0f), nRight),
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.5f), vec2(1.0f, 0.0f), nRight),
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.5f, 0.5f, 1.0f), vec2(0.0f, 1.0f), nRight),
    
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.5f), vec2(1.0f, 0.0f), nRight),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f, 0.5f, 1.0f), vec2(1.0f, 1.0f), nRight),
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.5f, 0.5f, 1.0f), vec2(0.0f, 1.0f), nRight),
    
    // Нижняя грань (Y = -1.0)
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(0.8f, 0.2f, 0.2f), vec2(0.0f, 0.0f), nBottom),
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.2f, 0.8f, 0.2f), vec2(1.0f, 0.0f), nBottom),
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.2f, 0.2f, 0.8f), vec2(0.0f, 1.0f), nBottom),
    
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.2f, 0.8f, 0.2f), vec2(1.0f, 0.0f), nBottom),
    Vertex(vec3( 1.0f, -1.0f,  1.0f), vec3(0.8f, 0.8f, 0.2f), vec2(1.0f, 1.0f), nBottom),
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.2f, 0.2f, 0.8f), vec2(0.0f, 1.0f), nBottom),
    
    // Верхняя грань (Y = 1.0)
    Vertex(vec3(-1.0f,  1.0f,  1.0f), vec3(1.0f, 0.8f, 0.8f), vec2(0.0f, 0.0f), nTop),
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.8f, 1.0f, 0.8f), vec2(1.0f, 0.0f), nTop),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.8f, 0.8f, 1.0f), vec2(0.0f, 1.0f), nTop),
    
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.8f, 1.0f, 0.8f), vec2(1.0f, 0.0f), nTop),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f, 0.8f, 1.0f), vec2(1.0f, 1.0f), nTop),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.8f, 0.8f, 1.0f), vec2(0.0f, 1.0f), nTop)
};

#endif
