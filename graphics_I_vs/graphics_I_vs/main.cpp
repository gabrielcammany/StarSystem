/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)

MAIN:
+ This file creates the window and uses a simple render loop to paint a coloured quad
+ GLUT is a simple toolkit for create OpenGL applications. 
  To download glut go to http://alunevans.info/resources/freeglut.zip
  To install GLUT, unzip the copy the contents of the 'include', 'bin' and 'lib' directories to any valid 
  for your project e.g. default places for VS2015: 
    C:\Archivos de programa (x86)\Microsoft Visual Studio 14.0\VC\include
	C:\Archivos de programa (x86)\Microsoft Visual Studio 14.0\VC\bin
	C:\Archivos de programa (x86)\Microsoft Visual Studio 14.0\VC\lib
+ GLEW is a library that loads some useful OpenGL extensions.
  To download GLEW go to http://alunevans.info/resources/glew.zip  
  To install GLEW, unzip and copy the contents the same as GLUT
+ GLM is a maths library that provides us useful C++ classes for managing vectors and matrices
  To download GLM go here http://alunevans.info/resources/glm.zip 
  To install, unzip and copy the contents of the include directory to the include path. GLM
  is a 'header only' library which means it does not have any pre-compiled libraries
*/

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <cmath> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <string> 
#include <vector> 
#include <glm/gtc/matrix_inverse.hpp>
#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"
#include "imageloader.h"
#include "camera.h"
#define M_PI 3.1416
#include "Shader.h"

using namespace glm;

int g_ViewportWidth = 1280;
int g_ViewportHeight = 720;

float counterForEarth = 0.0f;
float counterForJupiter = 0.0f;
float counterForEarthAngle = 0.0f;
float counterForMoon = 0.0;

glm::vec2 oldMousePosition;
std::vector<tinyobj::shape_t> shapesSphere;
std::vector<tinyobj::shape_t> shapesSatellite;

float xForEarth = 0.0f;
float yForEarth = 0.0f;
float xForMoon = 0.0f;
float yForMoon = 0.0f;
float xForJupiter = 0.0f;
float yForJupiter = 0.0f;

const vec3 g_ColorFons(0.01f, 0.01f, 0.01f);
vec3 g_light_dir(-100, 0, 0);

std::string basepath = "..\\graphics_I_vs\\";

GLuint g_ShaderProgram = 0;
GLuint g_ShaderProgram_sky = 0;
GLuint g_ShaderProgram_moon = 0;

float g_light_distance = 1.0f;
float g_light_angle = 20.0f;

GLuint texture_id[7]; 
Image *image[7];

GLuint vaoSphere;
GLuint vaoSatellite;

GLuint numTrianglesSphere;
GLuint numTrianglesSatellite;

const float DEG2RAD = 3.14159 / 180.0;

Camera camera;

