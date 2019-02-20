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
const uint NUM_FLOATS_PER_VERTICE = 8;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint PassThroughProgramID;
GLuint PostProcessingProgramID;
GLuint CubeMapProgramID;

GLuint cubeVertexBufferID;
GLuint cubeIndexBufferID;
GLuint cubeVertexArrayObjectID;
GLuint cubeIndices;

GLuint planeVertexBufferID;
GLuint planeIndexBufferID;
GLuint planeVertexArrayObjectID;
GLuint planeIndices;

GLuint framebuffer;
GLuint framebufferTexture;

GLuint teapotVertexBufferID;
GLuint teapotIndexBufferID;
GLuint teapotVertexArrayObjectID;
GLuint teapotIndices;

cyPoint3f BBoxMax;
cyPoint3f BBoxMin;

GLDisplayWidget::GLDisplayWidget()
{
	XDegree = 0.0;
	YDegree = 0.0;

	time = 0.0;
	meCamera = new MeCamera;
	RenderCamera = new MeCamera;
	ReadObjName = "objs/teapot.obj";// default one
	printf("read obj is %s", ReadObjName);

	ambientAmount = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLight1Position = glm::vec3(0.00f, 0.00f,20.00f);
	pointLight1Intensity = 1.0f;
}

GLDisplayWidget::~GLDisplayWidget()
{
}

void GLDisplayWidget::ClockTick()
{
	this->time++;
}

