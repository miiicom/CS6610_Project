#pragma once
#include<QtOpenGL/qglwidget>
class GLDisplayWidget :public QGLWidget
{
public:
	GLDisplayWidget();
	~GLDisplayWidget();
	void ClockTick();

protected:
	void paintGL();
	void initializeGL();

private:
	float time;
};