void creaGeometriaEsfera()
{

	std::vector<tinyobj::material_t> materials;
	std::string err;
	std::string inputfile = basepath + "sphere.obj";
	bool ret = tinyobj::LoadObj(shapesSphere, materials, err, inputfile.c_str(), basepath.c_str());

	if (!err.empty()) { 
		std::cerr << err << std::endl; 
	}
	std::cout << "# of shapes : " << shapesSphere.size() << std::endl;

	GLuint buffer;

	glGenVertexArrays(1, &vaoSphere);
	glBindVertexArray(vaoSphere);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, shapesSphere[0].mesh.texcoords.size() * sizeof(float), &(shapesSphere[0].mesh.texcoords[0]), GL_STATIC_DRAW);

	GLuint uvLoc  = glGetAttribLocation(g_ShaderProgram, "uvs");

	glEnableVertexAttribArray(uvLoc);
	glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint uvLoc_sky = glGetAttribLocation(g_ShaderProgram_sky, "uvs");
	glEnableVertexAttribArray(uvLoc_sky);
	glVertexAttribPointer(uvLoc_sky, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Create VBO por indices
	glGenBuffers(1, &buffer); //create more new buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); //bind them as element array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapesSphere[0].mesh.normals.size() * sizeof(unsigned), &(shapesSphere[0].mesh.normals[0]), GL_STATIC_DRAW); //fill buffer with index data

	GLuint aNormal  = glGetAttribLocation(g_ShaderProgram, "a_normal");
	glEnableVertexAttribArray(aNormal);
	glVertexAttribPointer(aNormal, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Crea VBO per vèrtexs
	glGenBuffers(1, &buffer); //glGenBuffers requires a reference
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, shapesSphere[0].mesh.positions.size() * sizeof(float), &(shapesSphere[0].mesh.positions[0]), GL_STATIC_DRAW); //fill it with out vertices

	// Activate shader attribute for this buffer
	GLuint vertexLoc = glGetAttribLocation(g_ShaderProgram, "vertexPos"); //find attribute vertexPos in shader
	glEnableVertexAttribArray(vertexLoc); //enable it
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //specify location and data format
	GLuint vertexLoc_sky = glGetAttribLocation(g_ShaderProgram_sky, "vertexPos"); //find attribute vertexPos in shader
	glEnableVertexAttribArray(vertexLoc_sky); //enable it
	glVertexAttribPointer(vertexLoc_sky, 3, GL_FLOAT, GL_FALSE, 0, 0); //specify location and data format

	// Create VBO por indices
	glGenBuffers(1, &buffer); //create more new buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); //bind them as element array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapesSphere[0].mesh.indices.size() * sizeof(unsigned), &(shapesSphere[0].mesh.indices[0]), GL_STATIC_DRAW); //fill buffer with index data

	numTrianglesSphere = shapesSphere[0].mesh.indices.size()/3;
		
	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
void creaGeometriaSatellite()
{
	std::vector<tinyobj::material_t> materials;
	std::string err;
	std::string inputfile = basepath + "teapot.obj";
	bool ret = tinyobj::LoadObj(shapesSatellite, materials, err, inputfile.c_str(), basepath.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	std::cout << "# of shapes : " << shapesSatellite.size() << std::endl;

	GLuint buffer;

	int i = 0;

		glGenVertexArrays(1, &vaoSatellite);
		glBindVertexArray(vaoSatellite);
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, shapesSatellite[i].mesh.texcoords.size() * sizeof(float), &(shapesSatellite[i].mesh.texcoords[0]), GL_STATIC_DRAW);

		GLuint uvLoc = glGetAttribLocation(g_ShaderProgram, "uvs");

		glEnableVertexAttribArray(uvLoc);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Create VBO por indices
		glGenBuffers(1, &buffer); //create more new buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); //bind them as element array
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapesSatellite[i].mesh.normals.size() * sizeof(unsigned), &(shapesSatellite[i].mesh.normals[0]), GL_STATIC_DRAW); //fill buffer with index data

		GLuint aNormal = glGetAttribLocation(g_ShaderProgram, "a_normal");
		glEnableVertexAttribArray(aNormal);
		glVertexAttribPointer(aNormal, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Crea VBO per vèrtexs
		glGenBuffers(1, &buffer); //glGenBuffers requires a reference
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, shapesSatellite[i].mesh.positions.size() * sizeof(float), &(shapesSatellite[i].mesh.positions[0]), GL_STATIC_DRAW); //fill it with out vertices

																																		// Activate shader attribute for this buffer
		GLuint vertexLoc = glGetAttribLocation(g_ShaderProgram, "vertexPos"); //find attribute vertexPos in shader
		glEnableVertexAttribArray(vertexLoc); //enable it
		glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //specify location and data format

																		   // Create VBO por indices
		glGenBuffers(1, &buffer); //create more new buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); //bind them as element array
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapesSatellite[i].mesh.indices.size() * sizeof(unsigned), &(shapesSatellite[i].mesh.indices[0]), GL_STATIC_DRAW); //fill buffer with index data


		numTrianglesSatellite = shapesSatellite[i].mesh.indices.size() / 3;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}*/

void print(int x, int y, int z, char *string)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, g_ViewportWidth, 0, g_ViewportHeight, 0, 1);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glLoadIdentity();

	glRasterPos2f(x, y);
	int len = (int)strlen(string);

	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

};

