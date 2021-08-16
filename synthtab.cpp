#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QVariant>
#include <qchecklist.h>
#include <QListWidget>

synth1Tab::synth1Tab(fileTab *filetab, QWidget *parent)
    : QWidget(parent)
{
    qDebug("---> @synth1Tab: Start");
    fpga = new FPGAFS;
    ftab1 = filetab;
    qDebug() << "QDir = " << ftab1->sysexfld->text();

    QFont font8("Times",8);
    QFont font10("Times",10);
    QFont font13("Times",13);
    QFont font16("Times",16);
    // Make main value slider
    makeMainslider();
    // Make osc text labels
    makeOscs();
    // Make matrix text labels
    makeMatrix();
    // Make Envelope text labels
    makeEnvs();
    // Make com text labels
    makeCom();
    // init slider:
    Addressreg_x = 0; Addressreg_y = 8;
    val = osc_lcd[0][0]->value();
    main_slider->setRange(-64,63);
    main_slider->setValue(val);

    midiexternCheckBox = new QCheckBox("Hw Midi In",this);
    midiexternCheckBox->setGeometry(QRect(QPoint(890,450),QSize(150,35)));
    QObject::connect(midiexternCheckBox,SIGNAL(stateChanged(int)),this,SLOT(midi_extern_pressed(int)));
    midiexternCheckBox->show();

    keyboard2openbutton = new QPushButton("key open",this);
    keyboard2openbutton->setGeometry(QRect(QPoint(1190, 325),QSize(button_width, button_height)));
    QObject::connect(keyboard2openbutton,SIGNAL(pressed()),this,SLOT(on_keyboard2openbutton_pressed()));
    keyboard2closebutton = new QPushButton("key end",this);
    keyboard2closebutton->setGeometry(QRect(QPoint(1190, 365),QSize(button_width, button_height)));
    QObject::connect(keyboard2closebutton,SIGNAL(pressed()),this,SLOT(on_keyboard2closebutton_pressed()));


    QObject::connect(main_slider,SIGNAL(valueChanged(int)),this,SLOT(main_slider_val_change(int)));
    QObject::connect(synthtolcdbutton,SIGNAL(pressed()),this,SLOT(synthtolcd()));

    filenamelineedit = new QLineEdit(this);
    filenamelineedit->setGeometry(QRect(QPoint(895, 325),QSize(280, 50)));
    filenamelineedit->setFont(font16);
    filenamelineedit->setMaxLength(16);
    QObject::connect(filenamelineedit,SIGNAL(textChanged(QString)),this,SLOT(filenametext_changed(QString)));

    // read valus into Gui
    QByteArray gui_only = qgetenv("GUI_ONLY");
    if( gui_only.isEmpty())
    {
        synthtolcd();
    }
/*
    folderbox = new QCheckList(this);
    folderbox->setFont(font10);
    populate_folderbox();
    folderbox->setStyleSheet(" QCheckList { border: 1px solid gray;"
                               "border-radius: 3px; padding: 1px 8px 1px 3px; min-width: 6em;}"
                               "QScrollBar:vertical {border: 1px solid black; background:green; width:60px; margin: 0px 0px 0px 0px; }"
                               );
    folderbox->setGeometry(QRect(QPoint(885,275),QSize(100,35)));
    QObject::connect(folderbox,SIGNAL(activated(QString)),this,SLOT(on_folderbox_activated(QString)));
*/
    f_label = new QLabel("   Patch Groups",this);
    f_label->setGeometry(QRect(QPoint(1090,457),QSize(170,25)));
    f_label->setAutoFillBackground(true);
    f_label->setStyleSheet("QLabel {"
    "border-style: solid;"
    "border-width: 1px;"
    "border-color: black; "
    "background-color:WhiteSmoke; "
    "}");

    folderlistbox = new QListWidget(this);
    folderlistbox->setFont(font8);
    folderlistbox->setAutoFillBackground(true);
    folderlistbox->setStyleSheet("background-color:WhiteSmoke;");
    populate_folderlistbox();
    folderlistbox->setGeometry(QRect(QPoint(1090,480),QSize(170,200)));
    QStringList folderlist;

//    QObject::connect(folderlistbox, SIGNAL(itemChanged(QListWidgetItem*)),
    QObject::connect(folderlistbox, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(highlightChecked(QListWidgetItem*)));
//   QObject::connect(folderlistbox,SIGNAL(itemPressed(QListWidgetItem)),this,SLOT(on_folderlistbox_statechanged(QListWidgetItem*)));

    fileloadbox = new QComboBox(this);
    fileloadbox->setGeometry(QRect(QPoint(885,275),QSize(350,35)));
    fileloadbox->setFont(font13);

    populate_fileloadbox();
    QObject::connect(fileloadbox,SIGNAL(activated(QString)),this,SLOT(on_fileloadbox_activated(QString)));

    fileloadbox->setStyleSheet(" QComboBox { border: 1px solid gray;"
                               "border-radius: 3px; padding: 1px 8px 1px 3px; min-width: 6em; }"
                               "QScrollBar:vertical {border: 1px solid black; background:green; width:60px; margin: 0px 0px 0px 0px; }"
                               );

    for(int i=0;i<10;i++) {
        presetbutton[i] = new QPushButton("Preset"+QString::number(i+1),this);
        presetbutton[i]->setGeometry(QRect(QPoint((10 + (i *(button_width - 2) )) , 275),QSize((button_width-10), button_height)));
        QObject::connect(presetbutton[i],SIGNAL(pressed()),this,SLOT(preset_pressed()));
        QObject::connect(presetbutton[i],SIGNAL(released()),this,SLOT(preset_released()));
    }
    presetlistenCheckBox = new QCheckBox("Preset Listen",this);
    presetlistenCheckBox->setGeometry(QRect(QPoint(885,680),QSize(150,25)));
    presetlistenCheckBox->show();
    qDebug("---> @synth2Tab: End");
} // synth1Tab::synth1Tab()  end

