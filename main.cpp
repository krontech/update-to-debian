#include "updatewindow.h"
#include <QWSServer>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWSServer::setCursorVisible(false);
	
	UpdateWindow w;
	w.show();
	
	return app.exec();
}
