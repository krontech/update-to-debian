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
	std::cout << "Syscheck" << std::endl;
	connect(qsn, SIGNAL(activated(int)), this, SLOT(readStdIn()));
	usbStatus = systemSDStatus = updateSDStatus = SYSCHECK_CHECKING;
	usbStatusString = systemSDStatusString = updateSDStatusString = "Checking...\n";
	ui->btnProceed->setEnabled(true);
	ui->lblSyscheckWarning->setVisible(false);
	ui->progressBar->setVisible(false);
}

UpdateWindow::~UpdateWindow()
{
	delete ui;
}

void UpdateWindow::on_btnCancel_clicked()
{
	system("reboot");
}
void UpdateWindow::on_btnProceed_clicked()
{
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
	qDebug()<<"index="<<ui->stackedWidget->currentIndex();
	
	if(CURRENT_TAB_2_SYSCHECK){
		if(QFile::exists("/opt/camera/to-update-ui")
		&& QFile::exists("/opt/camera/to-update-backend")) {
			systemSDStatusString = "OK\n";
			systemSDStatus = SYSCHECK_OK;
		}
		else {
			systemSDStatusString = "Fail\n";
			systemSDStatus = SYSCHECK_FAIL;
		}
		updateSyscheckTab();
	}

	if(CURRENT_TAB_3_WRITE){
		ui->btnProceed->setEnabled(false);
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
		ui->lblSyscheckWarning->setVisible(false);
		updateSyscheckTab();
		return;
	}
	
	if (line == "TopSDVettedBrand") {
		ui->lblSyscheckWarning->setVisible(false);
		return;
	}
	if (line == "TopSDUnknownBrand") {
		ui->lblSyscheckWarning->setVisible(true);
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
	
	if (line == "ProgramRunFromTopSD") ui->lblExplanation->setText("<html><head/><body><p>Version 0.4.0 is a major software upgrade. It requires an entirely new System SD Card to be created, unlike past upgrades which used the existing System SD Card. To create the new card, insert a blank MicroSD card in the SD Card Slot located on the top of your Chronos. You will need an SD to MicroSD adapter to do this.<br><br><span style=\" font-weight:600; color:#a40000;\">This update must be run from a USB stick, not from the SD card in the top slot. Please move the update files to a USB stick and re-run the update from there.</span></p></body></html>");
	
	if(qstring.contains("Skipping PMIC firmware update")){
		ui->lblWrite->setText("Chronos 0.4.0 is now being written to the top MicroSD Card. This process should take about 15 minutes to complete.");
		ui->progressBar->setVisible(true);
	}
	if(qstring.contains("Updating PMIC Firmware")) ui->lblWrite->setText("Power manager:\nUpdating power manager firmware...");
	if(qstring.contains("Erasing flash...")) ui->lblWrite->setText("Power manager:\nErasing flash...");
	if(qstring.contains("Erasing Address")) ui->lblWrite->setText("Power manager:\n" + qstring);
	if(qstring.contains("Done erasing flash")) ui->lblWrite->setText("Power manager:\nDone erasing flash");
	if(qstring.contains("Writing to address")) ui->lblWrite->setText("Power manager:\n" + qstring);
	if(qstring.contains("Firmware update complete!")) ui->lblWrite->setText("Power manager:\nFirmware update complete!");
	
	if(qstring.contains("out") && qstring.contains("+")){
		qstring.truncate(qstring.indexOf('+'));
		int64_t integer = qstring.toInt();
		float percent = 100 * integer / 7167999;
		qDebug()<<"percent is  " << percent;
		qDebug();
		ui->lblWrite->setText("Chronos 0.4.0 is now being written to the top MicroSD Card. This process should take about 15 minutes to complete.");
		ui->progressBar->setVisible(true);
		ui->progressBar->setValue(percent);
	}
	if(qstring.contains("dd") && qstring.contains("error")) ui->lblComplete->setText("Error writing to SD card.\n\nPlease remove and reinsert the card, then restart the update.");
	if(line == "WriteDone"){
		ui->btnProceed->click();
		ui->stackedWidget->setCurrentIndex(3);
		ui->btnCancel->setVisible(false);
		ui->btnProceed->setVisible(false);
	}
}

void UpdateWindow::updateSyscheckTab(){
	QString SyscheckText;
	
	SyscheckText.append("USB Drive: ");
	SyscheckText.append(usbStatusString);
	SyscheckText.append("System SD Card: ");
	SyscheckText.append(systemSDStatusString);
	SyscheckText.append("Update SD Card: ");
	SyscheckText.append(updateSDStatusString);

	if(usbStatus == SYSCHECK_OK  &&  systemSDStatus == SYSCHECK_OK  &&  updateSDStatus == SYSCHECK_OK) {
		SyscheckText.append("Press \"Proceed\" to continue.");
		ui->btnProceed->setEnabled(true);
	} else if(CURRENT_TAB_2_SYSCHECK) ui->btnProceed->setEnabled(false);
	
	ui->lblSyscheckStatus->setText(SyscheckText);
}
void UpdateWindow::on_btnPowerDown_clicked()
{
    ui->lblComplete->setText("Shutting down...");
    std::cout << "PowerDown" << std::endl;
}
