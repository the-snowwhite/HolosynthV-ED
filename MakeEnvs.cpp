#include <mainwindow.h>

// Make Envelope gens
void synth1Tab::makeEnvs(void)
{
    // Make Envelope text labels
    for(int j=0;j<9;j++)
    {
        p_env[j] = new QPalette;
        switch (j) {
        case 0:
            x_offset = 0; y_offset = 5; z=0; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::green);
            break;
        case 1:
            x_offset = 876; y_offset = 160; z=0; o=8;
            p_env[j]->setColor(QPalette::WindowText,Qt::black);
            break;
        case 2:
            x_offset = 220; y_offset = 5; z=1; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::blue);
            break;
        case 3:
            x_offset = 440; y_offset = 5; z=2; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::red);
            break;
        case 4:
            x_offset = 660; y_offset = 5; z=3; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::magenta);
            break;
        case 5:
            x_offset = 0; y_offset = 200; z=4; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::green);
            break;
        case 6:
            x_offset = 220; y_offset = 200; z=5; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::blue);
            break;
        case 7:
            x_offset = 440; y_offset = 200; z=6; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::red);
            break;
        case 8:
            x_offset = 660; y_offset = 200; z=7; o=0;
            p_env[j]->setColor(QPalette::WindowText,Qt::magenta);
            break;
        default:
            break;
        }
        envLabels <<"Rate "<<"Level";
        for(int i=0;i<2;i++)
        {
            env_label[i][j] = new QLabel(envLabels[i],this);
            env_label[i][j]->setGeometry(QRect(QPoint(x_offset + 10,(y_offset + 445+(lcd_height*i))),QSize(lcd_width,lcd_height)));
            env_label[i][j]->show();
        }
        for(int i=0;i<4;i++)
        {
            env_num_label[i][j] = new QLabel(QString::number(i+1),this);
            env_num_label[i][j]->setGeometry(QRect(QPoint((x_offset + 70+(lcd_width * i)),(y_offset + 420)),QSize(lcd_width,(lcd_height))));
            env_num_label[i][j]->show();
        }

        // Make Envelope lcd's and select buttons
        for(int xi=0;xi<2;xi++)
        {
            for(int i = 0;i<4;i++){
                env_lcd[(o+i+(xi*4))][z] = new QLCDNumber(this);
                env_lcd[(o+i+(xi*4))][z]->setGeometry(QRect(QPoint((x_offset + 50+(lcd_width * i)),(y_offset + 445 + (lcd_height * xi))),QSize(lcd_width,lcd_height)));
                env_lcd[(o+i+(xi*4))][z]->setDigitCount(3);
                env_lcd[(o+i+(xi*4))][z]->setSegmentStyle(QLCDNumber::Filled);
                env_lcd[(o+i+(xi*4))][z]->setPalette(*p_env[j]);
                env_button[(o+i+(xi*4))][z] = new QPushButton("" ,this);
                env_button[(o+i+(xi*4))][z]->setGeometry(QRect(QPoint((x_offset + 50+(lcd_width * i)),(y_offset + 445+ (lcd_height * xi))),QSize(lcd_width,lcd_height)));
                env_button[(o+i+(xi*4))][z]->setFlat(true);
                QObject::connect(env_button[(o+i+(xi*4))][z],SIGNAL(pressed()),this,SLOT(env_button_pressed()));
            }
        }
    }

    // Make Envelope qwt display
    for(int i=0;i<9;i++)
    {
        qwtPlotenv[i] = new QwtPlot(this);
        curve[i] = new QwtPlotCurve();
        curve[i]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        symbol[i] = new QwtSymbol( QwtSymbol::Ellipse,
                                   QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
        curve[i]->setSymbol( symbol[i] );
        points[i] << QPointF(0, 0 ) << QPointF( 0, 0 ) << QPointF( 0, 0 )
                  << QPointF( 0 , 127) << QPointF( 0 , 0 );
        curve[i]->setSamples( points[i] );
        curve[i]->attach( qwtPlotenv[i] );
        qwtPlotenv[i]->setAxisScale(QwtPlot::yLeft,0,127,0);
        qwtPlotenv[i]->setAxisScale(QwtPlot::xBottom ,0,508,0);
        qwtPlotenv[i]->enableAxis(QwtPlot::xBottom,false);
        qwtPlotenv[i]->enableAxis(QwtPlot::yLeft,false);
        qwtPlotenv[i]->show();
    }
    curve[0]->setPen( Qt::green, 4 );
    curve[1]->setPen( Qt::black, 4 );
    curve[2]->setPen( Qt::blue, 4 );
    curve[3]->setPen( Qt::red, 4 );
    curve[4]->setPen( Qt::magenta, 4 );
    curve[5]->setPen( Qt::green, 4 );
    curve[6]->setPen( Qt::blue, 4 );
    curve[7]->setPen( Qt::red, 4 );
    curve[8]->setPen( Qt::magenta, 4 );
    qwtPlotenv[0]->setGeometry(QRect(QPoint(10,330),QSize(200,100)));
    qwtPlotenv[1]->setGeometry(QRect(QPoint(890,480),QSize(200,100)));
    qwtPlotenv[2]->setGeometry(QRect(QPoint(230,330),QSize(200,100)));
    qwtPlotenv[3]->setGeometry(QRect(QPoint(450,330),QSize(200,100)));
    qwtPlotenv[4]->setGeometry(QRect(QPoint(670,330),QSize(200,100)));
    qwtPlotenv[5]->setGeometry(QRect(QPoint(10,520),QSize(200,100)));
    qwtPlotenv[6]->setGeometry(QRect(QPoint(230,520),QSize(200,100)));
    qwtPlotenv[7]->setGeometry(QRect(QPoint(450,520),QSize(200,100)));
    qwtPlotenv[8]->setGeometry(QRect(QPoint(670,520),QSize(200,100)));
}
