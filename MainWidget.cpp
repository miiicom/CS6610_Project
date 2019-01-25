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
}

void MainWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {

		printf("clicked in  mainWidget\n");
		if (hasMouseTracking()) {
			printf("tracking\n");
		}
		IsPressing = true;
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
		IsPressing = false;
	}
}

void MainWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() & Qt::LeftButton) {
		if ((event->pos() - dragStartPosition).manhattanLength()
			< QApplication::startDragDistance())
			return;
		float xMovement = event->pos().x() - dragStartPosition.x();
		float yMovement = event->pos().y() - dragStartPosition.y();

		dragStartPosition = event->pos();

		displayWidget->meCamera->mouseUpdate(glm::vec2(xMovement, yMovement));
		printf("Dragging in main Widget\n");
		displayWidget->repaint();
	}

	if (event->buttons() & Qt::RightButton) {
		if ((event->pos() - dragStartPosition).manhattanLength()
			< QApplication::startDragDistance())
			return;
		float xMovement = event->pos().x() - dragStartPosition.x();
		float yMovement = event->pos().y() - dragStartPosition.y();

		dragStartPosition = event->pos();
		if (yMovement >= 0.0) {
			displayWidget->meCamera->moveForward();
		}
		else {
			displayWidget->meCamera->moveBackward();
		}
		printf("Dragging in main Widget\n");
		displayWidget->repaint();
	}

}