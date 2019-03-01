#include "MainWidget.h"
#include <Qt\qapplication.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <GLDisplayWidget.h>


MainWidget::MainWidget()
{
	displayWidget = new GLDisplayWidget();
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);
	mainLayout->addWidget(displayWidget);

	//startTimer(50);
}

MainWidget::~MainWidget()
{
}

void MainWidget::setReadObj(const char * name)
{
	this->displayWidget->ReadObjName = name;
}

void MainWidget::timerEvent(QTimerEvent *event) {
	printf("time passes\n");
	displayWidget->ClockTick();
	displayWidget->repaint();
}
void MainWidget::keyPressEvent(QKeyEvent *event)
{
	printf("catch\n");
	if (event->key() == Qt::Key_W)
	{
		printf("w");
		displayWidget->repaint();
	}

	if (event->key() == Qt::Key_S)
	{
		printf("s");
		displayWidget->repaint();
	}

	if (event->key() == Qt::Key_A)
	{
		printf("a");
		displayWidget->repaint();
	}

	if (event->key() == Qt::Key_D)
	{
		printf("d");
		displayWidget->repaint();
	}

	if (event->key() == Qt::Key_G)
	{
		printf("d");
		displayWidget->meCamera->reset();
		displayWidget->repaint();
	}

	if (event->key() == Qt::Key_Escape)
	{
		printf("esc");
		QCoreApplication::quit();
	}

	if (event->key() == Qt::Key_F6)
	{
		printf("recomplie Shader");
		displayWidget->installShaders();
		displayWidget->repaint();
	}

	if (event->key() == Qt::Key_Control)
	{
		printf("ctrl pressed");
		IsCtrlPressing = true;
	}

	if (event->key() == Qt::Key_Alt)
	{
		printf("Alt pressed");
		IsAltPressing = true;
	}
	
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Control)
	{
		printf("ctrl pressed");
		IsCtrlPressing = false;
	}

	if (event->key() == Qt::Key_Alt)
	{
		printf("Alt pressed");
		IsAltPressing = false;
	}
}

void MainWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {

		printf("clicked in  mainWidget\n");
		if (hasMouseTracking()) {
			printf("tracking\n");
		}

		dragStartPosition = event->pos();
	}
}

void MainWidget::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {

		printf("released in mainWidget\n");
		if (hasMouseTracking()) {
			printf("tracking\n");
		}
	}
}

void MainWidget::mouseMoveEvent(QMouseEvent * event)
{
	if ((event->pos() - dragStartPosition).manhattanLength()
		< QApplication::startDragDistance())
		return;
	float xMovement = event->pos().x() - dragStartPosition.x();
	float yMovement = event->pos().y() - dragStartPosition.y();

	dragStartPosition = event->pos();
	if (event->buttons() & Qt::LeftButton) {
		if (IsCtrlPressing) {
			displayWidget->setPointLightPosition(xMovement, yMovement); //Change light Position
			displayWidget->LightCamera->position = displayWidget->pointLight1Position;
			displayWidget->LightCamera->viewDirection = -displayWidget->pointLight1Position;
			displayWidget->repaint();
			return;
		}
		if (IsAltPressing){ //Do this to meCamera
			displayWidget->meCamera->mouseUpdate(glm::vec2(xMovement, yMovement));
			printf("Dragging in main Widget\n");
			displayWidget->repaint();
			return;
		} 
		else{ //Do to render camera
			displayWidget->RenderCamera->mouseUpdate(glm::vec2(xMovement, yMovement));
			printf("Dragging in main Widget\n");
			displayWidget->repaint();
			return;
		}
	}

	if (event->buttons() & Qt::RightButton) {
		printf("Right clciked\n");

		dragStartPosition = event->pos();
		if (IsAltPressing) {
			if (yMovement >= 0.0) {
				displayWidget->meCamera->moveForward();
			}
			else {
				displayWidget->meCamera->moveBackward();
			}
		}
		else {
			if (yMovement >= 0.0) {
				displayWidget->RenderCamera->moveForward();
			}
			else {
				displayWidget->RenderCamera->moveBackward();
			}
		}
		printf("Dragging in main Widget\n");
		displayWidget->repaint();
	}

}