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

/*
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
*/

// КУБ
int cubeVertexCount = 36;  // 6 граней * 2 треугольника * 3 вершины = 36 вершин
Vertex cubeVertexes[] = {
    // Передняя грань (Z = 1.0)
    Vertex(vec3(-1.0f, -1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),
    Vertex(vec3( 1.0f, -1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
    Vertex(vec3(-1.0f,  1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
    
    Vertex(vec3( 1.0f, -1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
    Vertex(vec3( 1.0f,  1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f)),
    Vertex(vec3(-1.0f,  1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
    
    // Задняя грань (Z = -1.0)
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f, 0.5f, 0.0f)),
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.0f)),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f)),
    
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.0f)),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.0f, 1.0f, 1.0f)),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f)),
    
    // Левая грань (X = -1.0)
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.5f)),
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.0f, 1.0f, 0.5f)),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f)),
    
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.0f, 1.0f, 0.5f)),
    Vertex(vec3(-1.0f,  1.0f,  1.0f), vec3(0.5f, 1.0f, 0.0f)),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.5f, 0.0f, 1.0f)),
    
    // Правая грань (X = 1.0)
    Vertex(vec3( 1.0f, -1.0f,  1.0f), vec3(1.0f, 0.5f, 0.5f)),
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.5f)),
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.5f, 0.5f, 1.0f)),
    
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.5f, 1.0f, 0.5f)),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f, 0.5f, 1.0f)),
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.5f, 0.5f, 1.0f)),
    
    // Нижняя грань (Y = -1.0)
    Vertex(vec3(-1.0f, -1.0f, -1.0f), vec3(0.8f, 0.2f, 0.2f)),
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.2f, 0.8f, 0.2f)),
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.2f, 0.2f, 0.8f)),
    
    Vertex(vec3( 1.0f, -1.0f, -1.0f), vec3(0.2f, 0.8f, 0.2f)),
    Vertex(vec3( 1.0f, -1.0f,  1.0f), vec3(0.8f, 0.8f, 0.2f)),
    Vertex(vec3(-1.0f, -1.0f,  1.0f), vec3(0.2f, 0.2f, 0.8f)),
    
    // Верхняя грань (Y = 1.0)
    Vertex(vec3(-1.0f,  1.0f,  1.0f), vec3(1.0f, 0.8f, 0.8f)),
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.8f, 1.0f, 0.8f)),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.8f, 0.8f, 1.0f)),
    
    Vertex(vec3( 1.0f,  1.0f,  1.0f), vec3(0.8f, 1.0f, 0.8f)),
    Vertex(vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f, 0.8f, 1.0f)),
    Vertex(vec3(-1.0f,  1.0f, -1.0f), vec3(0.8f, 0.8f, 1.0f))
};


#endif
