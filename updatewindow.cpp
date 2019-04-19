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
	qDebug()<<"index="<<ui->stackedWidget->currentIndex();
	
	if(ui->stackedWidget->currentIndex() == 1){
		ui->btnProceed->setEnabled(false);
	}
	
	if(ui->stackedWidget->currentIndex() == 3){
		ui->btnProceed->hide();
	}
}

void UpdateWindow::readStdIn(){
	std::string line;
	std::getline(std::cin, line);
	
	if (std::cin.eof() || line == "start") {
		ui->btnProceed->setEnabled(true);
	}
	
	qDebug()<<"readStdIn" << QString::fromStdString(line);
	
	if (std::cin.eof() || line == "") {
		return;
	}
}