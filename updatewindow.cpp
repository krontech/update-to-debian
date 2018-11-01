#include "updatewindow.h"
#include "ui_updatewindow.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <QDebug>
#include <qfile.h>

UpdateWindow::UpdateWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpdateWindow)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	char * string[1000];
	
	mkfifo("/tmp/status.pipe", S_IRWXU | S_IRWXG | S_IRWXO);
	qDebug("about to open FIFO");
	fp = fopen("/tmp/status.pipe", "w+");
	qDebug("opened FIFO");
	
	while(fread(string, 1, 100, fp)){
		qDebug()<<"string: " <<string;//do stuff when process B writes to fifo
		qDebug("UpdateWindow() constructor finished");
	}
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
