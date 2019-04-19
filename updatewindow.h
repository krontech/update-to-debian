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
	void writeStdIn();

private:
	Ui::UpdateWindow *ui;
	FILE *fp;
	QSocketNotifier *qsn;
	QTimer timer;
	
private slots:
	void on_btnClose_clicked();
	void on_btnProceed_clicked();
	void readStdIn();
};

#endif // UPDATEWINDOW_H
