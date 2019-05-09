/*
 *  CSCI 444, Advanced Computer Graphics, Spring 2019
 *
 *  Project: lab2
 *  File: reflect.v.glsl
 *
 *  Description:
 *      Lab1
 *
 *      Pass Through Shader to apply color
 *
 *  Author:
 *      Dr. Jeffrey Paone, Colorado School of Mines
 *
 *  Notes:
 *
 */

// we are using OpenGL 4.1 Core profile
#version 410 core

// ***** VERTEX SHADER UNIFORMS *****
uniform mat4 MVP_Matrix;
uniform mat4 model_Matrix;

// ***** VERTEX SHADER INPUT *****
layout(location=0) in vec3 vPos;     // vertex position
layout(location=1) in vec3 vNormal;   // vertex normal

// ***** VERTEX SHADER OUTPUT *****
out vec3 position;
out vec3 normal;
out vec2 texcoord;

// ***** VERTEX SHADER SUBROUTINES *****

void main() {
    
    position = vec3(model_Matrix * vec4(vPos,1));
    normal = normalize(mat3(transpose(inverse(model_Matrix))) * vNormal);
    
    // transform our vertex into clip space
    gl_Position = MVP_Matrix * vec4( vPos, 1);

}
