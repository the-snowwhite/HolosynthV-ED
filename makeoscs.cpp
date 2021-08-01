#include <mainwindow.h>

// Make osc's
void synth1Tab::makeOscs(void)
{
    // Make osc text labels
    oscLabels <<"osc "<<"  ct"<<"  ft"<<" vol"<<" m_o"<<"fb_o"<<"k_sc"<<" off"<<" pan"<<"b_ct"<<"b_ft"<<"m_in"<<"fb_in";
    for(int i=0;i<13;i++)
    {
        osc_label[i] = new QLabel(oscLabels[i],this);
        osc_label[i]->setGeometry(QRect(QPoint((0+(lcd_width * i)),(0)),QSize(lcd_width,lcd_height)));
        osc_label[i]->show();
    }
    for(int i=0;i<NUM_OSC;i++)
    {
        osc_num_label[i] = new QLabel(QString::number(i+1),this);
        osc_num_label[i]->setGeometry(QRect(QPoint(10,(20+(lcd_height*i))),QSize(lcd_width,lcd_height)));
        osc_num_label[i]->show();
    }

    // Make osc lcds + selectbuttons and value view regs
    QPalette *p_osc = new QPalette;
    p_osc->setColor(QPalette::WindowText,Qt::red);
    for(int j=0;j<NUM_OSC;j++)
    {
        for(int i = 0;i<12;i++){
            osc_lcd[i][j] = new QLCDNumber(this);
            osc_lcd[i][j]->setGeometry(QRect(QPoint((34+(lcd_width * i)),(28 + (lcd_height * j))),QSize(lcd_width,lcd_height)));
            osc_lcd[i][j]->setDigitCount(3);
            osc_lcd[i][j]->setSegmentStyle(QLCDNumber::Filled);
            osc_lcd[i][j]->setPalette(*p_osc);
            osc_button[i][j] = new QPushButton("" ,this);
            osc_button[i][j]->setGeometry(QRect(QPoint((34+(lcd_width * i)),(28+ (lcd_height * j))),QSize(lcd_width,lcd_height)));
            osc_button[i][j]->setFlat(true);
            QObject::connect(osc_button[i][j],SIGNAL(pressed()),this,SLOT(osc_button_pressed()));
        }
    }
}
