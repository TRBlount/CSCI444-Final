/*
 *  CSCI 444, Advanced Computer Graphics, Spring 2019
 *
 *  Project: Final
 *  File: model.f.glsl
 *
 *  Description:
 *
 *      Fragment shader for PBR with Fresnel and reflections
 *
 *  Author:
 *      Tyler Blount
 *
 *  Notes:
 *
 */

#version 410 core

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

in VertexOutput {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vertIn;

const float SRGB_GAMMA = 2.2;
const float SRGB_GAMMA_INVERSE = 1.0 / SRGB_GAMMA;

float srgb_to_linear(float val) {
	return pow(val, SRGB_GAMMA);
}
vec2 srgb_to_linear(vec2 val) {
	return pow(val, vec2(SRGB_GAMMA));
}
vec3 srgb_to_linear(vec3 val) {
	return pow(val, vec3(SRGB_GAMMA));
}
vec4 srgb_to_linear(vec4 val) {
	return pow(val, vec4(SRGB_GAMMA));
}

float linear_to_srgb(float val) {
	return pow(val, SRGB_GAMMA_INVERSE);
}
vec2 linear_to_srgb(vec2 val) {
	return pow(val, vec2(SRGB_GAMMA_INVERSE));
}
vec3 linear_to_srgb(vec3 val) {
	return pow(val, vec3(SRGB_GAMMA_INVERSE));
}
vec4 linear_to_srgb(vec4 val) {
	return pow(val, vec4(SRGB_GAMMA_INVERSE));
}

const float PI = 3.14159;

vec3 FresnelSchlick(vec3 specColor, vec3 light, vec3 halfVec)
{
	return specColor + (vec3(1.0) - specColor) * pow(1.0 - clamp(dot(light, halfVec), 0.0, 1.0), 5.0);
}

vec3 FresnelLazarovEnv(vec3 specColor, vec3 view, vec3 normal, float gloss)
{
	// Fresnel for environment lighting 
	// Equation referenced from Dimitar Lazarov's presentation titled Physically Based Rendering in Call of Duty: Black Ops
	return specColor + (vec3(1.0) - specColor) * pow(1.0 - clamp(dot(view, normal), 0.0, 1.0), 5.0) / (4.0 - 3.0 * gloss);
}

vec3 SpecularGGX(vec3 specColor, vec3 light, vec3 normal, vec3 halfVec, vec3 view, float gloss, float fresnelFactor, float dotNL)
{
	float roughness = clamp(1.0 - gloss, 0.0, 1.0);
	float alpha = roughness * roughness;

	float dotNH = clamp(dot(normal, halfVec), 0.0, 1.0);
	float dotNV = clamp(dot(normal, view), 0.0, 1.0);

	float alphaSqr = alpha * alpha;
	float denom = dotNH * dotNH * (alphaSqr - 1.0) + 1.0;
	float distribution = alphaSqr / (PI * denom * denom);

	vec3 fresnel = mix(specColor, FresnelSchlick(specColor, light, halfVec), fresnelFactor);

	float alphaPrime = roughness + 1.0;
	float k = alphaPrime * alphaPrime / 8.0;
	float g1vNL = 1.0 / (dotNL * (1.0 - k) + k);
	float g1vNV = 1.0 / (dotNV * (1.0 - k) + k);
	float visibility = g1vNL * g1vNV;

	return distribution * fresnel * visibility * dotNL;
}

vec3 computeLighting(vec3 specColor, vec3 light, vec3 normal, vec3 halfVec, vec3 view, float gloss, float fresnelFactor, float dotNL) 
{
	return SpecularGGX(specColor, light, normal, halfVec, view, gloss, fresnelFactor, dotNL);
}

vec3 CalculateLighting(vec3 normal, vec3 diffuseMaterial, vec3 specularMaterial, float gloss)
{
	vec3 eyeDir = vec3(normalize(eyePos - vertIn.position).xyz);
	vec3 lightAmbient = ambientFactor;
	vec3 lightDiffuse = vec3(0.0, 0.0, 0.0);
	vec3 lightSpecular = vec3(0.0, 0.0, 0.0);
    vec3 halfVec = normalize(lightDir + eyeDir);
    float NdotL = clamp(dot(normal, lightDir), 0.0, 1.0);
    // Ambient, Diffuse, and Specular
    lightDiffuse += lightColor.rgb * NdotL * attenuation;
    lightSpecular += lightColor.rgb * computeLighting(specularMaterial, lightDir, normal, halfVec, eyeDir, gloss, 1.0, NdotL) * attenuation;

	return diffuseMaterial * (lightAmbient + lightDiffuse) + lightSpecular;
}

out vec4 fragOut;

void main() {
    
    vec3 I = normalize(eyePos - vertIn.position).xyz;
    vec3 N = normalize(vertIn.normal);
    vec3 R = normalize(reflect(-I, N));
    
    vec4 baseColor = vec4(1.0);
    vec4 diffColor = srgb_to_linear(texture(sDiffMap, vertIn.texCoord));
    vec4 specColor = texture(sSpecMap, vertIn.texCoord);
    specColor.rgb = srgb_to_linear(specColor.rgb);
    baseColor.rgb = CalculateLighting(N, diffColor.rgb, specColor.rgb, specColor.a);
    float mip = (1-specColor.a)*7.0;
    vec4 envColor = textureLod(sCubeMap, R, mip);
    baseColor.rgb += envColor.rgb * FresnelLazarovEnv(specColor.rgb, I, N, specColor.a);
    fragOut = linear_to_srgb(baseColor);
}