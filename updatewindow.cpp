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
		std::cout << "Syscheck" << std::endl;
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
	
	if (line == "NoSDPresent") {
		updateSDStatus = SYSCHECK_FAIL;
		updateSyscheckText();
		ui->btnProceed->setEnabled(false);
		return;
	}
	
	if (line == "WaitForUserInput") {
		updateSDStatus = SYSCHECK_OK;
		updateSyscheckText();
		ui->btnProceed->setEnabled(true);
		return;
	}
	
	QString qstring;
	qstring.operator =(QString::fromStdString(line));
	
	if(qstring.contains("FAILED")) usbStatus = SYSCHECK_FAIL;
	if(qstring.contains("No such file")) usbStatus = SYSCHECK_FAIL;
	if(line == "USBCheckStart"){
		usbStatus = SYSCHECK_CHECKING;
	}
	if(line == "USBCheckDone"){
		if(usbStatus != SYSCHECK_FAIL) usbStatus = SYSCHECK_OK;
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
	if(usbStatus == SYSCHECK_CHECKING) SyscheckText.append("Checking...\n");
	if(usbStatus == SYSCHECK_OK) SyscheckText.append("OK\n");
	if(usbStatus == SYSCHECK_FAIL) SyscheckText.append("Fail\n");
	SyscheckText.append("System SD Card: ");
	if(systemSDStatus == SYSCHECK_CHECKING) SyscheckText.append("Checking...\n");
	if(systemSDStatus == SYSCHECK_OK) SyscheckText.append("OK\n");
	if(systemSDStatus == SYSCHECK_FAIL) SyscheckText.append("Fail\n");
	SyscheckText.append("Update SD Card: ");
	if(updateSDStatus == SYSCHECK_CHECKING) SyscheckText.append("Checking...\n");
	if(updateSDStatus == SYSCHECK_OK) SyscheckText.append("OK\n");
	if(updateSDStatus == SYSCHECK_FAIL) SyscheckText.append("Fail\n");
	
	SyscheckText.append("Press \"Proceed\" to continue.");
	
	ui->lblSyscheckStatus->setText(SyscheckText);
}