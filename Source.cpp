
//#include <GL/Qt/include/Qt/qapplication.h> //If you want GL TO BE ALWAYS IN FRONT 
#include <Qt/qapplication.h>
int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	printf("test %s", argv[1]);

	return app.exec();

}