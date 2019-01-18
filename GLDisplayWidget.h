#pragma once
#include<QtOpenGL/qglwidget>
#include <QtGui\qmouseevent>
class GLDisplayWidget :public QGLWidget
{
public:
	GLDisplayWidget();
	~GLDisplayWidget();
	void ClockTick();

protected:
	void paintGL();
	void initializeGL();
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	float time;
};

