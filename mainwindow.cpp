#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
//    ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
//    this->setFixedSize(1280,800);// fix windows size
    this->setFixedSize(1366,768);// fix windows size
    mytabWidget = new QTabWidget(this);
    ftab1 = new fileTab();
    stab1 = new synth1Tab();

    mytabWidget->addTab(ftab1, tr("File Actions"));
    mytabWidget->addTab(stab1, tr("Synth1"));
    mytabWidget->setFixedSize(1350,768);
    mytabWidget->setGeometry(0,0,0,0);
    QObject::connect( ftab1 , SIGNAL(quit_touched()) ,this ,SLOT(close()));
    QObject::connect(stab1,SIGNAL(keyboard_view()),ftab1,SLOT(on_keyboardopenbutton_pressed()));
    QObject::connect(stab1,SIGNAL(keyboard_hide()),ftab1,SLOT(on_keyboardclosebutton_pressed()));
    QObject::connect(ftab1,SIGNAL(key_return_touched()),stab1,SLOT(save_preset_to_file2()));

    mytabWidget->setCurrentIndex(1);

    mytabWidget->show();
}

MainWindow::~MainWindow()
{
//    delete ui;
}

void MainWindow::quit(void)
{
    close();
}
