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

typedef enum SyscheckValues
{
	SYSCHECK_CHECKING = 0,
	SYSCHECK_OK,
	SYSCHECK_FAIL
} SyscheckValues;

UpdateWindow::UpdateWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpdateWindow)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	qsn = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
	std::cout << "message" << std::endl;
	connect(qsn, SIGNAL(activated(int)), this, SLOT(readStdIn()));
	usbStatus = systemSDStatus = updateSDStatus = SYSCHECK_CHECKING;
	usbStatusString = systemSDStatusString = updateSDStatusString = "Checking...\n";
	updateSyscheckText();
}

UpdateWindow::~UpdateWindow()
{
	delete ui;
}

void UpdateWindow::on_btnCancel_clicked()
{
	exit(0); //Presumably, to restart the camera.
}
void UpdateWindow::on_btnProceed_clicked()
{
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
	qDebug()<<"index="<<ui->stackedWidget->currentIndex();
	
	if(CURRENT_TAB_2_SYSCHECK){
		ui->btnProceed->setEnabled(false);
		std::cout << "Tab-Syscheck" << std::endl;
	}

	if(CURRENT_TAB_3_WRITE){
		std::cout << "Start" << std::endl;
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
	
	if (line == "NoTopSDPresent") {
		updateSDStatus = SYSCHECK_FAIL;
		updateSDStatusString = "Fail\n";
		updateSyscheckText();
		ui->btnProceed->setEnabled(false);
		return;
	}
	
	if (line == "TopSDPresent") {
		updateSDStatus = SYSCHECK_OK;
		updateSDStatusString = "OK\n";
		updateSyscheckText();
		ui->btnProceed->setEnabled(true);
		return;
	}
	
	QString qstring;
	qstring.operator =(QString::fromStdString(line));
	
	if(qstring.contains("FAILED")) usbStatusString = "Fail\n";
	if(qstring.contains("No such file")) usbStatusString = "Fail\n";
	if(line == "USBCheckStart"){
		usbStatusString = "Checking...\n";
	}
	if(line == "USBCheckDone"){
		if(!usbStatusString.contains("Fail")) usbStatusString = "OK\n";
	}
	if(qstring.contains("out") && qstring.contains("+")){
		qstring.truncate(qstring.indexOf('+'));
		int64_t integer = qstring.toInt();
		float percent = 100 * integer / 123456;
		qDebug()<<"percent is  " << percent;
		qDebug();
		ui->progressBar->setValue(percent);
	}
}

void UpdateWindow::updateSyscheckText(){
	QString SyscheckText;
	
	SyscheckText.append("USB Drive: ");
	SyscheckText.append(usbStatusString);
	SyscheckText.append("System SD Card: ");
	if(systemSDStatus == SYSCHECK_CHECKING) SyscheckText.append("Checking...\n");
	if(systemSDStatus == SYSCHECK_OK) SyscheckText.append("OK\n");
	if(systemSDStatus == SYSCHECK_FAIL) SyscheckText.append("Fail\n");
	SyscheckText.append("Update SD Card: ");
	SyscheckText.append(updateSDStatusString);

	SyscheckText.append("Press \"Proceed\" to continue.");
	
	ui->lblSyscheckStatus->setText(SyscheckText);
}