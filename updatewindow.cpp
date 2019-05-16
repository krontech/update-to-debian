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
	ui->btnProceed->setEnabled(true);
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
		std::cout << "Tab-Syscheck" << std::endl;
		updateSyscheckTab();
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
	if (std::cin.eof() || line == "") return;
	QString qstring;
	qstring.operator = (QString::fromStdString(line));
		
	qDebug()<<"readStdIn" << QString::fromStdString(line);
	
	if (line == "TopSDMissing") {
		updateSDStatus = SYSCHECK_FAIL;
		updateSDStatusString = "Fail - not present.\n";
		updateSyscheckTab();
		return;
	}
	
	if (line == "TopSDTooSmall") {
		updateSDStatus = SYSCHECK_FAIL;
		updateSDStatusString = "Fail - too small. Minimum size is 4GB.\n";
		updateSyscheckTab();
		return;
	}
	
	if (line == "TopSDGood") {
		updateSDStatus = SYSCHECK_OK;
		updateSDStatusString = "OK\n";
		updateSyscheckTab();
		return;
	}
	
	if(line == "USBCheckStart"){
		usbStatusString = "Checking...\n";
		usbStatus = SYSCHECK_CHECKING;
		updateSyscheckTab();
		return;
	}
	if(qstring.contains("FAILED")) usbStatusString = "Fail\n";
	if(qstring.contains("No such file")) usbStatusString = "Fail\n";
	if(line == "USBCheckDone" && !usbStatusString.contains("Fail")){
		usbStatusString = "OK\n";
		usbStatus = SYSCHECK_OK;
		updateSyscheckTab();
		return;
	}
	
	if(qstring.contains("out") && qstring.contains("+")){
		qstring.truncate(qstring.indexOf('+'));
		int64_t integer = qstring.toInt();
		float percent = 100 * integer / 123456;
		qDebug()<<"percent is  " << percent;
		qDebug();
		ui->progressBar->setValue(percent);
	}
	updateSyscheckTab();
}

void UpdateWindow::updateSyscheckTab(){
	QString SyscheckText;
	
	SyscheckText.append("USB Drive: ");
	SyscheckText.append(usbStatusString);
	SyscheckText.append("System SD Card: ");
	if(systemSDStatus == SYSCHECK_CHECKING) SyscheckText.append("Checking...\n");
	if(systemSDStatus == SYSCHECK_OK) SyscheckText.append("OK\n");
	if(systemSDStatus == SYSCHECK_FAIL) SyscheckText.append("Fail\n");
	SyscheckText.append("Update SD Card: ");
	SyscheckText.append(updateSDStatusString);

	if(usbStatus == SYSCHECK_OK && updateSDStatus == SYSCHECK_OK) {
		SyscheckText.append("Press \"Proceed\" to continue.");
		ui->btnProceed->setEnabled(true);
	} else ui->btnProceed->setEnabled(false);
	
	ui->lblSyscheckStatus->setText(SyscheckText);
}