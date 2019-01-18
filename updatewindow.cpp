#include "updatewindow.h"
#include "ui_updatewindow.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <QDebug>
#include <qfile.h>
#include <QTimer>

UpdateWindow::UpdateWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpdateWindow)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	//connect(timer, SIGNAL(timeout()), this, SLOT(readStdIn()));
	timer.setInterval(100);        //setinterval
	timer.start();        //start
	qsn = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
	std::cout << "First message" << std::endl;
	std::cout << "> " << std::flush;
	connect(qsn, SIGNAL(activated(int)), this, SLOT(readStdIn()));
	qDebug()<<"test";
}

UpdateWindow::~UpdateWindow()
{
	delete ui;
}

void UpdateWindow::on_btnClose_clicked()
{
	exit(0); //Presumably, to restart the camera.
}
void UpdateWindow::on_btnProceed_clicked()
{
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
	if(ui->stackedWidget->currentIndex() == 2){
		ui->btnProceed->setEnabled(false);
	}
	if(ui->stackedWidget->currentIndex() == 3) ui->btnProceed->hide();
}

void UpdateWindow::readStdIn(){
	qDebug()<<"readStdIn()";
	std::string line;
	 std::getline(std::cin, line);
	 if (std::cin.eof() || line == "quit") {
		std::cout << "Good bye!" << std::endl;
		qDebug()<<"Good bye";
		//emit quit();
	 } else {
		std::cout << "Echo: " << line << std::endl;
		std::cout << "> " << std::flush;
  }
}