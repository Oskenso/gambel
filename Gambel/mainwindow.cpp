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

	u8 dmg_bios[0x100] = {
	//QByteArray dmgBios[0x100] = {
		0x31,0xfe,0xff,0xaf,0x21,0xff,0x9f,0x32,0xcb,0x7c,0x20,0xfb,0x21,0x26,0xff,0x0e,
		0x11,0x3e,0x80,0x32,0xe2,0x0c,0x3e,0xf3,0xe2,0x32,0x3e,0x77,0x77,0x3e,0xfc,0xe0,
		0x47,0x11,0x04,0x01,0x21,0x10,0x80,0x1a,0xcd,0x95,0x00,0xcd,0x96,0x00,0x13,0x7b,
		0xfe,0x34,0x20,0xf3,0x11,0xd8,0x00,0x06,0x08,0x1a,0x13,0x22,0x23,0x05,0x20,0xf9,
		0x3e,0x19,0xea,0x10,0x99,0x21,0x2f,0x99,0x0e,0x0c,0x3d,0x28,0x08,0x32,0x0d,0x20,
		0xf9,0x2e,0x0f,0x18,0xf3,0x67,0x3e,0x64,0x57,0xe0,0x42,0x3e,0x91,0xe0,0x40,0x04,
		0x1e,0x02,0x0e,0x0c,0xf0,0x44,0xfe,0x90,0x20,0xfa,0x0d,0x20,0xf7,0x1d,0x20,0xf2,
		0x0e,0x13,0x24,0x7c,0x1e,0x83,0xfe,0x62,0x28,0x06,0x1e,0xc1,0xfe,0x64,0x20,0x06,
		0x7b,0xe2,0x0c,0x3e,0x87,0xe2,0xf0,0x42,0x90,0xe0,0x42,0x15,0x20,0xd2,0x05,0x20,
		0x4f,0x16,0x20,0x18,0xcb,0x4f,0x06,0x04,0xc5,0xcb,0x11,0x17,0xc1,0xcb,0x11,0x17,
		0x05,0x20,0xf5,0x22,0x23,0x22,0x23,0xc9,0xce,0xed,0x66,0x66,0xcc,0x0d,0x00,0x0b,
		0x03,0x73,0x00,0x83,0x00,0x0c,0x00,0x0d,0x00,0x08,0x11,0x1f,0x88,0x89,0x00,0x0e,
		0xdc,0xcc,0x6e,0xe6,0xdd,0xdd,0xd9,0x99,0xbb,0xbb,0x67,0x63,0x6e,0x0e,0xec,0xcc,
		0xdd,0xdc,0x99,0x9f,0xbb,0xb9,0x33,0x3e,0x3c,0x42,0xb9,0xa5,0xb9,0xa5,0x42,0x3c,
		0x21,0x04,0x01,0x11,0xa8,0x00,0x1a,0x13,0xbe,0x20,0xfe,0x23,0x7d,0xfe,0x34,0x20,
		0xf5,0x06,0x19,0x78,0x86,0x23,0x05,0x20,0xfb,0x86,0x20,0xfe,0x3e,0x01,0xe0,0x50
	};

	memcpy(gambel->cpu->memory, dmg_bios, 0x100);
	gambel->cpu->registers.PC = 0;


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

	for (int i = 0; i < 31; i++)
	{
		sprintf(f, "Opcode: 0x%02x", gambel->cpu->memory[gambel->cpu->registers.PC]);
		qDebug() << f;
		sprintf(f, "Opcode: 0x%02x", gambel->cpu->memory[gambel->cpu->registers.PC+1]);
		qDebug() << f;

		sprintf(f, "PC: 0x%04x", gambel->cpu->registers.PC);
		qDebug() << f;
		qDebug() << GB_Step(gambel);
		qDebug() << "\n";
	}





	GB_Destroy(gambel);
}

MainWindow::~MainWindow()
{
	delete ui;
}
