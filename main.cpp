#include "updatewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UpdateWindow w;
	w.show();
	
	return a.exec();
}
