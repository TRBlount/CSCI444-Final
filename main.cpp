/*
 *  CSCI 444, Advanced Computer Graphics, Spring 2019
 *
 *  Project: final
 *  File: main.cpp
 *
 *  Description:
 *      Implements basic PBR
 *
 *  Author:
 *      Tyler Blount
 *  
 *  Notes:
 *
 */

//*************************************************************************************

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdlib.h>
#include <stdio.h>

#include <deque>

#include <CSCI441/OpenGLUtils3.hpp>
#include <CSCI441/ShaderUtils3.hpp>
#include <CSCI441/ShaderProgram3.hpp>
#include <CSCI441/modelLoader3.hpp>
#include <CSCI441/objects3.hpp>

#include <SOIL/SOIL.h>

//*************************************************************************************

// Structure definitions

struct Vertex {
	GLfloat px, py, pz;	// point location x,y,z
	GLfloat nx, ny, nz;	// normal x, y, z
    GLfloat u, v;
};

// specify our Cube Vertex information
Vertex cubeVertices[] = {
		{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // brn
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // tln
		{  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // bln
        
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // brn
        { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // trn
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // tln
        
        {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // bln
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // tln
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // blf
        
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // blf
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // tln
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // tlf
        
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // tln
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // trf
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // tlf
        
        { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // trn
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // trf
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // tln
        
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // brn
		{  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // bln
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // brf
        
        {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // bln
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // blf
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // brf
        
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // brf
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // blf
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // tlf
        
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // brf
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // tlf
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // trf
        
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // brn
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // brf
        { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // trn
        
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // brf
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // trf
        { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // trn
};

// specify our Cube Index Ordering
const GLushort cubeIndices[] = {
		0, 2, 1, 0, 3, 2, // near
		1, 2, 5, 5, 2, 6, // right
		2, 7, 6, 3, 7, 2, // top
		0, 1, 4, 1, 5, 4, // bottom
		4, 5, 6, 4, 6, 7, // back
		0, 4, 3, 4, 7, 3  // left
};

struct TextureMaterial {
    GLuint diffMap;
    GLuint specMap;
};

struct character_info {
	GLfloat ax; // advance.x
	GLfloat ay; // advance.y

	GLfloat bw; // bitmap.width;
	GLfloat bh; // bitmap.rows;

	GLfloat bl; // bitmap_left;
	GLfloat bt; // bitmap_top;

	GLfloat tx; // x offset of glyph in texture coordinates
} font_characters[128];

//*************************************************************************************
//
// Global Parameters

GLint windowWidth, windowHeight;
bool shiftDown = false;
bool leftMouseDown = false;
glm::vec2 mousePosition( -9999.0f, -9999.0f );

glm::vec3 cameraAngles( 0.0f, 1.570796f, 15.0f );
glm::vec3 eyePoint(   0.0f, 0.0f, 15.0f );
glm::vec3 lookAtPoint( 0.0f,  0.0f,  0.0f );
glm::vec3 upVector(    0.0f,  1.0f,  0.0f );

CSCI441::ShaderProgram *colorShaderProgram = NULL;

CSCI441::ShaderProgram *skyBoxShaderProgram = NULL;

struct SkyboxShaderUniformLocations {
	GLint eyePosition;
	GLint lookAtPoint;
	GLint upVector;
	GLint aspectRatio;
} skyboxShaderUniformLocs;

GLuint cubeMapTextureHandle;

CSCI441::ShaderProgram *reflectionShaderProgram = NULL;

struct ReflectShaderUniformLocations {
	GLint mvpMatrix;
	GLint modelMatrix;
	GLint eyePosition;
} reflectShaderUniformLocs;

CSCI441::ShaderProgram *modelShaderProgram = NULL;

struct ModelShaderUniformLocations {
    GLint mMtx;
    GLint mvpMtx;
    GLint eyePos;
    GLint lightDir;
    GLint lightColor;
    GLint attenuation;
    GLint ambient;
    GLint diffMap;
    GLint specMap;
    GLint cubeMap;
} modelShaderUniformLocs;

TextureMaterial *metal, *paint, *rubber, *activeMat;

const GLuint CUBE = 0, SKYBOX = 2;
GLuint vaods[3];

struct ShaderAttributeLocations {
	GLint position;
	GLint normal;
    GLint texCoord;
} shaderAttribLocs;

FT_Face face;
GLuint font_texture_handle, text_vao_handle, text_vbo_handle;
GLint atlas_width, atlas_height;

CSCI441::ShaderProgram *textShaderProgram = nullptr;

struct TextShaderUniformLocations {
	GLint text_color_location;
	GLint text_mvp_location;
} textShaderUniformLocs;

struct TextShaderAttributeLocations {
	GLint text_texCoord_location;
} textShaderAttribLocs;

bool mackHack = false;

CSCI441::ModelLoader *cubeModel;

//*************************************************************************************

// Helper Funcs

void convertSphericalToCartesian() {
	eyePoint.x = cameraAngles.z * sinf( cameraAngles.x ) * sinf( cameraAngles.y );
	eyePoint.y = cameraAngles.z * -cosf( cameraAngles.y );
	eyePoint.z = cameraAngles.z * -cosf( cameraAngles.x ) * sinf( cameraAngles.y );
}

//*************************************************************************************

// GLFW Event Callbacks

// print errors from GLFW
static void error_callback(int error, const char* description) {
	fprintf(stderr, "[ERROR]: %s\n", description);
}

// handle key events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == 'Q') && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		shiftDown = true;
	else if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		shiftDown = false;
    else if(key == GLFW_KEY_M && action == GLFW_RELEASE) {
        if (activeMat == metal)
            activeMat = rubber;
        else
            activeMat = metal;
    }
}

// handle mouse clicks
static void mouseClick_callback(GLFWwindow* window, int button, int action, int mods) {
	if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ) {
		leftMouseDown = true;
	} else {
		leftMouseDown = false;
		mousePosition.x = -9999.0f;
		mousePosition.y = -9999.0f;
	}
}

// handle mouse positions
static void mousePos_callback(GLFWwindow* window, double xpos, double ypos) {
	// make sure movement is in bounds of the window
	// glfw captures mouse movement on entire screen
	if( xpos > 0 && xpos < windowWidth ) {
		if( ypos > 0 && ypos < windowHeight ) {
			// active motion
			if( leftMouseDown ) {
				if( (mousePosition.x - -9999.0f) < 0.001f ) {
					mousePosition.x = xpos;
					mousePosition.y = ypos;
				} else {
					if( !shiftDown ) {
						cameraAngles.x += (xpos - mousePosition.x)*0.005f;
						cameraAngles.y += (ypos - mousePosition.y)*0.005f;

						if( cameraAngles.y < 0 ) cameraAngles.y = 0.0f + 0.001f;
						if( cameraAngles.y >= M_PI ) cameraAngles.y = M_PI - 0.001f;
					} else {
						double totChgSq = (xpos - mousePosition.x) + (ypos - mousePosition.y);
						cameraAngles.z += totChgSq*0.01f;

						if( cameraAngles.z <= 2.0f ) cameraAngles.z = 2.0f;
						if( cameraAngles.z >= 50.0f ) cameraAngles.z = 50.0f;
					}
					convertSphericalToCartesian();


					mousePosition.x = xpos;
					mousePosition.y = ypos;
				}
			}
				// passive motion
			else {

			}
		}
	}
}

// handle scroll events
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset ) {
	GLdouble totChgSq = yOffset;
	cameraAngles.z += totChgSq*0.01f;

	if( cameraAngles.z <= 2.0f ) cameraAngles.z = 2.0f;
	if( cameraAngles.z >= 50.0f ) cameraAngles.z = 50.0f;

	convertSphericalToCartesian();
}

//*************************************************************************************

// Setup Funcs

// setup GLFW
GLFWwindow* setupGLFW() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
		exit(EXIT_FAILURE);
	}

	// create a 4.1 Core OpenGL Context
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Physically Based Rendering", NULL, NULL);
	if( !window ) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// register callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouseClick_callback);
	glfwSetCursorPosCallback(window, mousePos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// return our window
	return window;
}

// setup OpenGL parameters
void setupOpenGL() {
	glEnable( GL_DEPTH_TEST );							// turn on depth testing
	glDepthFunc( GL_LESS );								// use less than test
	glFrontFace( GL_CCW );								// front faces are CCW
	glEnable(GL_BLEND);									// turn on alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// blend w/ 1-a
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );				// clear our screen to black

	// initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewResult = glewInit();

	// check for an error
	if( glewResult != GLEW_OK ) {
		printf( "[ERROR]: Error initalizing GLEW\n");
		exit(EXIT_FAILURE);
	}

	// print information about our current OpenGL set up
	CSCI441::OpenGLUtils::printOpenGLInfo();
}

// load our shaders and get locations for uniforms and attributes
void setupShaders() {
	skyBoxShaderProgram = new CSCI441::ShaderProgram( "shaders/skybox.v.glsl",
													  "shaders/skybox.g.glsl",
													  "shaders/skybox.f.glsl" );

	// query all of uniform locations
	skyboxShaderUniformLocs.eyePosition   = skyBoxShaderProgram->getUniformLocation("eyePosition");
	skyboxShaderUniformLocs.lookAtPoint   = skyBoxShaderProgram->getUniformLocation("lookAtPoint");
	skyboxShaderUniformLocs.upVector      = skyBoxShaderProgram->getUniformLocation("upVector");
	skyboxShaderUniformLocs.aspectRatio   = skyBoxShaderProgram->getUniformLocation("aspectRatio");
    
    modelShaderProgram = new CSCI441::ShaderProgram( "shaders/model.v.glsl", "shaders/model.f.glsl" );
	
    modelShaderUniformLocs.mMtx          = modelShaderProgram->getUniformLocation( "modelMatrix" );
    modelShaderUniformLocs.eyePos         = modelShaderProgram->getUniformLocation( "eyePos" );
    modelShaderUniformLocs.mvpMtx         = modelShaderProgram->getUniformLocation( "modelViewProjMatrix" );
    modelShaderUniformLocs.lightDir       = modelShaderProgram->getUniformLocation( "lightDir" );
    modelShaderUniformLocs.lightColor     = modelShaderProgram->getUniformLocation( "lightColor" );
    modelShaderUniformLocs.attenuation    = modelShaderProgram->getUniformLocation( "attenuation" );
    modelShaderUniformLocs.ambient        = modelShaderProgram->getUniformLocation( "ambientFactor" );
    modelShaderUniformLocs.diffMap        = modelShaderProgram->getUniformLocation( "sDiffMap" );
    modelShaderUniformLocs.specMap        = modelShaderProgram->getUniformLocation( "sSpecMap" );
    modelShaderUniformLocs.cubeMap        = modelShaderProgram->getUniformLocation( "sCubeMap" );
    
    shaderAttribLocs.position   = modelShaderProgram->getAttributeLocation( "vPos" );
    shaderAttribLocs.normal     = modelShaderProgram->getAttributeLocation( "vNormal" );
    shaderAttribLocs.texCoord     = modelShaderProgram->getAttributeLocation( "vTexCoord" );

	reflectionShaderProgram = new CSCI441::ShaderProgram( "shaders/reflect.v.glsl",
														  "shaders/reflect.f.glsl" );
    
	reflectShaderUniformLocs.mvpMatrix = reflectionShaderProgram->getUniformLocation("MVP_Matrix");
	reflectShaderUniformLocs.modelMatrix = reflectionShaderProgram->getUniformLocation("model_Matrix");
	reflectShaderUniformLocs.eyePosition = reflectionShaderProgram->getUniformLocation("eyePosition");
    reflectionShaderProgram->getUniformLocation("cubeMap");

	textShaderProgram        = new CSCI441::ShaderProgram( "shaders/textShaderv410.v.glsl",
														   "shaders/textShaderv410.f.glsl" );
	textShaderUniformLocs.text_color_location      = textShaderProgram->getUniformLocation( "color" );
	textShaderUniformLocs.text_mvp_location        = textShaderProgram->getUniformLocation( "MVP_Matrix" );
	textShaderAttribLocs.text_texCoord_location    = textShaderProgram->getAttributeLocation( "coord" );
}

// load in our model data to VAOs and VBOs
void setupBuffers() {
	// generate our vertex array object descriptors
	glGenVertexArrays( 3, vaods );

	// will be used to stroe VBO descriptors for ARRAY_BUFFER and ELEMENT_ARRAY_BUFFER
	GLuint vbods[2];

	//------------ BEGIN CUBE VAO ------------

	// bind our Cube VAO
	glBindVertexArray( vaods[CUBE] );

	// generate our vertex buffer object descriptors for the CUBE
	glGenBuffers( 2, vbods );
	// bind the VBO for our Cube Array Buffer
	glBindBuffer( GL_ARRAY_BUFFER, vbods[0] );
	// send the data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW );

	// enable our position attribute
	glEnableVertexAttribArray( shaderAttribLocs.position );
	// map the position attribute to data within our buffer
	glVertexAttribPointer( shaderAttribLocs.position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) 0 );

	// enable our normal attribute
	glEnableVertexAttribArray( shaderAttribLocs.normal );
	// map the normal attribute to data within our buffer
	glVertexAttribPointer( shaderAttribLocs.normal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3*sizeof(float)) );
    // enable our texCoord attribute
	glEnableVertexAttribArray( shaderAttribLocs.texCoord );
	// map the normal attribute to data within our buffer
	glVertexAttribPointer( shaderAttribLocs.texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6*sizeof(float)) );

	//------------  END  CUBE VAO ------------

	//------------ BEGIN SKYBOX VAO ------------

	glm::vec3 skyboxPoint = glm::vec3(0.0, 0.0, 0.0);

	// Draw Ground
	glBindVertexArray( vaods[SKYBOX] );

	// generate our vertex buffer object descriptors for the GROUND
	glGenBuffers( 2, vbods );
	// bind the VBO for our Ground Array Buffer
	glBindBuffer( GL_ARRAY_BUFFER, vbods[0] );
	// send the data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(skyboxPoint), &skyboxPoint[0], GL_STATIC_DRAW );

	// enable our position attribute
	glEnableVertexAttribArray( shaderAttribLocs.position );
	// map the position attribute to data within our buffer
	glVertexAttribPointer( shaderAttribLocs.position, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

	//------------  END  SKYBOX VAO------------
    cubeModel = new CSCI441::ModelLoader("objects/cube.obj");
}

void setupNormals() {
    // Cube Normals
    for (int i = 0; i < 36; i += 3) {
        // Calculate the normals
        glm::vec3 v01 = glm::vec3(cubeVertices[i+1].px - cubeVertices[i].px,
                                  cubeVertices[i+1].py - cubeVertices[i].py,
                                  cubeVertices[i+1].pz - cubeVertices[i].pz);
        glm::vec3 v02 = glm::vec3(cubeVertices[i+2].px - cubeVertices[i].px,
                                  cubeVertices[i+2].py - cubeVertices[i].py,
                                  cubeVertices[i+2].pz - cubeVertices[i].pz);
        glm::vec3 normal = glm::normalize(glm::cross(v01, v02));
        // Set the normals
        cubeVertices[i].nx = normal.x;
        cubeVertices[i].ny = normal.y;
        cubeVertices[i].nz = normal.z;
        cubeVertices[i+1].nx = normal.x;
        cubeVertices[i+1].ny = normal.y;
        cubeVertices[i+1].nz = normal.z;
        cubeVertices[i+2].nx = normal.x;
        cubeVertices[i+2].ny = normal.y;
        cubeVertices[i+2].nz = normal.z;
    }
}

void setupTextures() {
    // Setup cubemap
    glGenTextures(1, &cubeMapTextureHandle);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureHandle);

    int w, h, c;
    unsigned char* nxd = SOIL_load_image("textures/colosseum/negx.jpg", &w, &h, &c, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nxd);
    unsigned char* pxd = SOIL_load_image("textures/colosseum/posx.jpg", &w, &h, &c, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pxd);
    unsigned char* nyd = SOIL_load_image("textures/colosseum/negy.jpg", &w, &h, &c, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nyd);
    unsigned char* pyd = SOIL_load_image("textures/colosseum/posy.jpg", &w, &h, &c, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pyd);
    unsigned char* nzd = SOIL_load_image("textures/colosseum/negz.jpg", &w, &h, &c, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nzd);
    unsigned char* pzd = SOIL_load_image("textures/colosseum/posz.jpg", &w, &h, &c, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pzd);
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    GLenum err;
    while ((err = glGetError()) = GL_NO_ERROR) {
        if (err == GL_INVALID_OPERATION) {
            fprintf(stderr, "[ERROR]: Attempt to create mipmaps for cubemap failed!\n");
        } else {
            fprintf(stderr, "[ERROR]: Gl Error %X found.\n", err);
        }
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 7);
    
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
    // Parse cube textures
    metal = new TextureMaterial();
    metal->diffMap = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/metal-diff.png" );
    metal->specMap = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/metal-spec.dds" );
    rubber = new TextureMaterial();
    rubber->diffMap = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/rubber-diff.png" );
    rubber->specMap = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/rubber-spec.dds" );
    activeMat = metal;
}

void setupFonts() {
	FT_Library ft;

	if(FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		exit(EXIT_FAILURE);
	}

	if(FT_New_Face(ft, "fonts/DroidSansMono.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		exit(EXIT_FAILURE);
	}

	FT_Set_Pixel_Sizes(face, 0, 20);

	FT_GlyphSlot g = face->glyph;
	GLuint w = 0;
	GLuint h = 0;

	for(int i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		w += g->bitmap.width;
		h = (h > g->bitmap.rows ? h : g->bitmap.rows);
	}

	/* you might as well save this value as it is needed later on */
	atlas_width = w;
	atlas_height = h;

	glEnable( GL_TEXTURE_2D );
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &font_texture_handle);
	glBindTexture(GL_TEXTURE_2D, font_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	GLint x = 0;

	for(int i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER))
			continue;

		font_characters[i].ax = g->advance.x >> 6;
		font_characters[i].ay = g->advance.y >> 6;

		font_characters[i].bw = g->bitmap.width;
		font_characters[i].bh = g->bitmap.rows;

		font_characters[i].bl = g->bitmap_left;
		font_characters[i].bt = g->bitmap_top;

		font_characters[i].tx = (float)x / w;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		x += g->bitmap.width;
	}

	glGenVertexArrays(1, &text_vao_handle);
	glBindVertexArray(text_vao_handle);

	glGenBuffers(1, &text_vbo_handle);
	glBindBuffer(GL_ARRAY_BUFFER, text_vbo_handle);
	glEnableVertexAttribArray(textShaderAttribLocs.text_texCoord_location);
	glVertexAttribPointer(textShaderAttribLocs.text_texCoord_location, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

//*************************************************************************************

// Rendering

void render_text(const char *text, FT_Face face, float x, float y, float sx, float sy) {
	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	} coords[6 * strlen(text)];

	GLint n = 0;

	for(const char *p = text; *p; p++) {
		int characterIndex = (int)*p;

		character_info character = font_characters[characterIndex];

		GLfloat x2 =  x + character.bl * sx;
		GLfloat y2 = -y - character.bt * sy;
		GLfloat w = character.bw * sx;
		GLfloat h = character.bh * sy;

		/* Advance the cursor to the start of the next character */
		x += character.ax * sx;
		y += character.ay * sy;

		/* Skip glyphs that have no pixels */
		if(!w || !h)
			continue;

		coords[n++] = (point){x2,     -y2    , character.tx,                                0};
		coords[n++] = (point){x2 + w, -y2    , character.tx + character.bw / atlas_width,   0};
		coords[n++] = (point){x2,     -y2 - h, character.tx,                                character.bh / atlas_height}; //remember: each glyph occupies a different amount of vertical space

		coords[n++] = (point){x2 + w, -y2    , character.tx + character.bw / atlas_width,   0};
		coords[n++] = (point){x2,     -y2 - h, character.tx,                                character.bh / atlas_height};
		coords[n++] = (point){x2 + w, -y2 - h, character.tx + character.bw / atlas_width,   character.bh / atlas_height};
	}

	glBindVertexArray(text_vao_handle);
	glBindBuffer(GL_ARRAY_BUFFER, text_vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof( coords ), coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, n);
}

// handles drawing everything to our buffer
void renderScene( GLFWwindow *window ) {
	GLdouble time = glfwGetTime();

	// query our current window size, determine the aspect ratio, and set our viewport size
	GLfloat ratio;
	ratio = windowWidth / (GLfloat) windowHeight;
	glViewport(0, 0, windowWidth, windowHeight);

	// create our Model, View, Projection matrices
	glm::mat4 mMtx, vMtx, pMtx;
	mMtx = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

	// compute our projection matrix
	pMtx = glm::perspective( 45.0f, ratio, 0.001f, 100.0f );
	// compute our view matrix based on our current camera setup
	vMtx = glm::lookAt( eyePoint,lookAtPoint, upVector );

	// precompute the modelview matrix
	glm::mat4 mvMtx = vMtx * mMtx;
	// precompute the modelviewprojection matrix
	glm::mat4 mvpMtx = pMtx * mvMtx;

	glDisable(GL_DEPTH_TEST);
	skyBoxShaderProgram->useProgram();
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureHandle);
	glUniform3fv( skyboxShaderUniformLocs.eyePosition, 1, &eyePoint[0] );
	glUniform3fv( skyboxShaderUniformLocs.lookAtPoint, 1, &lookAtPoint[0] );
	glUniform3fv( skyboxShaderUniformLocs.upVector, 1, &upVector[0] );
	glUniform1f( skyboxShaderUniformLocs.aspectRatio, ratio );

	glBindVertexArray( vaods[SKYBOX] );
	glDrawArrays( GL_POINTS, 0, 1 );
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_CUBE_MAP);

	modelShaderProgram->useProgram();
    
    // Set light uniforms
	glm::vec3 ambientFactor = glm::vec3(0.05, 0.05, 0.05);
	glUniform3fv(modelShaderUniformLocs.ambient, 1, &ambientFactor[0]);
    glm::vec3 lightDir = glm::normalize(glm::vec3(0.75f,0.75f,-0.5f));
    glUniform3fv(modelShaderUniformLocs.lightDir, 1, &lightDir[0]);
    glm::vec4 lightColor = glm::vec4(1,0.95,0.8,1);
    glUniform4fv(modelShaderUniformLocs.lightColor, 1, &lightColor[0]);
    glUniform1f(modelShaderUniformLocs.attenuation, 1.0f);
    glUniform3fv(modelShaderUniformLocs.eyePos, 1, &eyePoint[0]);
    
    mMtx = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);//glm::rotate( mMtx, (GLfloat)time, glm::vec3( sin(time), cos(time), sin(time)-cos(time) ) );
	// scale the cube up
	mMtx = glm::scale( mMtx, glm::vec3(5,5,5));

	// update our modelview matrix
	mvMtx = vMtx * mMtx;
	// update our modelviewprojection matrix
	mvpMtx = pMtx * mvMtx;
    
    glUniformMatrix4fv( modelShaderUniformLocs.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
	glUniformMatrix4fv( modelShaderUniformLocs.mMtx, 1, GL_FALSE, &mMtx[0][0] );
    
    glEnable(GL_TEXTURE_2D);
    
    // Set texture uniforms
    glUniform1i(modelShaderUniformLocs.diffMap, 0);
    glUniform1i(modelShaderUniformLocs.specMap, 1);
    glUniform1i(modelShaderUniformLocs.cubeMap, 2);
    
    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, activeMat->diffMap);
    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture( GL_TEXTURE_2D, activeMat->specMap);
    
    glEnable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0+2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureHandle);
    
	// bind our Cube VAO
	//glBindVertexArray( vaods[CUBE] );
	// draw our cube!
	//glDrawArrays( GL_TRIANGLES, 0, sizeof( cubeVertices ) / sizeof( Vertex ) );
    //cubeModel->draw(shaderAttribLocs.position, shaderAttribLocs.normal, shaderAttribLocs.texCoord);
    CSCI441::setVertexAttributeLocations(shaderAttribLocs.position, shaderAttribLocs.normal, shaderAttribLocs.texCoord);
    CSCI441::drawSolidSphere(0.5, 32, 32);
    glDisable(GL_TEXTURE_CUBE_MAP);
}

