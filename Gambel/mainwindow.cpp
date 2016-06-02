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


	qDebug() << gambel->cpu->registers.F;


	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Rom"), "/home/oskenso/Downloads/", "*.*");

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray dataFile = file.readAll();

		GB_LoadRom(gambel, (uint8_t*)dataFile.data());


	}

#include <string.h>
	//sprintf(stest, "0x%02x", gambel->cpu->memory[0x100]);

	//qDebug() << sprintf("0x%04x", gambel->cpu->memory[0x100]);

	//delete file;

	//qDebug() << gambel->cpu->memory[gambel->cpu->registers.PC];
	//qDebug() << QString(gambel->cpu->registers.PC).asprintf("0x%04x");

	//qDebug() << QString("0x%04x").arg(gambel->cpu->memory[gambel->cpu->registers.PC]);

	char f[6];

	for (int i = 0; i < 30; i++)
	{
		sprintf(f, "Opcode: 0x%02x", gambel->cpu->memory[gambel->cpu->registers.PC]);
		qDebug() << f;
		sprintf(f, "PC: 0x%04x", gambel->cpu->registers.PC);
		qDebug() << f;
		qDebug() << GB_Step(gambel);
	}





	GB_Destroy(gambel);
}

MainWindow::~MainWindow()
{
	delete ui;
}