void synth1Tab::synthtolcd()
{
    u_int8_t regvalue;
    unsigned int Address;
    qDebug("synth2lcd start");
    for(Address = 0x000;Address<0x018;Address++) { // env 1, main vol env, env  2
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x020;Address<0x028;Address++) { // env 3
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x030;Address<0x038;Address++) { // env  4
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x040;Address<0x048;Address++) { // env  5
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x050;Address<0x058;Address++) { // env  6
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x060;Address<0x068;Address++) { // env  7
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x070;Address<0x078;Address++) { // env  8
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x080;Address<0x08c;Address++) { // osc 1
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x090;Address<0x09c;Address++) { // osc 2
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x0a0;Address<0x0ac;Address++) { // osc 3
        regvalue = fpga->SynthregGet(Address);                   setLCD(Address,regvalue);  }
    for(Address = 0x0b0;Address<0x0bc;Address++) { // osc 4
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x0c0;Address<0x0cc;Address++) { // osc 5
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x0d0;Address<0x0dc;Address++) { // osc 6
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x0e0;Address<0x0ec;Address++) { // osc 7
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x0f0;Address<0x0fc;Address++) { // osc 8
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address = 0x100 ;Address<0x180;Address++) { // mod + fb matrix
        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);  }
    for(Address =0x280;Address<0x287;Address++) { // common
//        regvalue = fpga->SynthregGet(Address);                  setLCD(Address,regvalue);
        regvalue = ftab1->commondata[Address-0x280].toInt();    setLCD(Address,regvalue);
        if (Address == 0x282 && (regvalue & 0x10)==0) midiexternCheckBox->setChecked(true); }
    patch_name.clear();
    for(Address =0x290;Address<0x2A0;Address++) { // common patch name
        regvalue = fpga->SynthregGet(Address); setLCD(Address,regvalue);}//patch_name.append((char) regvalue);
//    filenamelineedit->clear();
//    filenamelineedit->setText(patch_name);                    // common patch name
    qDebug("Set filenamelineedit text= %s",qPrintable(patch_name));
    qDebug("patchname is now: %s ",qUtf8Printable(patch_name));
    qDebug("synth2lcd end");
}

