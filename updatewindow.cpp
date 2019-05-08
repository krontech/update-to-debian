#include "updatewindow.h"
#include "ui_updatewindow.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <QDebug>
#include <qfile.h>
#include <QTimer>

#define CURRENT_TAB_1_EXPLANATION  ui->stackedWidget->currentIndex() == 0
#define CURRENT_TAB_2_SYSCHECK ui->stackedWidget->currentIndex() == 1
#define CURRENT_TAB_3_WRITE  ui->stackedWidget->currentIndex() == 2
#define CURRENT_TAB_4_COMPLETE  ui->stackedWidget->currentIndex() == 3


UpdateWindow::UpdateWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpdateWindow)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	qsn = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
	std::cout << "message" << std::endl;
	//std::cout << "> " << std::flush;
	connect(qsn, SIGNAL(activated(int)), this, SLOT(readStdIn()));
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
	
	if(CURRENT_TAB_2_SYSCHECK){
		ui->btnProceed->setEnabled(false);
		std::cout << "Syscheck" << std::endl;
		std::cout << "> " << std::flush;
	}

	if(CURRENT_TAB_3_WRITE){
		std::cout << "Start";
	}
	
	if(CURRENT_TAB_4_COMPLETE){
		ui->btnProceed->hide();
	}
}

void UpdateWindow::readStdIn(){
	std::string line;
	std::getline(std::cin, line);
	
	if (std::cin.eof() || line == "") {
		return;
	}
	
	qDebug()<<"readStdIn" << QString::fromStdString(line);
	
	if (line == "NoSDPresent") {
		ui->btnProceed->setEnabled(false);
	}
	if (line == "WaitForUserInput") {
		ui->btnProceed->setEnabled(true);
	}
}

void UpdateWindow::writeStdIn(){
	std::string line;
		
}