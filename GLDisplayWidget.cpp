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
GLuint DepthRenderProgramID;
GLuint OutlineProgramID;

GLuint cubeVertexBufferID;
GLuint cubeIndexBufferID;
GLuint cubeVertexArrayObjectID;
GLuint cubeIndices;

GLuint planeVertexBufferID;
GLuint planeIndexBufferID;
GLuint planeVertexArrayObjectID;
GLuint planeIndices;
GLushort outlineIndices;

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
	LightCamera = new MeCamera;
	ReadObjName = "objs/teapot.obj";// default one
	printf("read obj is %s", ReadObjName);

	ambientAmount = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLight1Position = glm::vec3(0.00f, 5.00f,3.25f);
	RenderCamera->position = glm::vec3(0.0f, 0.0f,10.0f);
	LightCamera->position = pointLight1Position;
	LightCamera->UP = glm::vec3(0.0, -1.0, 0.0);
	LightCamera->viewDirection = -pointLight1Position; //set up light camera so it is always looking at center
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
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glCullFace(GL_FRONT);
	glClearColor(0.05, 0.3, 0.05, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glEnable(GL_DEPTH_TEST);
	glUseProgram(DepthRenderProgramID);

	//glm::mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.01f, 50.0f); // Projection matrix
	glm::mat4 projectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 50); // Projection matrix
	modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f,0.0f,0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	modelRotateMatrix = glm::rotate(mat4(),0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(0.2f,0.2f,0.2f));

	mat4 ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	mat4 ModelToViewMatrix = LightCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	mat4 DepthfullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(DepthRenderProgramID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &DepthfullTransformMatrix[0][0]);
	glBindVertexArray(teapotVertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, teapotIndices, GL_UNSIGNED_INT, 0);

	modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	modelRotateMatrix = glm::rotate(mat4(), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(0.2f, 0.2f, 0.2f));

	ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	ModelToViewMatrix = LightCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	DepthfullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	fullTransformMatrixUniformLocation = glGetUniformLocation(DepthRenderProgramID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &DepthfullTransformMatrix[0][0]);

	glBindVertexArray(planeVertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, 0);

	//-------------------------shadow implementation-----------------------------
	glViewport(0, 0, width(), height());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(OutlineProgramID);

	projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.01f, 50.0f);
	modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	//printf("Offset is %f in X, %f in Y, %f in z \n", (BBoxMax.x + BBoxMin.x) / 2.0f * 0.2f, (BBoxMax.y + BBoxMin.y) / 2.0f * 0.2f, (BBoxMax.z + BBoxMin.z) / 2.0f * 0.2f);
	modelRotateMatrix = glm::rotate(mat4(), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(0.2f,0.2f, 0.2f));

	ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	ModelToViewMatrix = RenderCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	mat4 fullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glm:mat3 viewportMatrix(
		width() / 2, 0, width() / 2 + 1,
		0, height() / 2, height() / 2 + 1,
		0, 0, 1
	);

	glm::mat4 DepthBiasFullTransformMatrix = biasMatrix * DepthfullTransformMatrix;
	
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	fullTransformMatrixUniformLocation = glGetUniformLocation(OutlineProgramID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	GLint modelToWroldMatrixUniformLocation = glGetUniformLocation(OutlineProgramID, "modelToWorldTransMatrix");
	glUniformMatrix4fv(modelToWroldMatrixUniformLocation, 1, GL_FALSE, &ModelToWorldMatrix[0][0]);
	GLint tessellationLevelUniformLocation = glGetUniformLocation(OutlineProgramID, "TessLevel");
	glUniform1i(tessellationLevelUniformLocation, 64);
	//GLint BiasMVPUniformLocation = glGetUniformLocation(OutlineProgramID, "BiasmodelToProjectionMatrix");
	//glUniformMatrix4fv(BiasMVPUniformLocation, 1, GL_FALSE, &DepthBiasFullTransformMatrix[0][0]);
	//GLint modelToWroldMatrixUniformLocation = glGetUniformLocation(OutlineProgramID, "modelToWorldTransMatrix");
	//glUniformMatrix4fv(modelToWroldMatrixUniformLocation, 1, GL_FALSE, &ModelToWorldMatrix[0][0]);
	//GLint ambientUniformLocation = glGetUniformLocation(OutlineProgramID, "ambientLightUniform");
	//glUniform3fv(ambientUniformLocation, 1, &ambientAmount[0]);
	//GLint Light1PositionUniformLocation = glGetUniformLocation(OutlineProgramID, "pointLightPosition");
	//glUniform3fv(Light1PositionUniformLocation, 1, &pointLight1Position[0]);
	//GLint Light1IntensityUniformLocation = glGetUniformLocation(OutlineProgramID, "pointLightIntensity");
	//glUniform1f(Light1PositionUniformLocation, pointLight1Intensity);
	//GLuint cameraUniformLocation = glGetUniformLocation(OutlineProgramID, "cameraPositionWorld");
	//glm::vec3 cameraPosition = meCamera->position;
	//glUniform3fv(cameraUniformLocation, 1, &cameraPosition[0]);
	//GLuint diffuseMapUniformLocation = glGetUniformLocation(OutlineProgramID, "diffuseTexture");
	//glUniform1i(diffuseMapUniformLocation, 0);
	//GLuint speculareMapUniformLocation = glGetUniformLocation(OutlineProgramID, "specularTexture");
	//glUniform1i(speculareMapUniformLocation, 1);
	//GLuint normalMapUniformLocation = glGetUniformLocation(OutlineProgramID, "normalTexture");
	//glUniform1i(normalMapUniformLocation, 4);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	//GLuint framebufferTextureUniformLoc = glGetUniformLocation(OutlineProgramID, "frameBufferTexture");
	//glUniform1i(framebufferTextureUniformLoc, 2);

	glBindVertexArray(planeVertexArrayObjectID);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, planeIndices, GL_UNSIGNED_SHORT, 0);

	//-----------Draw actual mesh-------------------
	glUseProgram(PassThroughProgramID);
	fullTransformMatrixUniformLocation = glGetUniformLocation(PassThroughProgramID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	GLint BiasMVPUniformLocation = glGetUniformLocation(PassThroughProgramID, "BiasmodelToProjectionMatrix");
	glUniformMatrix4fv(BiasMVPUniformLocation, 1, GL_FALSE, &DepthBiasFullTransformMatrix[0][0]);
	modelToWroldMatrixUniformLocation = glGetUniformLocation(PassThroughProgramID, "modelToWorldTransMatrix");
	glUniformMatrix4fv(modelToWroldMatrixUniformLocation, 1, GL_FALSE, &ModelToWorldMatrix[0][0]);
	GLint ambientUniformLocation = glGetUniformLocation(PassThroughProgramID, "ambientLightUniform");
	glUniform3fv(ambientUniformLocation, 1, &ambientAmount[0]);
	GLint Light1PositionUniformLocation = glGetUniformLocation(PassThroughProgramID, "pointLightPosition");
	glUniform3fv(Light1PositionUniformLocation, 1, &pointLight1Position[0]);
	GLint Light1IntensityUniformLocation = glGetUniformLocation(PassThroughProgramID, "pointLightIntensity");
	glUniform1f(Light1PositionUniformLocation, pointLight1Intensity);
	GLuint cameraUniformLocation = glGetUniformLocation(PassThroughProgramID, "cameraPositionWorld");
	glm::vec3 cameraPosition = meCamera->position;
	glUniform3fv(cameraUniformLocation, 1, &cameraPosition[0]);
	GLuint diffuseMapUniformLocation = glGetUniformLocation(PassThroughProgramID, "diffuseTexture");
	glUniform1i(diffuseMapUniformLocation, 0);
	GLuint speculareMapUniformLocation = glGetUniformLocation(PassThroughProgramID, "specularTexture");
	glUniform1i(speculareMapUniformLocation, 1);
	GLuint normalMapUniformLocation = glGetUniformLocation(PassThroughProgramID, "normalTexture");
	glUniform1i(normalMapUniformLocation, 4);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	GLuint framebufferTextureUniformLoc = glGetUniformLocation(PassThroughProgramID, "frameBufferTexture");
	glUniform1i(framebufferTextureUniformLoc, 2);

	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, 0);

	//------------Draw light plane------------------
	modelTransformMatrix = glm::translate(mat4(), pointLight1Position); 
	modelRotateMatrix = glm::rotate(mat4(), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	modelScaleMatrix = glm::scale(mat4(), glm::vec3(0.02f, 0.02f, 0.02f));

	ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	ModelToViewMatrix = RenderCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	fullTransformMatrix = projectionMatrix * ModelToViewMatrix;
	glUseProgram(PostProcessingProgramID);
	fullTransformMatrixUniformLocation = glGetUniformLocation(PostProcessingProgramID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

	
	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, 0);


	////---------Debug frame buffer-------------------
	//glViewport(0, 0, width(), height());
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glUseProgram(PostProcessingProgramID);

	//projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.01f, 50.0f);
	//modelTransformMatrix = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); // Because I scale by 0.2, I need to cut my BBOX by 0.2
	//printf("Offset is %f in X, %f in Y, %f in z \n", (BBoxMax.x + BBoxMin.x) / 2.0f * 0.2f, (BBoxMax.y + BBoxMin.y) / 2.0f * 0.2f, (BBoxMax.z + BBoxMin.z) / 2.0f * 0.2f);
	//modelRotateMatrix = glm::rotate(mat4(), 30.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
	//modelScaleMatrix = glm::scale(mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

	//ModelToWorldMatrix = modelTransformMatrix * modelRotateMatrix *  modelScaleMatrix;
	//ModelToViewMatrix = RenderCamera->getWorldToViewMatrix() * ModelToWorldMatrix;
	//mat4 fullTransformMatrix = projectionMatrix * ModelToViewMatrix;

	//glBindVertexArray(planeVertexArrayObjectID);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	//GLuint framebufferTextureUniformLoc = glGetUniformLocation(PostProcessingProgramID, "frameBufferTexture");
	//glUniform1i(framebufferTextureUniformLoc, 2);
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
	// ------------------------------old frame buffers-----------------------------------

	//glGenFramebuffers(1, &framebuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	////To make framebuffer render to a texture I need to generate a texture object first


	//glGenTextures(1, &framebufferTexture);
	//glActiveTexture(GL_TEXTURE2); // Use texture unit 2
	//glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0); //bind back to default

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
	//// use render Buffer object for depth test
	//unsigned int renderBufferObject;
	//glGenRenderbuffers(1, &renderBufferObject);
	//glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	////glActiveTexture(GL_TEXTURE0); // Bind back to default slot

	//---------------End of old frame buffer ---------------------------

	// ------------------------------old frame buffers changes-----------------------------------

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//To make framebuffer render to a texture I need to generate a texture object first


	glGenTextures(1, &framebufferTexture);
	glActiveTexture(GL_TEXTURE2); // Use texture unit 2
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0); //bind back to default

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, framebufferTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glActiveTexture(GL_TEXTURE0); // Bind back to default slot

	//---------------End of old frame buffer changes------------------------
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
	GLushort indices[] = { 0,2,1,3 };
	//memcpy(outlineIndices, indices, sizeof(indices));
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

	QImage NormalMap = loadTexture("Textures/teapot_normal.png");
	// send Image to OpenGL
	glActiveTexture(GL_TEXTURE4);
	GLuint normaltextureID;
	glGenTextures(1, &normaltextureID);
	glBindTexture(GL_TEXTURE_2D, normaltextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, NormalMap.width(),
		NormalMap.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		NormalMap.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

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
	GLuint  DepthRendervertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint  DepthRenderfragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint  OutlinevertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint  OutlinefragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint  OutlineGeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint  TessellationControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
	GLuint  TessellationEvaluationShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);

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

	temp = readShaderCode("shaders/DepthRenderVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(DepthRendervertexShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/DepthRenderFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(DepthRenderfragmentShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/OutlineVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(OutlinevertexShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/OutlineFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(OutlinefragmentShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/OutlineGeometryShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(OutlineGeometryShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/TessellationControlShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(TessellationControlShaderID, 1, adapter, 0);

	temp = readShaderCode("shaders/TessellationEvalutionShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(TessellationEvaluationShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	glCompileShader(PPvertexShaderID);
	glCompileShader(PPfragmentShaderID);
	glCompileShader(DepthRendervertexShaderID);
	glCompileShader(DepthRenderfragmentShaderID);
	glCompileShader(OutlinevertexShaderID);
	glCompileShader(OutlinefragmentShaderID);
	glCompileShader(OutlineGeometryShaderID);
	glCompileShader(TessellationControlShaderID);
	glCompileShader(TessellationEvaluationShaderID);

	if (!checkShaderStatus(vertexShaderID)
		|| !checkShaderStatus(fragmentShaderID)
		|| !checkShaderStatus(PPvertexShaderID)
		|| !checkShaderStatus(PPfragmentShaderID)
		|| !checkShaderStatus(DepthRendervertexShaderID)
		|| !checkShaderStatus(DepthRenderfragmentShaderID)
		|| !checkShaderStatus(OutlinevertexShaderID)
		|| !checkShaderStatus(OutlinefragmentShaderID)
		|| !checkShaderStatus(OutlineGeometryShaderID)
		|| !checkShaderStatus(TessellationControlShaderID)
		|| !checkShaderStatus(TessellationEvaluationShaderID)
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

	DepthRenderProgramID = glCreateProgram();
	glAttachShader(DepthRenderProgramID, DepthRendervertexShaderID);
	glAttachShader(DepthRenderProgramID, DepthRenderfragmentShaderID);
	glLinkProgram(DepthRenderProgramID);

	OutlineProgramID = glCreateProgram();
	glAttachShader(OutlineProgramID, OutlinevertexShaderID);
	glAttachShader(OutlineProgramID, OutlinefragmentShaderID);
	glAttachShader(OutlineProgramID, OutlineGeometryShaderID);
	glAttachShader(OutlineProgramID, TessellationControlShaderID);
	glAttachShader(OutlineProgramID, TessellationEvaluationShaderID);
	glLinkProgram(OutlineProgramID);

	if (!checkProgramStatus(PassThroughProgramID) || !checkProgramStatus(PostProcessingProgramID) || !checkProgramStatus(DepthRenderProgramID) || !checkProgramStatus(OutlineProgramID)) {
		return;
	}
}

void GLDisplayWidget::setPointLightPosition(float Xmovement, float Ymovement)
{
	glm::vec3 previousPosition = this->pointLight1Position;
	this->pointLight1Position = glm::vec3(previousPosition.x + Xmovement/10, previousPosition.y + Ymovement/10, previousPosition.z);
}