void GLDisplayWidget::paintGL() {

	//In here render to my new frame Buffer 
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(0.05, 0.3, 0.05, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glDisable(GL_DEPTH_TEST);//Disable this for cubemap
	glUseProgram(CubeMapProgramID);

	glm::mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.01f, 50.0f); // Projection matrix
	modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f,0.0f,0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	//printf("Offset is %f in X, %f in Y, %f in z \n", (BBoxMax.x + BBoxMin.x) / 2.0f * 0.2f, (BBoxMax.y + BBoxMin.y) / 2.0f * 0.2f, (BBoxMax.z + BBoxMin.z) / 2.0f * 0.2f);
	modelRotateMatrix = glm::rotate(mat4(),0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(20.0f,20.0f,20.0f));

	mat4 ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	mat4 ModelToViewMatrix = meCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	mat4 fullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(CubeMapProgramID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	GLint modelToWroldMatrixUniformLocation = glGetUniformLocation(CubeMapProgramID, "modelToWorldTransMatrix");
	glUniformMatrix4fv(modelToWroldMatrixUniformLocation, 1, GL_FALSE, &ModelToWorldMatrix[0][0]);
	GLint ambientUniformLocation = glGetUniformLocation(CubeMapProgramID, "ambientLightUniform");
	glUniform3fv(ambientUniformLocation, 1, &ambientAmount[0]);
	GLint Light1PositionUniformLocation = glGetUniformLocation(CubeMapProgramID, "pointLightPosition");
	glUniform3fv(Light1PositionUniformLocation, 1, &pointLight1Position[0]);
	GLint Light1IntensityUniformLocation = glGetUniformLocation(CubeMapProgramID, "pointLightIntensity");
	glUniform1f(Light1PositionUniformLocation, pointLight1Intensity);
	GLuint cameraUniformLocation = glGetUniformLocation(CubeMapProgramID, "cameraPositionWorld");
	glm::vec3 cameraPosition = meCamera->position;
	glUniform3fv(cameraUniformLocation, 1, &cameraPosition[0]);
	GLuint diffuseMapUniformLocation = glGetUniformLocation(CubeMapProgramID, "diffuseTexture");
	glUniform1i(diffuseMapUniformLocation, 0);
	GLuint speculareMapUniformLocation = glGetUniformLocation(CubeMapProgramID, "specularTexture");
	glUniform1i(speculareMapUniformLocation, 1);
	GLuint cubeMapUniformLocation = glGetUniformLocation(CubeMapProgramID, "CubeMapTexture");
	glUniform1i(cubeMapUniformLocation, 3);
	GLuint DrawSkyboxUniformLocation = glGetUniformLocation(CubeMapProgramID, "DrawSkyBox");
	glUniform1i(DrawSkyboxUniformLocation, 1);
	glBindVertexArray(cubeVertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, 0);

	//This one for teapot

	glEnable(GL_DEPTH_TEST);//Enable this for object

	modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	modelRotateMatrix = glm::rotate(mat4(), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(0.2f, 0.2f, 0.2f));

	ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	ModelToViewMatrix = meCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	fullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWroldMatrixUniformLocation, 1, GL_FALSE, &ModelToWorldMatrix[0][0]);
	glUniform1i(DrawSkyboxUniformLocation, 0);

	glBindVertexArray(teapotVertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, teapotIndices, GL_UNSIGNED_INT, 0);

	//Draw Plane
	modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	modelRotateMatrix = glm::rotate(mat4(), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(5.0f, 5.0f,5.0f));

	ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	ModelToViewMatrix = meCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	fullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWroldMatrixUniformLocation, 1, GL_FALSE, &ModelToWorldMatrix[0][0]);
	glUniform1i(DrawSkyboxUniformLocation, 0);

	glBindVertexArray(planeVertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, 0);

	//Finish rendering to frame Buffer
	//Now use a simple plane to display the texture
	//glViewport(0, 0, width(), height());
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glUseProgram(PostProcessingProgramID);

	//modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	//printf("Offset is %f in X, %f in Y, %f in z \n", (BBoxMax.x + BBoxMin.x) / 2.0f * 0.2f, (BBoxMax.y + BBoxMin.y) / 2.0f * 0.2f, (BBoxMax.z + BBoxMin.z) / 2.0f * 0.2f);
	//modelRotateMatrix = glm::rotate(mat4(), 30.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
	//modelScaleMatrix = glm::scale(mat4(), glm::vec3(3.0f,3.0f, 3.0f));

	//ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	//ModelToViewMatrix = RenderCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	//fullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	//glBindVertexArray(planeVertexArrayObjectID);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	//GLuint framebufferTextureUniformLoc = glGetUniformLocation(PostProcessingProgramID, "frameBufferTexture");
	//glUniform1i(framebufferTextureUniformLoc,2);
	//fullTransformMatrixUniformLocation = glGetUniformLocation(PostProcessingProgramID, "modelToProjectionMatrix");
	//glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

	//glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, 0);

	/*glBindVertexArray(planeVertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, 0);*/ // For display plane only
}

void GLDisplayWidget::initializeGL() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	setupVertexArrays();
	setupFrameBuffer();
	installShaders();
}

void GLDisplayWidget::keyPressEvent(QKeyEvent * event)
{
	event->ignore();
}

void GLDisplayWidget::keyReleaseEvent(QKeyEvent * event)
{
	event->ignore();
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

void GLDisplayWidget::setupFrameBuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//To make framebuffer render to a texture I need to generate a texture object first


	glGenTextures(1, &framebufferTexture);
	glActiveTexture(GL_TEXTURE2); // Use texture unit 2
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0); //bind back to default

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
	// use render Buffer object for depth test
	unsigned int renderBufferObject;
	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glActiveTexture(GL_TEXTURE0); // Bind back to default slot
}

