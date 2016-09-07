#pragma once

#include <QMainWindow>

#include "libgambel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
	Ui::MainWindow *ui;
	GB* gambel;
	QImage *screen;
    //char* f;
};
