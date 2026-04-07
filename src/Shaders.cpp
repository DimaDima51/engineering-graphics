#include "Shaders.h"
#include "Helpers.h"
#include <stdio.h>
#include <stdlib.h>


GLuint createShaderFromSources(const char* vertexShader, const char* fragmentShader){
    GLuint vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);
    CHECK_GL_ERRORS();

    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);
    CHECK_GL_ERRORS();

    GLuint shaderProgram = glCreateProgram ();
    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);
    CHECK_GL_ERRORS();

    /*int infologLength = 0;
    glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0){
        GLchar* infoLog = (GLchar*)malloc(static_cast<size_t>(infologLength));
        if (infoLog == nullptr){
            printf("ERROR: Could not allocate InfoLog buffer");
            exit(1);
        }
        int charsWritten = 0;
        glGetShaderInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
        printf("Shader InfoLog:\n%s", infoLog );
        free(infoLog);
    }
    CHECK_GL_ERRORS();*/
    
    return shaderProgram;
}

GLuint createShader(){
    // Шейдер вершин
    const char* vertexShader = STRINGIFY_SHADER(
        attribute vec3 aPos;
        attribute vec3 aColor;
        attribute vec2 aTexCoord;
        
        uniform mat4 uModelViewProjMat;
        
        varying vec3 vColor;
        varying vec2 vTexCoord;

        void main () {
            gl_Position = uModelViewProjMat * vec4(aPos, 1.0);
            vColor = aColor;
            vTexCoord = aTexCoord;
        }
    );

    // Фрагментный шейдер
   const char* fragmentShader = STRINGIFY_SHADER(
        precision mediump float;
        varying vec3 vColor;
        varying vec2 vTexCoord;
        
        uniform sampler2D uTexture;

        void main () {
            // Создаем квадратную дырку в центре (координаты от 0 до 1)
            // Центр текстурных координат = (0.5, 0.5)
            float holeMin = 0.35;  // левая/нижняя граница дырки
            float holeMax = 0.65;  // правая/верхняя граница дырки
            
            // Если пиксель внутри области дырки - отбрасываем его
            if (vTexCoord.x > holeMin && vTexCoord.x < holeMax && 
                vTexCoord.y > holeMin && vTexCoord.y < holeMax) {
                discard;  // это и есть "дырка"!
            }
            
            // Все остальные пиксели рисуем как обычно
            gl_FragColor = texture2D(uTexture, vTexCoord);
        }
    );

    GLuint shader = createShaderFromSources(vertexShader, fragmentShader);
    CHECK_GL_ERRORS();
    return shader;
}

