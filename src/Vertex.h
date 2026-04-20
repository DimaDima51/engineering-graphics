#ifndef VERTEX_H
#define VERTEX_H

#include <glm.hpp>

using namespace glm;

struct Vertex {
    vec3 pos;
    vec3 color;
    vec2 texCoord;
    vec3 normal;

    Vertex(vec3 inPos, vec3 inColor, vec2 inTexCoord, vec3 inNormal);
};

#endif