void loadResources()
{
	Shader simpleShader("shader.vert", "shader.frag");
	g_ShaderProgram = simpleShader.program;

	Shader simpleShader2("shader_sky.vert", "shader_sky.frag");
	g_ShaderProgram_sky = simpleShader2.program;

	Shader simpleShader3("shader_moon.vert", "shader_moon.frag");
	g_ShaderProgram_moon = simpleShader3.program;

	std::string Imageinputfile = basepath + "earthmap1k.bmp";
	image[0] = loadBMP(Imageinputfile.c_str());

	std::string Imageinputfile5 = basepath + "earthnormal.bmp";
	image[1] = loadBMP(Imageinputfile5.c_str());

	std::string Imageinputfile6 = basepath + "earthspec.bmp";
	image[2] = loadBMP(Imageinputfile6.c_str());

	std::string Imageinputfile2 = basepath + "milkyway.bmp";
	image[3] = loadBMP(Imageinputfile2.c_str());

	std::string Imageinputfile3 = basepath + "sunmap.bmp";
	image[4] = loadBMP(Imageinputfile3.c_str());

	std::string Imageinputfile4 = basepath + "moon.bmp";
	image[5] = loadBMP(Imageinputfile4.c_str());

	std::string Imageinputfile7 = basepath + "sky_lightblue_3000.bmp";
	image[6] = loadBMP(Imageinputfile7.c_str());

	for (int i = 0; i < 7; i++) {
		glGenTextures(1, &texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, //target
			0, //level, = 0 base, no mipmap
			GL_RGB, //the way opengl stores the data
			image[i]->width, //width
			image[i]->height, //height
			0, //border, must be 0!
			GL_RGB, //the format of the original data
			GL_UNSIGNED_BYTE, //type of data
			image[i]->pixels); // pointer to start of data
	}


	creaGeometriaEsfera();
	//creaGeometriaSatellite();
}

void onReshape(int w, int h)
{
	g_ViewportWidth = w;
	g_ViewportHeight = h;

	glViewport(0, 0, g_ViewportWidth, g_ViewportHeight);
}

