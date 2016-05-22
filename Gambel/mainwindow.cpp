#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	GB* gambel = GB_Create();
	gambel->memory = 300;
	qDebug() << gambel->memory;
	GB_Destroy(gambel);
}

MainWindow::~MainWindow()
{
	delete ui;
}