void synth1Tab::setEnvPlot(int envNr)
{
    int x[4];
    switch (envNr) {
    case 1:// env 1
        x[0] = env_lcd[0][0]->value();
        x[1] = env_lcd[1][0]->value();
        x[2] = env_lcd[2][0]->value();
        x[3] = env_lcd[3][0]->value();
        points[0].clear();
        points[0] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][0]->value() ) << QPointF( x[0] + x[1], env_lcd[5][0]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][0]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][0]->value() );
        curve[0]->setSamples( points[0] );
        qwtPlotenv[0]->replot();
        break;
    case 2: // Main vol env
        x[0] = env_lcd[8][0]->value();
        x[1] = env_lcd[9][0]->value();
        x[2] = env_lcd[10][0]->value();
        x[3] = env_lcd[11][0]->value();
        points[1].clear();
        points[1] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[12][0]->value() ) << QPointF( x[0] + x[1], env_lcd[13][0]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[14][0]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[15][0]->value() );
        curve[1]->setSamples( points[1] );
        qwtPlotenv[1]->replot();
        break;
    case 3:// env 2
        x[0] = env_lcd[0][1]->value();
        x[1] = env_lcd[1][1]->value();
        x[2] = env_lcd[2][1]->value();
        x[3] = env_lcd[3][1]->value();
        points[2].clear();
        points[2] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][1]->value() ) << QPointF( x[0] + x[1], env_lcd[5][1]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][1]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][1]->value() );
        curve[2]->setSamples( points[2] );
        qwtPlotenv[2]->replot();
        break;
    case 4:// env 3
        x[0] = env_lcd[0][2]->value();
        x[1] = env_lcd[1][2]->value();
        x[2] = env_lcd[2][2]->value();
        x[3] = env_lcd[3][2]->value();
        points[3].clear();
        points[3] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][2]->value() ) << QPointF( x[0] + x[1], env_lcd[5][2]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][2]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][2]->value() );
        curve[3]->setSamples( points[3] );
        qwtPlotenv[3]->replot();
        break;
    case 5:// env 4
        x[0] = env_lcd[0][3]->value();
        x[1] = env_lcd[1][3]->value();
        x[2] = env_lcd[2][3]->value();
        x[3] = env_lcd[3][3]->value();
        points[4].clear();
        points[4] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][3]->value() ) << QPointF( x[0] + x[1], env_lcd[5][3]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][3]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][3]->value() );
        curve[4]->setSamples( points[4] );
        qwtPlotenv[4]->replot();
        break;
    case 6:// env 5
        x[0] = env_lcd[0][4]->value();
        x[1] = env_lcd[1][4]->value();
        x[2] = env_lcd[2][4]->value();
        x[3] = env_lcd[3][4]->value();
        points[5].clear();
        points[5] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][4]->value() ) << QPointF( x[0] + x[1], env_lcd[5][4]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][4]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][4]->value() );
        curve[5]->setSamples( points[5] );
        qwtPlotenv[5]->replot();
        break;
    case 7:// env 6
        x[0] = env_lcd[0][5]->value();
        x[1] = env_lcd[1][5]->value();
        x[2] = env_lcd[2][5]->value();
        x[3] = env_lcd[3][5]->value();
        points[6].clear();
        points[6] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][5]->value() ) << QPointF( x[0] + x[1], env_lcd[5][5]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][5]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][5]->value() );
        curve[6]->setSamples( points[6] );
        qwtPlotenv[6]->replot();
        break;
    case 8:// env 7
        x[0] = env_lcd[0][6]->value();
        x[1] = env_lcd[1][6]->value();
        x[2] = env_lcd[2][6]->value();
        x[3] = env_lcd[3][6]->value();
        points[7].clear();
        points[7] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][6]->value() ) << QPointF( x[0] + x[1], env_lcd[5][6]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][6]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][6]->value() );
        curve[7]->setSamples( points[7] );
        qwtPlotenv[7]->replot();
        break;
    case 9:// env 8
        x[0] = env_lcd[0][7]->value();
        x[1] = env_lcd[1][7]->value();
        x[2] = env_lcd[2][7]->value();
        x[3] = env_lcd[3][7]->value();
        points[8].clear();
        points[8] << QPointF( 0, 0 ) << QPointF( x[0], env_lcd[4][7]->value() ) << QPointF( x[0] + x[1], env_lcd[5][7]->value() )
            << QPointF( x[0] + x[1] + x[2], env_lcd[6][7]->value() ) << QPointF( x[0] + x[1] + x[2] + x[3],env_lcd[7][7]->value() );
        curve[8]->setSamples( points[8] );
        qwtPlotenv[8]->replot();
        break;
   default:
        break;
    }
}

void synth1Tab::env_button_pressed(void)
{
    unsigned int reg_num, i,j;
    for(j=0;j<NUM_OSC;j++)
    {
        if(j==0) reg_num =16;
        else reg_num = 8;
        for(i=0;i<reg_num;i++)
        {
            if(env_button[i][j]->QAbstractButton::isDown())
            {
                Addressreg_x = i; Addressreg_y = j;
                val = env_lcd[i][j]->value();
                main_slider->setRange(0,127);
                main_slider->setValue(val);
            }
       }
    }
}

void synth1Tab::osc_button_pressed(void)
{
    unsigned int i,j;
    for(j=0;j<NUM_OSC;j++)
    {
        for(i=0;i<12;i++)
        {
            if(osc_button[i][j]->QAbstractButton::isDown())
            {
                qDebug( "pressed i = %d j = %d",i,j);
                Addressreg_x = i; Addressreg_y = j+8;
                val = osc_lcd[i][j]->value();
                if((i < 2) || (i >= 7 && i <= 9) ) {
                    main_slider->setRange(-64,63);
                    main_slider->setValue(val);
                }
                else {
                    main_slider->setRange(0,127);
                    main_slider->setValue(val);
                }
            }
        }
    }
}

