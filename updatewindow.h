#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QObject>
#include <QSocketNotifier>
#include <iostream>

namespace Ui {
class UpdateWindow;
}

class UpdateWindow : public QWidget
{
	Q_OBJECT
	
public:
	explicit UpdateWindow(QWidget *parent = 0);
	~UpdateWindow();	

private:
	Ui::UpdateWindow *ui;
	FILE *fp;
	QSocketNotifier *qsn;
	QTimer timer;
	int usbStatus, systemSDStatus, updateSDStatus;
	QString usbStatusString, systemSDStatusString, updateSDStatusString;
	
private slots:
	void on_btnCancel_clicked();
	void on_btnProceed_clicked();
	void readStdIn();
	void updateSyscheckTab();
	void on_btnPowerDown_clicked();
};

#endif // UPDATEWINDOW_H
