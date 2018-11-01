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
};

#endif // UPDATEWINDOW_H
