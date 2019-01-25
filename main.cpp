
//#include <GL/Qt/include/Qt/qapplication.h> //If you want GL TO BE ALWAYS IN FRONT 
#include <GLDisplayWidget.h>
#include <MainWidget.h>
#include <Qt/qapplication.h>
#include <iostream>
int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	printf("test %s", argv[1]);

	MainWidget mymainWidget;
	mymainWidget.setFixedSize(1024, 1024);
	mymainWidget.setReadObj(argv[1]);
	mymainWidget.show();
	return app.exec();

}