void synth1Tab::mat_button_pressed(void)
{
    unsigned int i,j;

    for(j=0;j<NUM_OSC;j++)
    {
        for(i=0;i<16;i++)
        {
            if(mat_button[i][j]->QAbstractButton::isDown())
            {
                Addressreg_x = i; Addressreg_y = j+(NUM_OSC * 2);
                val = mat_lcd[i][j]->value();
                main_slider->setRange(0,127);
                main_slider->setValue(val);
            }
        }
    }
}

void synth1Tab::com_button_pressed(void)
{
    unsigned int i;
    for(i=0;i<7;i++)
    {
        if(com_button[i]->QAbstractButton::isDown())
        {
            Addressreg_x = i; Addressreg_y = (NUM_OSC * 5);
            if(i == 2) {
                main_slider->setRange(0,15);
            } else {
                main_slider->setRange(0,127);
            }
//            val = com_lcd[i]->value();
            val = ftab1->commondata[i].toInt();
            main_slider->setValue(val);
        }
    }
}

void synth1Tab::midi_extern_pressed(int value){
    unsigned int regaddress;
    regaddress = ((NUM_OSC * 5) << 4) + 2;
//    if(midiexternCheckBox->isChecked()){
    if(value != 0){
        val = ((int) com_lcd[2]->value()) & 0x0f;
    } else {
        val = ((int) com_lcd[2]->value()) | 0x10;
    }
    fpga->SynthregSet(regaddress,val);
//    qDebug("HW midi val = 0x%x  state_value =%d",val,value);
}

void synth1Tab::main_slider_val_change(int value)
{
    unsigned int regaddress, reg_x;
    regaddress = (Addressreg_y << 4) + Addressreg_x;
    reg_x = regaddress & 0xf;
//    qDebug("slider change Addressreg_y = 0x%x regaddress = 0x%x  Set to value --> %d",Addressreg_y,regaddress,value);
    if(((reg_x < 2) || (reg_x >= 7 && reg_x <= 9)) && (Addressreg_y >=0x08) && (Addressreg_y <=0x0f)) {
        fpga->SynthregSet(regaddress,value+64);
        setLCD(regaddress, value+64);
    }
    else if (regaddress >= 0x280 && regaddress <= 0x287){
        if (regaddress == 0x282){
            if(midiexternCheckBox->isChecked()){
                fpga->SynthregSet(regaddress,(value & 0x0f));
                ftab1->commondata[regaddress - 0x280] = QString::number(value & 0x0f);
            } else {
                fpga->SynthregSet(regaddress,(value | 0x10));
                ftab1->commondata[regaddress - 0x280] = QString::number(value | 0x10);
            }
            setLCD(regaddress, (value & 0x0f));
        }
        else {
            ftab1->commondata[regaddress - 0x280] = QString::number(value);
        }
    }
    else {
        fpga->SynthregSet(regaddress,value);
        setLCD(regaddress, value);
    }
}
void synth1Tab::setLCD(unsigned int RegAddress, u_int8_t newValue)
{
   unsigned  int reg_y,reg_x;
    reg_x = RegAddress & 0xf;
//    qDebug("setLCD RegAddress = 0x%x value= %d",RegAddress,newValue);
    if(RegAddress < 0x080)
    {
         reg_y = (RegAddress & 0xf0) >> 4;
         env_lcd[reg_x][reg_y]->display(newValue);
        switch (reg_y) {
        case 0:     if(reg_x<8) setEnvPlot(1); else setEnvPlot(2); break;
        case 1:     setEnvPlot(3); break;
        case 2:     setEnvPlot(4); break;
        case 3:     setEnvPlot(5); break;
        case 4:     setEnvPlot(6); break;
        case 5:     setEnvPlot(7); break;
        case 6:     setEnvPlot(8); break;
        case 7:     setEnvPlot(9); break;
        default:    break;
        }
    }
    else if(RegAddress >= 0x080 && (RegAddress < 0x100))
    {
        reg_y =( (RegAddress & 0xff0 ) >> 4) - 8;
        if((reg_x < 2) || (reg_x >= 7 && reg_x <= 9)) {osc_lcd[reg_x][reg_y]->display(newValue-64);}
        else osc_lcd[reg_x][reg_y]->display(newValue);
    }
    else if(RegAddress >= 0x100 && RegAddress < 0x180)
    {
       reg_y = ( (RegAddress & 0xff0) >> 4) - 0x10;
       mat_lcd[reg_x][reg_y]->display(newValue);
    }
    else if(RegAddress >= 0x280 && RegAddress < 0x287)
    {
        if(RegAddress == 0x282) {
            if((ftab1->commondata[2].toInt() & 0x10) == 0){
                midiexternCheckBox->setChecked(true);
                com_lcd[reg_x]->display(newValue);
            } else {
                midiexternCheckBox->setChecked(false);
                com_lcd[reg_x]->display(newValue & 0x0F);
            }
        } else {
            com_lcd[reg_x]->display(newValue);
        }
    }
   else if(RegAddress >=0x290 && RegAddress < 0x2A0)
    {
        if(RegAddress == 0x290) patch_name.clear();
        patch_name.append((char) newValue);
 //       qDebug("Text: newValue = %s @ Address 0x%x",(const char *)&newValue,RegAddress);
        if(RegAddress == 0x29F) {
           filenamelineedit->clear();
           filenamelineedit->setText(patch_name);
        }
    }
}

