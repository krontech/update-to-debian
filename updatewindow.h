#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QWidget>

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
	
private slots:
	void on_btnClose_clicked();
	void on_btnProceed_clicked();
};

#endif // UPDATEWINDOW_H
