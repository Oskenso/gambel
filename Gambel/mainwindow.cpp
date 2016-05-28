#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

#include <QFileDialog>
#include <stdio.h>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	GB* gambel = GB_Create();

	gambel->cpu->registers.F = 0;
	gambel->cpu->registers.zero = 1;

	qDebug() << gambel->cpu->registers.F;


	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Rom"), "/home/oskenso/Downloads/", "*.*");

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray dataFile = file.readAll();

		GB_LoadRom(gambel, (uint8_t*)dataFile.data());


	}


	//sprintf(stest, "0x%02x", gambel->cpu->memory[0x100]);

	//qDebug() << sprintf("0x%04x", gambel->cpu->memory[0x100]);

	//delete file;

	qDebug() << gambel->cpu->memory[gambel->cpu->registers.PC];
	qDebug() << GB_Step(gambel);
	qDebug() << GB_Step(gambel);
	qDebug() << GB_Step(gambel);


	GB_Destroy(gambel);
}

MainWindow::~MainWindow()
{
	delete ui;
}