void synth1Tab::populate_folderlistbox()
{
//    qDebug() << "Now in populate_folderlistbox";
//    qDebug() << "QDir = " <<  ftab1->sysexrootfolder;//  fileTab(0).sysexfld->text();
//    qDebug() << "QDir = " <<  ftab1->sysexfld->text();
    QDir directory(ftab1->sysexrootfolder);
    QStringList syxDirectories = directory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    folderlistbox->clear();
    folderlistbox->addItems(syxDirectories);
    QListWidgetItem* item = 0;
//    qDebug() << "bankfolderlist contents = " << ftab1->bankfolderlist;
    for(int i = 0; i < folderlistbox->count(); ++i){
        item = folderlistbox->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        if(ftab1->bankfolderlist.contains(item->text()))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
    }
}

void synth1Tab::populate_fileloadbox()
{
//    qDebug("@ populate_fileloadbox");
    QStringList nameFilter("*.syx");
    fileloadbox->clear();
    for (int i=0;i<ftab1->bankfolderlist.size();i++){
        QDir directory(ftab1->sysexfld->text() + "/" + ftab1->bankfolderlist[i]);
        QFileInfoList syxFilesAndDirectories = directory.entryInfoList(nameFilter);
        QString item;
        for ( const auto& y : syxFilesAndDirectories  )
        {
            item = "<" + ftab1->bankfolderlist[i] + "> " + y.completeBaseName();
            fileloadbox->addItem(item,y.absoluteFilePath());
        }
    }
}

void synth1Tab::on_fileloadbox_activated(const QString &file)
{
    qDebug("on_fileloadbox_activated with file %s",qUtf8Printable(file));
    patch_name.clear();
    qDebug("userData = %s",qUtf8Printable(fileloadbox->currentData().toString()));
    readSyxFile(fileloadbox->currentData().toString());
}

void synth1Tab::preset_pressed()
{
    unsigned int r, size;
    for(r=0;r<10;r++)
    {
        if(presetbutton[r]->QAbstractButton::isDown())
        {
            active_preset = r;
            if(PresetBuffer[r].isEmpty())
            {
                preset_state = 1;// preset_was empty
                size = save_preset_to_array(r);
                p_size = size;
//                qDebug("size = %d",size);
                presetbutton[r]->setStyleSheet("background-color:cyan");
            }
            else  if(presetlistenCheckBox->isChecked()){
                size = load_preset_from_array(r);
                presetbutton[r]->setStyleSheet("background-color:green");
                p_size = size;
                preset_state = 0;  // no button released processing
            }
            else {
                preset_state = 2; // preset is full
            }
        }
    }
}

void synth1Tab::preset_released()
{
    unsigned int size ;
    QMessageBox *msgBox = new QMessageBox;
    QMessageBox *msgBox2 = new QMessageBox;
    if(preset_state == 1)
    {
        msgBox2->setText("<font size=12 > Preset "+ QString::number(active_preset + 1)+ " stored <br> size is " + QString::number(p_size) + " bytes  </font> ");
        msgBox2->setStyleSheet("background-color:cyan");
        msgBox2->exec();
    }
    else if (preset_state == 2)
    {
        msgBox->setText("<font size=12 > Preset " + QString::number(active_preset+1) + " is in use <br> do you want to overwrite, recall or save ?</font>");
        overwriteButton = msgBox->addButton(tr("Overwrite"), QMessageBox::ActionRole);
        overwriteButton->setStyleSheet("background-color:cyan");
        overwriteButton->setAttribute(Qt::WA_AcceptTouchEvents);
        recallButton = msgBox->addButton(tr("Recall"), QMessageBox::ActionRole);
        saveButton = msgBox->addButton(tr("Save"), QMessageBox::ActionRole);
        cancelButton = msgBox->addButton(tr("Cancel"), QMessageBox::ActionRole);
        int ret = msgBox->exec();
        if (msgBox->clickedButton() == overwriteButton){
            PresetBuffer[active_preset].clear();
            size = save_preset_to_array(active_preset);
           msgBox2->setText("<font size=12 >Preset "+ QString::number(active_preset + 1)+ " stored<br> size is " + QString::number(size) + " bytes </font> ");
           msgBox2->exec();
        }
        else if (msgBox->clickedButton() == recallButton){
            size = load_preset_from_array(active_preset);
//            presetbutton[active_preset]->setStyleSheet("background-color:green");
            msgBox2->setText("<font size=12 >Preset "+ QString::number(active_preset + 1)+ " loaded<br> size is " + QString::number(size) + " bytes </font>");
            msgBox2->exec();
        }
        else if (msgBox->clickedButton() == saveButton){
            size = save_preset_to_file1(active_preset);
            size = PresetBuffer[active_preset].size() + 4;
        }
        else if (msgBox->clickedButton() == cancelButton){
           msgBox2->setText("<font size=12 > Action Cancled </font>");
           msgBox2->exec();
        }
        qDebug("msxbox ret = %d",ret);

    }
}

