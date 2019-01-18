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
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private:
	GLDisplayWidget * displayWidget;
	QPoint dragStartPosition;
	bool IsPressing;
};
