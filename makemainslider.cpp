#include <mainwindow.h>

// Make main value slider
void synth1Tab::makeMainslider(void)
{
    QPalette *p_slider = new QPalette;
    p_slider->setColor(QPalette::Background,Qt::green);
    main_slider = new QSlider(Qt::Vertical,this);
    main_slider->setGeometry(QRect(QPoint(1282,322),QSize(60,300)));
    aSliderProxy = new SliderProxy;
    main_slider->setStyle(aSliderProxy);
    main_slider->setMaximum(127);
    main_slider->setMinimum(0);
    main_slider->setPageStep(1);
    main_slider->setPalette(*p_slider);
    main_slider->show();
    main_lcd = new QLCDNumber(this);
    main_lcd->setGeometry(QRect(QPoint(1285,625),QSize(lcd_width+5,lcd_height)));
    main_lcd->setDigitCount(3);
    main_lcd->setSegmentStyle(QLCDNumber::Filled);
    main_lcd->show();

    synthtolcdbutton = new QPushButton("Read",this);
    synthtolcdbutton->setGeometry(QRect(QPoint(1270 , 660),QSize(button_width, button_height)));

    QObject::connect(main_slider,SIGNAL(valueChanged(int)),main_lcd,SLOT(display(int)));
}