void synth1Tab::overwritebutton_pressed()
{

}

unsigned int synth1Tab::save_preset_to_array(unsigned int arraynum)
{
    u_int8_t val;
    unsigned int i,size;
    for(i=0;i<672;i++) {
        val = fpga->SynthregGet(i);
        PresetBuffer[arraynum].append((char) val);
        qDebug("PresetBuffer[%d] Address = 0x%x value= %d",arraynum, i, val);
    }
    size = PresetBuffer[arraynum].size();
    return(size);
}

unsigned int synth1Tab::load_preset_from_array(unsigned int arraynum)
{
    u_int8_t val;
    unsigned int i,j;
    unsigned int PreserBufferIndex = 0;

    for(j=0;j<NUM_OSC;j++) // Envs
    {
        for(i=0;i<16;i++)
        {
            val = PresetBuffer[arraynum].at(PreserBufferIndex);
            fpga->SynthregSet(PreserBufferIndex,val);
            if(j==0 || i<8) {  setLCD(PreserBufferIndex,val); }
            PreserBufferIndex++;
        }
    }

    for(j=0;j<NUM_OSC;j++) // Oscs
    {
        for(i=0;i<16;i++)
        {
            val = PresetBuffer[arraynum].at(PreserBufferIndex);
            fpga->SynthregSet(PreserBufferIndex,val);
            if(i<12) setLCD(PreserBufferIndex,val);
            PreserBufferIndex++;
        }
    }
    for(j=0;j<NUM_OSC;j++) // Mat 1
    {
        for(i=0;i<16;i++)
        {
            val = PresetBuffer[arraynum].at(PreserBufferIndex);
            fpga->SynthregSet(PreserBufferIndex,val);
            setLCD(PreserBufferIndex,val);
            PreserBufferIndex++;
        }
    }
    for(j=0;j<NUM_OSC;j++) // Mat 2
    {
        for(i=0;i<16;i++)
        {
            PreserBufferIndex++;
        }
    }
    for(j=0;j<NUM_OSC;j++) // Spare
    {
        for(i=0;i<16;i++)
        {
            PreserBufferIndex++;
        }
    }
    for(j=0;j<2;j++) // Com
    {
        for(i=0;i<16;i++)
        {
            val = PresetBuffer[arraynum].at(PreserBufferIndex);
            fpga->SynthregSet(PreserBufferIndex,val);
            if(j== 0 && i<7)   setLCD(PreserBufferIndex,val);
            else if(j==1) setLCD(PreserBufferIndex,val);
            PreserBufferIndex++;
        }
    }
    return(PresetBuffer[arraynum].size());
}

void synth1Tab::on_keyboard2openbutton_pressed()
{
    emit keyboard_view();
}

void synth1Tab::on_keyboard2closebutton_pressed()
{
    emit keyboard_hide();
}

unsigned int synth1Tab::save_preset_to_file1(unsigned int arraynum)
{
    cur_save_index = arraynum;
    emit keyboard_view();
    return(arraynum);
}

unsigned int synth1Tab::save_preset_to_file2()
{
    QString filename =  filenamelineedit->text() + ".syx";
    QStringList nameFilter("*.syx");
    QDir directory(ftab1->sysexfld->text());//SYX_FILE_DIR
    QStringList syxFilesAndDirectories = directory.entryList(nameFilter);
    emit keyboard_hide();
    if(syxFilesAndDirectories.contains(filename)) {
        QMessageBox *msgBox = new QMessageBox;
        QPushButton *overwriteButton = msgBox->addButton(tr("Overwrite"), QMessageBox::ActionRole);
        overwriteButton->setStyleSheet("background-color:cyan");
        QPushButton *renameButton = msgBox->addButton(tr("Rename"), QMessageBox::ActionRole);
        msgBox->setText("<font size=12 > Filename:  " + filename + " allready exists  <br> do you want to overwrite or rename ?</font>");
        msgBox->exec();
        if (msgBox->clickedButton() == overwriteButton){
            write_syx_file(filename);
        }
        else if (msgBox->clickedButton() == renameButton){
            emit keyboard_view();
            return(0);
        }
    }
    else {
        write_syx_file(filename);
    }

    return(2);
}

