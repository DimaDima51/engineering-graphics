#ifndef FIGURES_H
#define FIGURES_H

#include <glm.hpp>
#include "Vertex.h"

using namespace glm;

// данные о вершинах
/*
// ТРЕУГОЛЬНИК
int triangleVertexCount = 3;
Vertex triangleVertexes[] = {
    //                ВЕРШИНА                   ЦВЕТ
    Vertex(vec3( 0.0f,  1.0f, 0.0f), vec3(0.5f, 0.0f, 0.2f)),  // верх
    Vertex(vec3(-1.0f, -1.0f, 0.0f), vec3(0.0f, 0.5f, 0.2f)),  // лево
    Vertex(vec3( 1.0f, -1.0f, 0.0f), vec3(0.6f, 0.7f, 0.0f))   // право
};
*/

// КВАДРАТ
int squareVertexCount = 6;  // 2 треугольника = 6 вершин
Vertex squareVertexes[] = {
    // Первый треугольник (нижний левый)
    Vertex(vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),  // лево-низ (красный)
    Vertex(vec3( 1.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),  // право-низ (зеленый)
    Vertex(vec3(-1.0f,  1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),  // лево-верх (синий)
    
    // Второй треугольник (верхний правый)
    Vertex(vec3( 1.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),  // право-низ (зеленый)
    Vertex(vec3( 1.0f,  1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f)),  // право-верх (желтый)
    Vertex(vec3(-1.0f,  1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f))   // лево-верх (синий)
};

#endif
