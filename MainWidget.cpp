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

	startTimer(50);
}
void MainWidget::timerEvent(QTimerEvent *event) {
	printf("time passes\n");
	displayWidget->ClockTick();
	displayWidget->repaint();
}



MainWidget::~MainWidget()
{
}