void GLDisplayWidget::sendDataToOpenGL() {
	ShapeData shape = ShapeGenerator::makefillerQuard();

	glGenBuffers(1, &planeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &planeIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	planeIndices = shape.numIndices;

	shape = ShapeGenerator::makeCube();

	glGenBuffers(1, &cubeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &cubeIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	cubeIndices = shape.numIndices;
	shape.cleanup();

	std::ostream *outStream = &std::cout;
	if (teapot.LoadFromFileObj(ReadObjName, TRUE, outStream)) {
		printf("load success");
	}
	else {
		printf("load fail");
	}
	// in here get obj map name
	const char *diffuseMapName = teapot.M(0).map_Kd.data;
	const char *specularMapName = teapot.M(0).map_Ks.data;
	std::string filePrefix = "Textures/";
	filePrefix.append(diffuseMapName);
	diffuseMapName = filePrefix.c_str();
	printf("\nthe spec name is %s\n", diffuseMapName);

	QImage diffuseMap = loadTexture(diffuseMapName);
	// send Image to OpenGL
	glActiveTexture(GL_TEXTURE0);
	GLuint diffusetextureID;
	glGenTextures(1, &diffusetextureID);
	glBindTexture(GL_TEXTURE_2D, diffusetextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, diffuseMap.width(),
		diffuseMap.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		diffuseMap.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	filePrefix = "Textures/";
	filePrefix.append(specularMapName);
	specularMapName = filePrefix.c_str();
	printf("\nthe diffuse name is %s\n", specularMapName);

	QImage SpecMap = loadTexture(specularMapName);
	// send Image to OpenGL
	glActiveTexture(GL_TEXTURE1);
	GLuint spectextureID;
	glGenTextures(1, &spectextureID);
	glBindTexture(GL_TEXTURE_2D, spectextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SpecMap.width(),
		SpecMap.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		SpecMap.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	//Environment map information
	glActiveTexture(GL_TEXTURE3); // 3 for cube map 
	GLuint EnvironmentMapTextureID;
	glGenTextures(1, &EnvironmentMapTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, EnvironmentMapTextureID);

	const char * suffixes[] = { "posx", "negx", "posy","negy", "posz", "negz" };

	GLuint targets[] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	std::string cubemapName = "Textures/cubemap";

	for (int i = 0; i < 6; i++) {
		std::string texName = cubemapName + "_" + suffixes[i] + ".png";
		QImage EnvironmentImg = QGLWidget::convertToGLFormat(QImage(texName.c_str(), "PNG"));
		glTexImage2D(targets[i], 0, GL_RGBA, EnvironmentImg.width(), EnvironmentImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, EnvironmentImg.bits());
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//obj load
	teapot.ComputeBoundingBox();
	if (teapot.IsBoundBoxReady()) {
		BBoxMax= teapot.GetBoundMax();
		BBoxMin = teapot.GetBoundMin();
		//cyPoint3f BBoxMax = teapot.GetBoundMax();
	}
	//INTERWEAVE two sets of information together into a vector. Dont know if it is efficient
	std::vector<cyPoint3f> teapotVertices;
	for (int i = 0; i < teapot.NV(); i++) {
		teapotVertices.push_back(teapot.V(i));
	}

	std::vector<cyPoint3f> teapotNormals;
	for (int i = 0; i < teapot.NV(); i++) {
		teapotNormals.push_back(teapot.VN(i));
	}

	//std::vector<cyPoint3f> teapotUVs;
	//for (int i = 0; i < teapot.NVT(); i++) {
	//	teapotUVs.push_back(teapot.VT(i));
	//}

	std::vector<cyPoint3f> teapotUVs;
	teapotUVs.resize(teapot.NV());
	for (int i = 0; i < teapot.NF(); i++) {
		for (int j = 0; j < 3; j++) {
			teapotUVs[teapot.F(i).v[j]] = cyPoint3f(teapot.VT(teapot.FT(i).v[j]).x,
				teapot.VT(teapot.FT(i).v[j]).y, 0);
		}
	}

	std::vector<cyPoint3f> teapotInfos;
	teapotInfos.insert(teapotInfos.end(), teapotVertices.begin(), teapotVertices.end());
	teapotInfos.insert(teapotInfos.end(), teapotNormals.begin(), teapotNormals.end());
	teapotInfos.insert(teapotInfos.end(), teapotUVs.begin(), teapotUVs.end());

	printf("teapot vertices buffer is %d size large\n", teapotVertices.size());
	printf("teapot has %d vertices\n", teapot.NV());

	printf("\nthe num of vertes is %d\n", teapot.NV());
	printf("\nthe num of NORMAL is %d\n", teapot.NV());
	printf("\nthe num of FACE is %d\n", teapot.NF());
	printf("\nthe num of uv is %d\n", teapot.NVT());

	glGenBuffers(1, &teapotVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVertexBufferID);
	// read vertex position info only
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.NF() * sizeof(unsigned int) *3 , &teapot.F(0), GL_STATIC_DRAW);
	// read vertex information from a vector
	glBufferData(GL_ARRAY_BUFFER, (teapot.NV() + teapot.NVN() + teapot.NV())* sizeof(cyPoint3f), &teapotInfos[0], GL_STATIC_DRAW);
	

	glGenBuffers(1, &teapotIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.NF() * sizeof(unsigned int) * 3, &teapot.F(0), GL_STATIC_DRAW);
	teapotIndices = teapot.NF() * 3;

}

void GLDisplayWidget::setupVertexArrays()
{
	glGenVertexArrays(1, &planeVertexArrayObjectID);

	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIndexBufferID);

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

	glGenVertexArrays(1, &teapotVertexArrayObjectID);

	glBindVertexArray(teapotVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(cyPoint3f) * teapot.NV()));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(cyPoint3f) * teapot.NVN() + sizeof(cyPoint3f) * teapot.NV()));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotIndexBufferID);

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
QImage GLDisplayWidget::loadTexture(const char * texName)
{
	return QGLWidget::convertToGLFormat(QImage(texName, "PNG"));
}
//--------------Shader utility functions

