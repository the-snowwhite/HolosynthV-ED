#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QDesktopServices>

static const char *default_config_filename = "/default_holosynthv";

fileTab::fileTab(QWidget *parent)
    : QWidget(parent)
{
    QString appDataLocation;
//    fpga = new FPGAFS;
    closebutton = new QPushButton("Quit",this);
    fileopenbutton = new QPushButton("Open",this);
    keyboardopenbutton = new QPushButton("key open",this);
    keyboardclosebutton = new QPushButton("key close",this);
    QGroupBox *topGroup = new QGroupBox();
    QGridLayout *topLayout = new QGridLayout(this);
//    topLayout->setHorizontalSpacing(10);
    topLayout->addWidget(closebutton,0,0);
    topLayout->addWidget(fileopenbutton,0,1);
    topLayout->addWidget(keyboardopenbutton,0,2);
    topLayout->addWidget(keyboardclosebutton,0,3);
    topGroup->setLayout(topLayout);
    QLabel *sysexfldLabel = new QLabel(tr("Patch files root location:"));
    QFileDialog *flddialog = new QFileDialog(this);
    flddialog->setStyleSheet("QPushButton{min-height: 80px; min-width: 300px;}");
    flddialog->setFileMode(QFileDialog::AnyFile);

    sdirbutton = new QPushButton("Set Patch folder",this);

    sysexfld = new QLineEdit(this);
    sysexfld->setPlaceholderText("Top Sysex Patch folder");
    display = new QTextEdit(this);
//    display->setGeometry(QRect(QPoint(10, 150),QSize(400, 400)));
    QGroupBox *botGroup = new QGroupBox();
    QGridLayout *botLayout = new QGridLayout(this);
    botLayout->addWidget(sysexfldLabel,1,0);
    botLayout->addWidget(sdirbutton,2,0);
    botLayout->addWidget(sysexfld,2,1);
    botLayout->addWidget(display,3,1);
    botGroup->setLayout(botLayout);


    this->myKeyboard = new widgetKeyBoard(false);
    this->myKeyboard->setZoomFacility(true);
    this->myKeyboard->enableSwitchingEcho(false); // enable possibility to change echo through keyboard
    this->myKeyboard->createKeyboard(); // only create keyboard

    QObject::connect(closebutton, SIGNAL(pressed()) , this , SLOT(on_closebutton_pressed()));
    QObject::connect(fileopenbutton,SIGNAL(pressed()),this,SLOT(open_file_dialog()));
    QObject::connect(keyboardopenbutton,SIGNAL(pressed()),this,SLOT(on_keyboardopenbutton_pressed()));
    QObject::connect(keyboardclosebutton,SIGNAL(pressed()),this,SLOT(on_keyboardclosebutton_pressed()));
    QObject::connect(sdirbutton, SIGNAL(pressed()) , this , SLOT(on_sdirbutton_pressed()));
    QObject::connect(myKeyboard,SIGNAL(return_touched()),this,SIGNAL(key_return_touched()));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(topGroup, 0, 0);
    layout->addWidget(botGroup, 1, 0);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    appDataLocation = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if (appDataLocation.isEmpty())
        QMessageBox::information(this, tr("Error"), tr("Can't retrieve standard folder location"));
    else
    {
        m_sSettingsFile = appDataLocation + default_config_filename;

        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, appDataLocation);
    }

    loadSettings();
 //   settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "HolosynthVED", "", this);
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

void fileTab::on_sdirbutton_pressed()
{
    QFileDialog *flddialog = new QFileDialog(this);
    flddialog->setStyleSheet("QPushButton{min-height: 80px; min-width: 300px;}");
    flddialog->setFileMode(QFileDialog::Directory);
    QString homedirpath = QDir::homePath();
    QString dirname = flddialog->getExistingDirectory(this,tr("Open Folder", homedirpath.toStdString().c_str(), QFileDialog::DontUseNativeDialog));
    sysexfld->setText(dirname);
    saveSettings();
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

    QString dirname = fdiag->getOpenFileName( this, tr("Read  syx Patch File"), sysexfld->text() , tr("Document files (*.syx );;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );
//    readSyxFile(filename);
}


void fileTab::loadSettings() {

    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText = settings.value("text", "").toString();
    qDebug("in loadSettings(): m_sSettingsFile = %s\n",qUtf8Printable(m_sSettingsFile));
    qDebug("in loadSettings(): sText = %s\n",qUtf8Printable(sText));
    if (sysexfld)
    {
        sysexfld->setText(sText);
        qDebug("in loadSettings(): saved to sysexfld -> %s\n",qUtf8Printable(sText));
    }
}
void fileTab::saveSettings() {

    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText = (sysexfld) ? sysexfld->text() : "";
    qDebug("in saveSettings(): sText = %s\n",qUtf8Printable(sText));
    settings.setValue("text", sText);
    if (sysexfld)
    {
        sysexfld->setText(sText);
    }
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