void drawEarth() {

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_ShaderProgram);

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");
	GLuint u_model_light = glGetUniformLocation(g_ShaderProgram, "u_model_light");
	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");
	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");
	GLuint u_texture_normal = glGetUniformLocation(g_ShaderProgram, "u_texture_normal");
	GLuint u_texture_spec = glGetUniformLocation(g_ShaderProgram, "u_texture_spec");
	GLuint u_light_color = glGetUniformLocation(g_ShaderProgram, "u_light_color");
	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	GLuint u_shininess = glGetUniformLocation(g_ShaderProgram, "u_shininess");
	GLuint u_ambient = glGetUniformLocation(g_ShaderProgram, "u_ambient");
	GLuint u_normal_matrix = glGetUniformLocation(g_ShaderProgram, "u_normal_matrix");

	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());

	glm::mat4 projection_matrix = glm::perspective(
		60.0f, // Field of view
		(float)g_ViewportWidth / g_ViewportHeight, // Aspect ratio
		0.1f, // near plane (distance from camera)
		50.0f //far plane (distance from camera)
	);
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));


	glUniform3f(u_light_color, 1.0, 1.0, 1.0);
	glUniform3f(u_cam_pos, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);
	glUniform1f(u_shininess, 100.0);
	glUniform1f(u_ambient, 0.9);
	glUniform3f(u_light_dir, g_light_distance * (-(xForEarth)), g_light_distance, g_light_distance * (-(yForEarth)));

	if (counterForEarth>359.5f)
		counterForEarth = 0.00f;
	else
		counterForEarth += 0.01;//earth speed

	if (counterForEarthAngle>719.5f)
		counterForEarthAngle = 0.0f;
	else
		counterForEarthAngle += 0.001;//earth speed

	xForEarth = cos((counterForEarth)*DEG2RAD)*20.0f;
	yForEarth = sin((counterForEarth)*DEG2RAD)*20.0f;

	glm::mat4 matrix = glm::translate(mat4(), vec3(xForEarth, 0.0, yForEarth)) *
		glm::rotate(mat4(), counterForEarth, glm::vec3(0.0, 0.0, 1.0));

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(matrix));

	mat3 normal_matrix = inverseTranspose((mat3(matrix)));
	glUniformMatrix3fv(u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

	glm::mat4 matrix_light = glm::translate(mat4(), vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(u_model_light, 1, GL_FALSE, glm::value_ptr(matrix_light));

	glUniform1i(u_texture, 0);
	glUniform1i(u_texture_spec, 1);
	glUniform1i(u_texture_normal, 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id[2]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_id[1]);

	glBindVertexArray(vaoSphere);
	glDrawElements(GL_TRIANGLES, numTrianglesSphere * 3, GL_UNSIGNED_INT, 0);
}

/*
void drawSatellite() {

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_ShaderProgram);

		GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");
		GLuint u_model_light = glGetUniformLocation(g_ShaderProgram, "u_model_light");
		GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
		GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");
		GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
		GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");
		GLuint u_light_color = glGetUniformLocation(g_ShaderProgram, "u_light_color");
		GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
		GLuint u_shininess = glGetUniformLocation(g_ShaderProgram, "u_shininess");
		GLuint u_ambient = glGetUniformLocation(g_ShaderProgram, "u_ambient");
		GLuint u_normal_matrix = glGetUniformLocation(g_ShaderProgram, "u_normal_matrix");

		glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());

		glm::mat4 projection_matrix = glm::perspective(
			60.0f, // Field of view
			(float)g_ViewportWidth / g_ViewportHeight, // Aspect ratio
			0.1f, // near plane (distance from camera)
			50.0f //far plane (distance from camera)
		);
		glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));

		glUniform3f(u_light_color, 1.0, 1.0, 0.0);
		glUniform3f(u_cam_pos, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);
		glUniform1f(u_shininess, 10.0);
		glUniform1f(u_ambient, 0.2);
		glUniform3f(u_light_dir, g_light_distance * (-(xForEarth)), g_light_distance, g_light_distance * (-(yForEarth)));

		glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(mat4(1.0f)));

		mat3 normal_matrix = inverseTranspose((mat3((mat4(1.0f)))));
		glUniformMatrix3fv(u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

		glm::mat4 matrix2 = glm::translate(mat4(), vec3(-camera.getCameraPos().x, -camera.getCameraPos().y, -camera.getCameraPos().z));
		glUniformMatrix4fv(u_model_light, 1, GL_FALSE, glm::value_ptr(matrix2));

		glBindVertexArray(vaoSatellite);
		glDrawElements(GL_TRIANGLES, numTrianglesSatellite * 3, GL_UNSIGNED_INT, 0);
	
}
*/

void drawSun() {

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glUseProgram(g_ShaderProgram_sky);

	GLuint u_model = glGetUniformLocation(g_ShaderProgram_sky, "u_model");
	GLuint u_projection = glGetUniformLocation(g_ShaderProgram_sky, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram_sky, "u_view");
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram_sky, "u_texture");

	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());

	glm::mat4 projection_matrix = glm::perspective(
		60.0f, // Field of view
		(float)g_ViewportWidth / g_ViewportHeight, // Aspect ratio
		0.1f, // near plane (distance from camera)
		50.0f //far plane (distance from camera)
	);
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 myScalingMatrix = glm::scale(mat4(1.0f), vec3(3, 3, 3));
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * myScalingMatrix;
	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(matrix));

	glUniform1i(u_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id[4]);

	glBindVertexArray(vaoSphere);
	glDrawElements(GL_TRIANGLES, numTrianglesSphere * 3, GL_UNSIGNED_INT, 0);

}

void drawSkyBox() {

	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	glUseProgram(g_ShaderProgram_sky);

	GLuint u_model = glGetUniformLocation(g_ShaderProgram_sky, "u_model");
	GLuint u_projection = glGetUniformLocation(g_ShaderProgram_sky, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram_sky, "u_view");
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram_sky, "u_texture");

	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());

	glm::mat4 projection_matrix = glm::perspective(
		60.0f, // Field of view
		(float)g_ViewportWidth / g_ViewportHeight, // Aspect ratio
		0.1f, // near plane (distance from camera)
		50.0f //far plane (distance from camera)
	);
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 matrix = glm::translate(mat4(), camera.getCameraPos());
	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(matrix));

	glUniform1i(u_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id[3]);

	glBindVertexArray(vaoSphere);
	glDrawElements(GL_TRIANGLES, numTrianglesSphere * 3, GL_UNSIGNED_INT, 0);

}