void synth1Tab::readSyxFile(QString filename)
{
    unsigned int filetype = 0;
    unsigned int Address;
    unsigned int FileBufferIndex;
    u_int8_t regvalue;
    qDebug("in readSyxFile(): filename = %s\n",qUtf8Printable(filename));

//    display->append(filename);
    QFile fileB(filename);
    if (!fileB.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,"show","<font size=12 > Failed to read " + filename + "</font>");
        return;
    }
    Buffer=fileB.readAll();
    fileB.close();
    // read file into synth regs and ui
    if(Buffer.size() == 228 ) filetype = 1;
    else if(Buffer.size() == 676 ) filetype = 2;
    else {
        QMessageBox::information(NULL,"show","<font size=12 >File " +filename + "<br>size: " +Buffer.size() + "<br> has wrong file size </font>");
        return;
    }
    qDebug("filetype = %d", filetype );
    filenamelineedit->clear();
    // Env 1
    FileBufferIndex = 3;
    for(Address=0x000;Address < 0x010;Address++)// Env [0], main vol env(div [0])
    {
        regvalue = Buffer.at(FileBufferIndex);
        fpga->SynthregSet(Address,regvalue);
        setLCD(Address,regvalue);
        FileBufferIndex++;
    }
    // Env 2
    for(Address=0x010;Address < 0x018;Address++)// Env [1]
    {
        regvalue = Buffer.at(FileBufferIndex);
        fpga->SynthregSet(Address,regvalue);
        setLCD(Address,regvalue);
        FileBufferIndex++;
    }
    // Env 3
    FileBufferIndex = 0x23;
    for(Address=0x020;Address < 0x028;Address++)// Env [2]//, (div [2])
    {
        regvalue = Buffer.at(FileBufferIndex);
        fpga->SynthregSet(Address,regvalue);
        setLCD(Address,regvalue);
        FileBufferIndex++;
    }
    // Env 4
    FileBufferIndex = 0x33;
    for(Address=0x030;Address < 0x038;Address++)// Env [3]//, (div [2])
    {
        regvalue = Buffer.at(FileBufferIndex);
        fpga->SynthregSet(Address,regvalue);
        setLCD(Address,regvalue);
        FileBufferIndex++;
    }
    if(filetype == 2){
        // Env 5
        FileBufferIndex = 0x43;
        for(Address=0x040;Address < 0x034;Address++)// Env [4]//, (div [4])
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Env 6
        FileBufferIndex = 0x53;
        for(Address=0x050;Address < 0x058;Address++)// Env [5]//, (div [5])
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Env 7
        FileBufferIndex = 0x63;
        for(Address=0x060;Address < 0x068;Address++)// Env [6]//, (div [6])
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Env 8
        FileBufferIndex = 0x73;
        for(Address=0x070;Address < 0x078;Address++)// Env [7]//, (div [7])
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }

    }
    if(filetype == 1){
        // Osc 1
        FileBufferIndex = 0x43;
        for(Address=0x080;Address < 0x08C;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 2
        FileBufferIndex = 0x53;
        for(Address=0x090;Address < 0x09C;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 3
        FileBufferIndex = 0x63;
        for(Address=0x0A0;Address < 0x0AC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 4
        FileBufferIndex = 0x73;
        for(Address=0x0B0;Address < 0x0BC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }

    }
    else if(filetype == 2){
        // Osc 1
        FileBufferIndex = 0x83;
        for(Address=0x080;Address < 0x08C;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 2
        FileBufferIndex = 0x93;
        for(Address=0x090;Address < 0x09C;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 3
        FileBufferIndex = 0xA3;
        for(Address=0x0A0;Address < 0x0AC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 4
        FileBufferIndex = 0xB3;
        for(Address=0x0B0;Address < 0x0BC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 5
        FileBufferIndex = 0xC3;
        for(Address=0x0C0;Address < 0x0CC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 6
        FileBufferIndex = 0xD3;
        for(Address=0x0D0;Address < 0x0DC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 7
        FileBufferIndex = 0xE3;
        for(Address=0x0E0;Address < 0x0EC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Osc 8
        FileBufferIndex = 0xF3;
        for(Address=0x0F0;Address < 0x0FC;Address++)
        {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
    }
    // Matrix
    if(filetype == 1){
        // Mod 1
        FileBufferIndex = 0x83;
        for(Address=0x100;Address < 0x104;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Fb 1
        FileBufferIndex = 0x8b;
        for(Address=0x108;Address < 0x10c;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Mod 2
        FileBufferIndex = 0x93;
        for(Address=0x110;Address < 0x114;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Fb 2
        FileBufferIndex = 0x9b;
        for(Address=0x118;Address < 0x11c;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Mod 3
        FileBufferIndex = 0xa3;
        for(Address=0x120;Address < 0x124;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Fb 3
        FileBufferIndex = 0xab;
        for(Address=0x128;Address < 0x12c;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Mod 4
        FileBufferIndex = 0xb3;
        for(Address=0x130;Address < 0x134;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
        // Fb 4
        FileBufferIndex = 0xbb;
        for(Address=0x138;Address < 0x13c;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
    }
    else if(filetype == 2){
        FileBufferIndex = 0x103;
        for(Address=0x100;Address < 0x180;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
    }
    // com
    if (filetype == 1){
        FileBufferIndex = 0xC3;
/*        for(Address=0x280;Address < 0x287;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
            fpga->SynthregSet(Address,regvalue);
            setLCD(Address,regvalue);
            FileBufferIndex++;
        }
 */
    }
    else if (filetype == 2){
        FileBufferIndex = 0x283;
        for(Address=0x280;Address < 0x2A0;Address++) {
            regvalue = Buffer.at(FileBufferIndex);
//            if(Address >= 0x290) qDebug("read_syx_file name data = %s at address 0x%x", &regvalue, Address);
            if(Address < 0x287) {} else {
                fpga->SynthregSet(Address,regvalue);
                setLCD(Address,regvalue);
            }
            FileBufferIndex++;
        }
    }
 }

void synth1Tab::write_syx_file(QString filename)
{
    patch_name.clear();
    patch_name.append(filenamelineedit->text().toUtf8());
    QByteArray Buffer = PresetBuffer[cur_save_index];
    Buffer.replace(0x290,patch_name.size(),patch_name);
    Buffer.prepend((char) 0x70);
    Buffer.prepend((char) 0x7D);
    Buffer.prepend((char) 0xF0);
    Buffer.append((char) 0xF7);

    QFileDialog *fflddialog = new QFileDialog(this);
    fflddialog->setStyleSheet("QPushButton{min-height: 80px; min-width: 300px;}");
    fflddialog->setFileMode(QFileDialog::Directory);
    QString fdirname = fflddialog->getExistingDirectory(this,tr("Open Directory"), ftab1->sysexfld->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog);
    QFile fileB(fdirname + "/" + filename);

    QMessageBox *msgBox = new QMessageBox;
    msgBox->setText("<font size=12 > Saving preset " + QString::number(cur_save_index+1) +" <br> to file " + fdirname + "/" + filename +"<br>size = "+ QString::number(Buffer.size()) +  "</font>");
    msgBox->setInformativeText("<font size = 8> do you want to save under this filename ? </font>");
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox->setStyleSheet("background-color:red");
    int ret = msgBox->exec();
    if(ret == QMessageBox::Yes)
    {
        if (!fileB.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(NULL,"show","<font size=12 > Failed to open file</font>");
            return;
        }
        fileB.write(Buffer);
        fileB.close();
        populate_fileloadbox();
        presetbutton[active_preset]->setStyleSheet("background-color:red");
    }
    else if (ret == QMessageBox:: No)
    {
//        fileB.close();
        emit keyboard_view();
    }
    else if (ret == QMessageBox:: Cancel)
    {
//        fileB.close();
    }
}

void synth1Tab::filenametext_changed(QString f_name)
{
    QString filename_nowhite;
    QByteArray filename;
    unsigned int Address;
    int i;
    uint8_t regvalue;
    filename_nowhite = f_name;
    filename_nowhite.replace( " ","" );
    filenamelineedit->setText(filename_nowhite);
    filename = filename_nowhite.toUtf8();
    i = 0;
    for(Address =0x290;Address<0x2A0;Address++) { // common patch name
       if(i <= filename.size()) {
            regvalue = filename[i++];
        } else { regvalue = 32; }
        fpga->SynthregSet(Address,regvalue);
    }
    qDebug("filenametext_changed end %u",filename.size());
}

void synth1Tab::highlightChecked(QListWidgetItem *item){
    if(item->checkState() == Qt::Checked){
        item->setBackground(QColor("#33ff00"));//QColor("#ffffb2")
        ftab1->bankfolderlist.append(item->text());
        for(int loop1 = 0; loop1 < ftab1->bankfolderlist.size(); loop1++) {
            qDebug("folderlist %d is now = %s",loop1,qUtf8Printable(ftab1->bankfolderlist.at(loop1)));
        }
    }
    else {
        item->setBackground(QColor("#ffffff"));
        ftab1->bankfolderlist.removeAll(item->text());
        for(int loop1 = 0; loop1 < ftab1->bankfolderlist.size(); loop1++) {
            qDebug("folderlist %d is now = %s",loop1,qUtf8Printable(ftab1->bankfolderlist.at(loop1)));
        }
    }
    QString temp = "";
    ftab1->saveSettings(temp);
    populate_fileloadbox();
}
