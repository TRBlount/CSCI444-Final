/*
 *  CSCI 444, Advanced Computer Graphics, Spring 2019
 *
 *  Project: Final
 *  File: model.v.glsl
 *
 *  Description:
 *
 *      Vertex shader for PBR
 *
 *  Author:
 *      Tyler Blount
 *
 *  Notes:
 *
 */
 
#version 410 core

layout(location=0) in vec3 vPos;      // vertex position
layout(location=1) in vec3 vNormal;   // vertex normal
layout(location=2) in vec2 vTexCoord; // vertex texcoord

uniform mat4 modelViewProjMatrix;
uniform mat4 modelMatrix;
uniform vec3 eyePos;

uniform vec3 ambientFactor;
uniform vec3 lightDir;
uniform vec4 lightColor;
uniform float attenuation;

uniform sampler2D sDiffMap;
uniform sampler2D sSpecMap;
uniform samplerCube sCubeMap;

out VertexOutput {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vertOut;

void main()
{
    vertOut.position = (modelMatrix * vec4(vPos,1.0)).xyz;
    vertOut.normal = (mat3(transpose(inverse(modelMatrix))) * vNormal).xyz;
    vertOut.texCoord = vTexCoord;
    gl_Position = modelViewProjMatrix * vec4(vPos,1.0);
}