// program entry point
int main( int argc, char *argv[] ) {
	GLFWwindow *window = setupGLFW();	// setup GLFW and get our window
	setupOpenGL();						// setup OpenGL & GLEW
	setupShaders();						// load our shader programs, uniforms, and attribtues
    setupNormals();                     // compute the normals for the cube
	setupBuffers();						// load our models into GPU memory
	setupTextures();                    // load textures
	setupFonts();						// load our fonts into memory

	convertSphericalToCartesian();		// position our camera in a pretty place

	GLdouble lastTime = glfwGetTime();
	GLuint nbFrames = 0;
	GLdouble fps = 0;
	std::deque<GLdouble> fpsAvgs(9);
	GLdouble fpsAvg = 0;

	// as long as our window is open
	while( !glfwWindowShouldClose(window) ) {
		// clear the prior contents of our buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

		// render our scene
		renderScene( window );

		// Measure speed
		GLdouble currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 0.33f ){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			fps = GLdouble(nbFrames)/(currentTime - lastTime);
			nbFrames = 0;
			lastTime = currentTime;

			fpsAvgs.pop_front();
			fpsAvgs.push_back( fps );

			GLdouble totalFPS = 0;
			for( auto fpsUnit : fpsAvgs ) {
				totalFPS += fpsUnit;
			}
			fpsAvg = totalFPS / fpsAvgs.size();
		}

		glBindVertexArray(text_vao_handle);
		glBindTexture(GL_TEXTURE_2D, font_texture_handle);

		textShaderProgram->useProgram();
		//glUniform1i(textShaderUniformLocs.text_texture_location, GL_TEXTURE0);

		glm::mat4 mvp = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

		glUniformMatrix4fv(textShaderUniformLocs.text_mvp_location, 1, GL_FALSE, &mvp[0][0]);

		GLfloat white[4] = {1, 1, 1, 1};
		glUniform4fv(textShaderUniformLocs.text_color_location, 1, white);

		GLfloat sx = 2.0f / (GLfloat)windowWidth;
		GLfloat sy = 2.0f / (GLfloat)windowHeight;

		char fpsStr[80];
		sprintf( fpsStr, "%.3f frames/sec (Avg: %.3f)", fps, fpsAvg);
		render_text(fpsStr, face, -1 + 8 * sx,   1 - 30 * sy, sx, sy);

		// swap the front and back buffers
		glfwSwapBuffers(window);
		// check for any events
		glfwPollEvents();

		// the following code is a hack for OSX Mojave
		// the window is initially black until it is moved
		// so instead of having the user manually move the window,
		// we'll automatically move it and then move it back
		if( !mackHack ) {
			GLint xpos, ypos;
			glfwGetWindowPos(window, &xpos, &ypos);
			glfwSetWindowPos(window, xpos+10, ypos+10);
			glfwSetWindowPos(window, xpos, ypos);
			mackHack = true;
		}
	}

	// destroy our window
	glfwDestroyWindow(window);
	// end GLFW
	glfwTerminate();

	// delete our shader program
    delete skyBoxShaderProgram;
    delete modelShaderProgram;
    delete reflectionShaderProgram;
	// SUCCESS!!
	return EXIT_SUCCESS;
}
