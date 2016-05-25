#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	GB* gambel = GB_Create();

	gambel->cpu->registers.F = 0;
	gambel->cpu->registers.zero = 1;

	qDebug() << gambel->cpu->registers.F;
	GB_Destroy(gambel);
}

MainWindow::~MainWindow()
{
	delete ui;
}