void drawMoon() {

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_ShaderProgram_moon);

	GLuint u_model = glGetUniformLocation(g_ShaderProgram_moon, "u_model");
	GLuint u_model_light = glGetUniformLocation(g_ShaderProgram_moon, "u_model_light");
	GLuint u_projection = glGetUniformLocation(g_ShaderProgram_moon, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram_moon, "u_view");
	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram_moon, "u_light_dir");
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram_moon, "u_texture");
	GLuint u_texture_normal = glGetUniformLocation(g_ShaderProgram_moon, "u_texture_normal");
	GLuint u_texture_spec = glGetUniformLocation(g_ShaderProgram_moon, "u_texture_spec");
	GLuint u_light_color = glGetUniformLocation(g_ShaderProgram_moon, "u_light_color");
	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram_moon, "u_cam_pos");
	GLuint u_shininess = glGetUniformLocation(g_ShaderProgram_moon, "u_shininess");
	GLuint u_ambient = glGetUniformLocation(g_ShaderProgram_moon, "u_ambient");
	GLuint u_normal_matrix = glGetUniformLocation(g_ShaderProgram_moon, "u_normal_matrix");


	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());

	glm::mat4 projection_matrix = glm::perspective(
		60.0f, // Field of view
		(float)g_ViewportWidth / g_ViewportHeight, // Aspect ratio
		0.1f, // near plane (distance from camera)
		50.0f //far plane (distance from camera)
	);
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(u_light_color, 1.0, 1.0, 1.0);
	glUniform3f(u_cam_pos, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);
	glUniform1f(u_shininess, 100.0);
	glUniform1f(u_ambient, 0.9);

	if (counterForMoon>359.5)
		counterForMoon = 0.0;
	else
		counterForMoon += 0.1;//moon speed

	xForMoon = cos((counterForMoon)*DEG2RAD)*3.0f;
	yForMoon = sin((counterForMoon)*DEG2RAD)*3.0f;

	glm::mat4 myScalingMatrix = glm::scale(mat4(1.0f), vec3(0.3, 0.3, 0.3));
	glm::mat4 matrix = glm::translate(mat4(), vec3(xForEarth + xForMoon, 0.0, yForEarth + yForMoon)) * myScalingMatrix;
	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(matrix));

	mat3 normal_matrix = inverseTranspose((mat3(matrix)));
	glUniformMatrix3fv(u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

	glUniform3f(u_light_dir, g_light_distance * (-(xForEarth + xForMoon)), g_light_distance, g_light_distance * (-(yForEarth + yForMoon)));

	glUniform1i(u_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id[5]);

	glBindVertexArray(vaoSphere);
	glDrawElements(GL_TRIANGLES, numTrianglesSphere * 3, GL_UNSIGNED_INT, 0);

}


void drawJupiter() {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	glUseProgram(g_ShaderProgram_moon);
	GLuint u_model = glGetUniformLocation(g_ShaderProgram_moon, "u_model");
	GLuint u_model_light = glGetUniformLocation(g_ShaderProgram_moon, "u_model_light");
	GLuint u_projection = glGetUniformLocation(g_ShaderProgram_moon, "u_projection");
	GLuint u_transparency = glGetUniformLocation(g_ShaderProgram_moon, "u_transparency");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram_moon, "u_view");
	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram_moon, "u_light_dir");
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram_moon, "u_texture");
	GLuint u_texture_normal = glGetUniformLocation(g_ShaderProgram_moon, "u_texture_normal");
	GLuint u_texture_spec = glGetUniformLocation(g_ShaderProgram_moon, "u_texture_spec");
	GLuint u_light_color = glGetUniformLocation(g_ShaderProgram_moon, "u_light_color");
	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram_moon, "u_cam_pos");
	GLuint u_shininess = glGetUniformLocation(g_ShaderProgram_moon, "u_shininess");
	GLuint u_ambient = glGetUniformLocation(g_ShaderProgram_moon, "u_ambient");
	GLuint u_normal_matrix = glGetUniformLocation(g_ShaderProgram_moon, "u_normal_matrix");

	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());

	glm::mat4 projection_matrix = glm::perspective(
		60.0f, // Field of view
		(float)g_ViewportWidth / g_ViewportHeight, // Aspect ratio
		0.1f, // near plane (distance from camera)
		50.0f //far plane (distance from camera)
	);
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));


	glUniform3f(u_light_color, 1.0, 1.0, 1.0);
	glUniform3f(u_cam_pos, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);
	glUniform1f(u_shininess, 100.0);
	glUniform1f(u_ambient, 0.5);
	glUniform1f(u_transparency, 0.8);
	glUniform3f(u_light_dir, g_light_distance * (-(xForJupiter)), g_light_distance, g_light_distance * (-(yForJupiter)));

	if (counterForJupiter<0.5f)
		counterForJupiter = 359.5f;
	else
		counterForJupiter -= 0.01;//earth speed

	xForJupiter = cos((counterForJupiter)*DEG2RAD)*13.0f;
	yForJupiter = sin((counterForJupiter)*DEG2RAD)*13.0f;

	glm::mat4 myScalingMatrix = glm::scale(mat4(1.0f), vec3(0.5, 0.5, 0.5));
	glm::mat4 matrix = glm::translate(mat4(), vec3(xForJupiter, 0.0, yForJupiter)) * myScalingMatrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(matrix));

	mat3 normal_matrix = inverseTranspose((mat3(matrix)));
	glUniformMatrix3fv(u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

	glUniform1i(u_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id[6]);

	glBindVertexArray(vaoSphere);
	glDrawElements(GL_TRIANGLES, numTrianglesSphere * 3, GL_UNSIGNED_INT, 0);
}


