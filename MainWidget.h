#pragma once
#include <QtGui\qwidget.h>
#include <Qt\qevent.h>
#include <GLDisplayWidget.h>

class MainWidget : public QWidget
{
public:
	MainWidget();
	~MainWidget();
protected:
	void timerEvent(QTimerEvent *event);
private:
	GLDisplayWidget * displayWidget;
};
