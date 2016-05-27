#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	GB* gambel = GB_Create();

	gambel->cpu->registers.F = 0;
	gambel->cpu->registers.zero = 1;

	qDebug() << gambel->cpu->registers.F;

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Rom"), "dir", "*.*");

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray dataFile = file.readAll();
		//gambel->
		GB_LoadRom(gb, dataFile.data());
	}
	//delete file;

	//GB_LoadRom(gambel, d;
	//gambel->LoadRom(fileName);

	qDebug() << fileName;

	GB_Destroy(gambel);
}

MainWindow::~MainWindow()
{
	delete ui;
}
