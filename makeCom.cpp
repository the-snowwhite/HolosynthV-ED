#include <mainwindow.h>

// Make com
void synth1Tab::makeCom(void)
{
    // Make com text labels
    comLabels <<"  pb \nrange"<<"main \n  vol"<<"midi \n  ch "<<" vel\nscale"<<" osc  \nindex" <<" dev \n  id " << " oc\ntave";
    for(int i=0;i<7;i++)
    {
        com_label[i] = new QLabel(comLabels[i],this);
        com_label[i]->setGeometry(QRect(QPoint((892+(lcd_width * i)),(380)),QSize(lcd_width,(lcd_height+15))));
        com_label[i]->show();
    }
    // Make com lcds + selectbuttons and value view regs
    p_com = new QPalette;
    p_com->setColor(QPalette::WindowText,Qt::black);
    //    p_com->setColor(QPalette::Window ,Qt::darkGray);
    for(int i = 0;i<7;i++){
        com_lcd[i] = new QLCDNumber(this);
        com_lcd[i]->setGeometry(QRect(QPoint((890+(lcd_width * i)),422 ),QSize(lcd_width,lcd_height)));
        com_lcd[i]->setDigitCount(3);
        com_lcd[i]->setSegmentStyle(QLCDNumber::Filled);
        com_lcd[i]->setPalette(*p_com);
        com_button[i] = new QPushButton("" ,this);
        com_button[i]->setGeometry(QRect(QPoint((890+(lcd_width * i)),422),QSize(lcd_width,lcd_height)));
        com_button[i]->setFlat(true);
        QObject::connect(com_button[i],SIGNAL(pressed()),this,SLOT(com_button_pressed()));
    }
}