void GLDisplayWidget::installShaders() {

	GLuint  vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint  PPvertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint  PPfragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint  CubeMapvertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint  CubeMapfragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	string temp = readShaderCode("shaders/passThroughVertexShader.glsl");
	const GLchar* adapter[1];

	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/passThroughFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/PostProcessingVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(PPvertexShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/PostProcessingFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(PPfragmentShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/CubeMapVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(CubeMapvertexShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/CubeMapFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(CubeMapfragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	glCompileShader(PPvertexShaderID);
	glCompileShader(PPfragmentShaderID);
	glCompileShader(CubeMapvertexShaderID);
	glCompileShader(CubeMapfragmentShaderID);

	if (!checkShaderStatus(vertexShaderID)
		|| !checkShaderStatus(fragmentShaderID)
		|| !checkShaderStatus(PPvertexShaderID)
		|| !checkShaderStatus(PPfragmentShaderID)
		|| !checkShaderStatus(CubeMapvertexShaderID)
		|| !checkShaderStatus(CubeMapfragmentShaderID)
		) {
		return;
	}

	PassThroughProgramID = glCreateProgram();
	glAttachShader(PassThroughProgramID, vertexShaderID);
	glAttachShader(PassThroughProgramID, fragmentShaderID);
	glLinkProgram(PassThroughProgramID);

	PostProcessingProgramID = glCreateProgram();
	glAttachShader(PostProcessingProgramID, PPvertexShaderID);
	glAttachShader(PostProcessingProgramID, PPfragmentShaderID);
	glLinkProgram(PostProcessingProgramID);

	CubeMapProgramID = glCreateProgram();
	glAttachShader(CubeMapProgramID, CubeMapvertexShaderID);
	glAttachShader(CubeMapProgramID, CubeMapfragmentShaderID);
	glLinkProgram(CubeMapProgramID);

	if (!checkProgramStatus(PassThroughProgramID) || !checkProgramStatus(PostProcessingProgramID) || !checkProgramStatus(CubeMapProgramID)) {
		return;
	}
}

void GLDisplayWidget::setPointLightPosition(float Xmovement, float Ymovement)
{
	glm::vec3 previousPosition = this->pointLight1Position;
	XDegree += Xmovement / 100;
	YDegree += Ymovement / 100;
	this->pointLight1Position = glm::vec3(sin(XDegree) * 100,cos(YDegree) * 100, previousPosition.z);
}