void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSkyBox();

	drawEarth();

	drawSun();

	drawMoon();

	drawJupiter();



	//drawSatellite();

	if (camera.isLock()) {
		glColor3ub(255, 0, 0);
		print(40.0f, 50.0f, 0.4f, "[L] Lock: True");
	}
	else {
		glColor3ub(255, 0, 0);
		print(40.0f, 50.0f, 0.4f, "[L] Lock: False");
	}

	if (camera.isClipping()) {
		glColor3ub(0, 255, 0);
		print(40.0f, 80.0f, 0.4f, "[C] Clipping: False");
	}
	else {
		glColor3ub(0, 255, 0);
		print(40.0f, 80.0f, 0.4f, "[C] Clipping: True");
	}

	glBindVertexArray(0);
	glUseProgram(0);
	glutSwapBuffers(); 
	glutPostRedisplay();
}

void mouseUpdate(int x, int y) {
	glm::vec2 newMousePosition = glm::vec2(x, y);
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 10.0f) {
		oldMousePosition = newMousePosition;
		return;
	}
	else {
		camera.mouseUpdate(mouseDelta);
		if (camera.isLock()) {
			oldMousePosition = glm::vec2(g_ViewportWidth / 2, g_ViewportHeight / 2);
			glutWarpPointer(g_ViewportWidth / 2, g_ViewportHeight / 2);
		}
		else {
			oldMousePosition = newMousePosition;
		}
		onDisplay();
	}
}

void keyPressed(unsigned char key, int xx, int yy)
{
	switch (key)
	{
	case 'w':
	{
		camera.moveForward();
		break;
	}

	case 's':
	{
		camera.moveBackwards();
		break;
	}
	case 'a':
	{
		camera.moveLeft();
		break;
	}

	case 'd':
	{
		camera.moveRight();
		break;
	}
	case 'c':
	{
		camera.changeClipping();
		break;
	}
	case 'l':
	{
		camera.changeLock();
		break;
	}
	case 'r':
	{
		camera.moveUp();
		break;
	}
	case 'f':
	{
		camera.moveDown();
		break;
	}
	case 'z':
	{
		camera.addSpeed();
		break;
	}
	case 'x':
	{
		camera.lesSpeed();
		break;
	}
	case 'v':
	{
		camera.addRotationalSpeed();
		break;
	}
	case 'b':
	{
		camera.takeRotationalSpeed();
		break;
	}
	case 32:
	{
		glutWarpPointer(g_ViewportWidth / 2, g_ViewportHeight / 2);
		break;
	}
	default:
		break;
	}

}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv); 	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(g_ViewportWidth, g_ViewportHeight);
	glutCreateWindow("Hello graphics!");

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}
	
	glClearColor(g_ColorFons.x, g_ColorFons.y, g_ColorFons.z, 1.0f );

	loadResources();

	glutDisplayFunc(onDisplay);
	glutIdleFunc(onDisplay);
	glutKeyboardFunc(keyPressed);

	glutPassiveMotionFunc(mouseUpdate);

	glutMainLoop();

	return EXIT_SUCCESS;
}