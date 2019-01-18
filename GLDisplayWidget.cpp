#include <gl/glew.h>//exact first thing 
#include "GLDisplayWidget.h"

GLDisplayWidget::GLDisplayWidget()
{
	time = 0.0;
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
	float newRedColor = ((int)time) % 10;
	float newGreenColor = ((int)time) % 20;
	float newBlueColor = ((int)time) % 20;
	printf("color is %f", newRedColor);
	glClearColor(newRedColor / 10.0f, newGreenColor / 20.0f, newBlueColor/ 30.0f, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

}

void GLDisplayWidget::initializeGL() {
	glewInit();
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