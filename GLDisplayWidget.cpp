#include <gl/glew.h>//exact first thing 
#include <iostream>
#include <fstream>
#include "GLDisplayWidget.h"
#include <ShapeGenerator.h>
#include <vertex.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <MeCamera.h>

using namespace std;
using namespace glm;
const uint NUM_VERTICES_PER_TRU = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint PassThroughProgramID;

GLuint cubeVertexBufferID;
GLuint cubeIndexBufferID;
GLuint cubeVertexArrayObjectID;
GLuint cubeIndices;

GLDisplayWidget::GLDisplayWidget()
{
	time = 0.0;
	meCamera = new MeCamera;
}

GLDisplayWidget::~GLDisplayWidget()
{
}

void GLDisplayWidget::ClockTick()
{
	this->time++;
}

void GLDisplayWidget::paintGL() {
	//Clean buffer before draw
	//float newRedColor = ((int)time) % 10;
	//float newGreenColor = ((int)time) % 20;
	//float newBlueColor = ((int)time) % 20;
	//printf("color is %f", newRedColor);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glUseProgram(PassThroughProgramID);

	glm::mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.01f, 50.0f); // Projection matrix
	modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, -3.0f)); // 
	modelRotateMatrix = glm::rotate(mat4(), +45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

	mat4 ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	mat4 ModelToViewMatrix = meCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	mat4 fullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(PassThroughProgramID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glBindVertexArray(cubeVertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, 0);
}

void GLDisplayWidget::initializeGL() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	setupVertexArrays();
	installShaders();
}

void GLDisplayWidget::mouseMoveEvent(QMouseEvent * event)
{
	event->ignore(); // pass event down to the MainWidget (logic widget)
}
void GLDisplayWidget::mousePressEvent(QMouseEvent * event)
{
	event->ignore();
}
void GLDisplayWidget::mouseReleaseEvent(QMouseEvent * event)
{
	event->ignore();
}

void GLDisplayWidget::sendDataToOpenGL() {
	ShapeData shape = ShapeGenerator::makeCube();

	glGenBuffers(1, &cubeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &cubeIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	cubeIndices = shape.numIndices;
	shape.cleanup();
}

void GLDisplayWidget::setupVertexArrays()
{
	glGenVertexArrays(1, &cubeVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID);
}
//--------------Shader utility functions
bool  GLDisplayWidget::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}
bool  GLDisplayWidget::checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool  GLDisplayWidget::checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string GLDisplayWidget::readShaderCode(const char * filename)
{
	ifstream meInput(filename); {
		if (!meInput.good()) {
			cout << "File fail to load" << filename;
			exit(1);
		}

		return std::string(
			std::istreambuf_iterator<char>(meInput),
			std::istreambuf_iterator<char>()
		);
	}
}
//--------------Shader utility functions

void GLDisplayWidget::installShaders() {

	GLuint  vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	string temp = readShaderCode("shaders/passThroughVertexShader.glsl");
	const GLchar* adapter[1];

	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/passThroughFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID)
		|| !checkShaderStatus(fragmentShaderID)) {
		return;
	}

	PassThroughProgramID = glCreateProgram();
	glAttachShader(PassThroughProgramID, vertexShaderID);
	glAttachShader(PassThroughProgramID, fragmentShaderID);
	glLinkProgram(PassThroughProgramID);

	if (!checkProgramStatus(PassThroughProgramID)) {
		return;
	}
}