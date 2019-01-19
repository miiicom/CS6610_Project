#pragma once
#include <GL\glew.h>
#include<QtOpenGL/qglwidget>
#include <QtGui/qmouseevent>
#include <glm/glm.hpp>
#include <String>
#include <MeCamera.h>
class GLDisplayWidget :public QGLWidget
{
public:
	GLDisplayWidget();
	~GLDisplayWidget();
	void ClockTick();
	MeCamera * meCamera;

protected:
	void paintGL();
	void initializeGL();
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void sendDataToOpenGL();
	void setupVertexArrays();
	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	std::string readShaderCode(const char * filename);
	void installShaders();
private:
	float time;

	glm::mat4 modelTransformMatrix;
	glm::mat4 modelRotateMatrix;
	glm::mat4 modelScaleMatrix;
	glm::mat4 projectionMatrix;
};

