#include <mainwindow.h>

// Make matrix
void synth1Tab::makeMatrix(void)
{
    // Make matrix text labelss
    matLabels <<" osc "<<"in 1"<<"in 2"<<"in 3"<<"in 4"<<"in 5"<<"in 6"<<"in 7"<<"in 8";
    for(int z=0;z<2;z++)
    {
        x_offset = z * 360;
        xi = z * 8;
        for(int i=0;i<9;i++)
        {
            mat_label[i+xi] = new QLabel(matLabels[i],this);
            mat_label[i+xi]->setGeometry(QRect(QPoint((x_offset + 522+(lcd_width * i)),(0)),QSize(lcd_width,(lcd_height))));
            mat_label[i+xi]->show();
        }
        xy = z * 8;
        for(int i=0;i<NUM_OSC;i++)
        {
            mat_num_label[i+xy] = new QLabel(QString::number(i+1)+">",this);
            mat_num_label[i+xy]->setGeometry(QRect(QPoint(x_offset + 530,(30+(lcd_height*i))),QSize(lcd_width,lcd_height)));
            mat_num_label[i+xy]->show();
        }
        // Make mod and fb matrix lcds and buttons
        QPalette *p_mat = new QPalette;
        p_mat->setColor(QPalette::WindowText,Qt::blue);
        for(int j=0;j<NUM_OSC;j++)
        {
            for(int i = 0;i<NUM_OSC;i++){
                mat_lcd[i+xy][j] = new QLCDNumber(this);
                mat_lcd[i+xy][j]->setGeometry(QRect(QPoint((x_offset +550+(lcd_width * i)),(28 + (lcd_height * j))),QSize(lcd_width,lcd_height)));
                mat_lcd[i+xy][j]->setDigitCount(3);
                mat_lcd[i+xy][j]->setSegmentStyle(QLCDNumber::Filled);
                mat_lcd[i+xy][j]->setPalette(*p_mat);
                mat_button[i+xy][j] = new QPushButton("" ,this);
                mat_button[i+xy][j]->setGeometry(QRect(QPoint((x_offset +550+(lcd_width * i)),(28+ (lcd_height * j))),QSize(lcd_width,lcd_height)));
                mat_button[i+xy][j]->setFlat(true);
                QObject::connect(mat_button[i+xy][j],SIGNAL(pressed()),this,SLOT(mat_button_pressed()));
            }
        }
    }
}
