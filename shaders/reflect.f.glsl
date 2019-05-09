/*
 *  CSCI 444, Advanced Computer Graphics, Spring 2019
 *
 *  Project: lab2
 *  File: colorPassThrough.f.glsl
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

// ***** FRAGMENT SHADER UNIFORMS *****
uniform samplerCube cubeMap;
uniform vec3 eyePosition;

// ***** FRAGMENT SHADER INPUT *****
in vec3 position;
in vec3 normal;

// ***** FRAGMENT SHADER OUTPUT *****
out vec4 fragColorOut;

// ***** FRAGMENT SHADER SUBROUTINES *****


void main() {

    vec3 I = normalize(position - eyePosition);
    vec3 N = normalize(normal);
    vec3 R = normalize(reflect(I, N));

    vec4 envColor = textureLod(cubeMap, R, 2.5);
    
    fragColorOut=vec4(envColor.rgb,1);

	// if viewing the backside of the fragment,
	// reverse the colors as a visual cue
	if( !gl_FrontFacing ) {
		fragColorOut.rgb = fragColorOut.bgr;
	}
}
