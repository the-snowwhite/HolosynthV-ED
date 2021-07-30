#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>

fileTab::fileTab(QWidget *parent)
    : QWidget(parent)
{
    fpga = new FPGAFS;
    closebutton = new QPushButton("Quit",this);
    fileopenbutton = new QPushButton("Open",this);
    keyboardopenbutton = new QPushButton("key open",this);
    keyboardclosebutton = new QPushButton("key close",this);
    closebutton->setGeometry(QRect(QPoint(10,10),QSize(button_width,button_height)));
    fileopenbutton->setGeometry(QRect(QPoint(10 +(2 * button_width), 10),QSize(button_width, button_height)));
    keyboardopenbutton->setGeometry(QRect(QPoint(10 +(5 * button_width), 10),QSize(button_width, button_height)));
    keyboardclosebutton->setGeometry(QRect(QPoint(10 +(7 * button_width), 10),QSize(button_width, button_height)));
    QObject::connect(closebutton, SIGNAL(pressed()) , this , SLOT(on_closebutton_pressed()));
    QObject::connect(fileopenbutton,SIGNAL(pressed()),this,SLOT(open_file_dialog()));
    QObject::connect(keyboardopenbutton,SIGNAL(pressed()),this,SLOT(on_keyboardopenbutton_pressed()));
    QObject::connect(keyboardclosebutton,SIGNAL(pressed()),this,SLOT(on_keyboardclosebutton_pressed()));
    display = new QTextEdit(this);
    display->setGeometry(QRect(QPoint(10, 100),QSize(400, 400)));

    this->myKeyboard = new widgetKeyBoard(false);
    this->myKeyboard->setZoomFacility(true);
    this->myKeyboard->enableSwitchingEcho(false); // enable possibility to change echo through keyboard
    this->myKeyboard->createKeyboard(); // only create keyboard
    QObject::connect(myKeyboard,SIGNAL(return_touched()),this,SIGNAL(key_return_touched()));
}
fileTab::~fileTab()
{
    this->myKeyboard->hide(true);
    delete (this->myKeyboard);
}
void fileTab::on_closebutton_pressed()
{
    emit quit_touched();
}

void fileTab::on_keyboardopenbutton_pressed()
{
    qDebug("opening keyboard");
    this->myKeyboard->show(this);   // once created keyboard object, use this method to switch between windows
    this->myKeyboard->move(this->x(), this->y() + this->myKeyboard->height()+300); // to move keyboard
}

void fileTab::on_keyboardclosebutton_pressed()
{
   qDebug("hiding keyboard");
    myKeyboard->hide(true);
}

void fileTab::open_file_dialog()
{
    QFileDialog *fdiag = new QFileDialog(this);
    fdiag->setStyleSheet("QPushButton{min-height: 80px; min-width: 300px;}");

    QString filename = fdiag->getOpenFileName( this, tr("Read  syx Patch File"), SYX_FILE_DIR , tr("Document files (*.syx );;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );
//    readSyxFile(filename);
}
/*
void fileTab::saveSyxFile(QString filename)
{
//    synth1Tab* synthTab = new synth1Tab();
    display->append(filename);
    QFile fileB(filename);
    if (!fileB.open(QIODevice::WriteOnly))
    {
    QMessageBox::information(NULL,"show","Failed to open file");
    return;
    }
}
*/
