#include "DlgBackground.h"

#define x_origin    200

DlgSetBackground::DlgSetBackground(KEYHDR *padhdr, QWidget *parent) :
    QDialog(parent),
    m_EdtClr1(this),m_EdtClr2(this),m_EdtClr3(this)
{
    m_padhdr = padhdr;
    m_cx = m_padhdr->cx;
    m_cy = m_padhdr->cy;
    setGeometry(0, 0, m_cx+200, m_cy+100);

    ////////////////// { Fill, Linear, Radial, Pict, PictScale, Preset, Fill2 };
    QComboBox *pCombo = new QComboBox(this);
    pCombo->setGeometry(10, 20, 160, 30);
    pCombo->addItem("Fill Collor");
    pCombo->addItem("Linear Gradient (a:0-100)");
    pCombo->addItem("Radial Gradient (a:0-100)");
    pCombo->addItem("Picture");
    pCombo->addItem("Picture Scaled");
    pCombo->addItem("Preset Gradient (a:1-180) webgradients.com)");
    pCombo->addItem("2 x Fill Collor (a:0-14)");
    pCombo->addItem("no");
    connect(pCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangeCombo(int)));

    btn1 = new QPushButton("Color 1", this);
    btn1->setGeometry(10,50,100,30);
    connect(btn1, SIGNAL(clicked()), this, SLOT(setBtClr1()));

    btn2 = new QPushButton("Color 2", this);
    btn2->setGeometry(10,80,100,30);
    connect(btn2, SIGNAL(clicked()), this, SLOT(setBtClr2()));

    btn3 = new QPushButton("Color 3", this);
    btn3->setGeometry(10,110,100,30);
    connect(btn3, SIGNAL(clicked()), this, SLOT(setBtClr3()));

    //btn4 = new QPushButton("Color 4", this);
    //btn4->setGeometry(10,140,100,30);
    //connect(btn4, SIGNAL(clicked()), this, SLOT(setBtClr4()));

    QPushButton *btnX = new QPushButton("Exit", this);
    btnX->setGeometry(10,200,160,30);
    connect(btnX, SIGNAL(clicked()), this, SLOT(reject()));

    m_EdtClr1.setGeometry(120,50,50,30);
    m_EdtClr2.setGeometry(120,80,50,30);
    m_EdtClr3.setGeometry(120,110,50,30);

    connect(&m_EdtClr1, SIGNAL(textEdited(const QString)), this, SLOT(OnChangeEdtClr1(const QString)));// doesn't emit by SetText()
    connect(&m_EdtClr2, SIGNAL(textEdited(const QString)), this, SLOT(OnChangeEdtClr2(const QString)));// doesn't emit by SetText()
    connect(&m_EdtClr3, SIGNAL(textEdited(const QString)), this, SLOT(OnChangeEdtClr3(const QString)));// doesn't emit by SetText()

    /////////////////////////////////// set data
    btn1->setPalette(QPalette(m_padhdr->color[0]));
    btn1->setAutoFillBackground(true);
    btn2->setPalette(QPalette(m_padhdr->color[1]));
    btn2->setAutoFillBackground(true);
    btn3->setPalette(QPalette(m_padhdr->color[2]));
    btn3->setAutoFillBackground(true);
    //btn4->setPalette(QPalette(m_padhdr->color[3]));
    //btn4->setAutoFillBackground(true);

    pCombo->setCurrentIndex(m_padhdr->bckgrType);

    m_EdtClr1.setText(QString::number( m_padhdr->attr[5]));
    m_EdtClr2.setText(QString::number( m_padhdr->attr[6]));
    m_EdtClr3.setText(QString::number( m_padhdr->attr[7]));
}



void DlgSetBackground::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    if(m_padhdr->bckgrType==Back::Fill)
    {        
        p.fillRect(x_origin, 0, m_cx, m_cy, m_padhdr->color[0]);
    }
    else if(m_padhdr->bckgrType==Back::Linear)
    {
        QLinearGradient gradient(m_padhdr->attr[0]+x_origin, m_padhdr->attr[1], m_padhdr->attr[2]+x_origin, m_padhdr->attr[3]);
        gradient.setColorAt((float)m_padhdr->attr[5]/100, m_padhdr->color[0]);
        if(m_padhdr->attr[6]) gradient.setColorAt((float)m_padhdr->attr[6]/100, m_padhdr->color[1]);
        if(m_padhdr->attr[7]) gradient.setColorAt((float)m_padhdr->attr[7]/100, m_padhdr->color[2]);
        //if(m_padhdr->attr[8]) gradient.setColorAt((float)m_padhdr->attr[8]/100, m_padhdr->color[3]);
        p.fillRect(x_origin, 0, m_cx, m_cy, QBrush(gradient));
    }
    else if(m_padhdr->bckgrType==Back::Radial)
    {
        ////////////////////////          center point x/y                  radial,                 focus point x/y
        QRadialGradient gradient(m_padhdr->attr[0]+x_origin, m_padhdr->attr[1], m_padhdr->attr[4], m_padhdr->attr[2]+x_origin, m_padhdr->attr[3]);
        gradient.setColorAt((float)m_padhdr->attr[5]/100, m_padhdr->color[0]);
        if(m_padhdr->attr[6]) gradient.setColorAt((float)m_padhdr->attr[6]/100, m_padhdr->color[1]);
        if(m_padhdr->attr[7]) gradient.setColorAt((float)m_padhdr->attr[7]/100, m_padhdr->color[2]);
        //if(m_padhdr->attr[8]) gradient.setColorAt((float)m_padhdr->attr[8]/100, m_padhdr->color[3]);
        p.fillRect(x_origin, 0, m_cx, m_cy, QBrush(gradient));
    }
    else if(m_padhdr->bckgrType==Back::Preset)
    {
        if(m_padhdr->attr[5]==0 || m_padhdr->attr[5]>180) m_padhdr->attr[5]=1;
        p.fillRect(x_origin, 0, m_cx, m_cy, (QGradient::Preset)m_padhdr->attr[5]);
    }
    else if(m_padhdr->bckgrType==Back::Fill2)
    {
        p.fillRect(x_origin, 0, m_cx, m_cy, m_padhdr->color[0]);
        p.fillRect(x_origin, 0, m_cx, m_cy, QBrush(m_padhdr->color[1], (Qt::BrushStyle)m_padhdr->attr[6]));
    }
}


void DlgSetBackground::mousePressEvent(QMouseEvent *ev)
{
    if(!(ev->buttons()&Qt::LeftButton))
    {
        ContextMenu(ev->x()-x_origin, ev->y());
    }
}


void DlgSetBackground::ContextMenu(int x, int y)
{
    QPoint p( x+x_origin, y );
    p=mapToGlobal(p);

    /////////////////////////////// select by action return
    QMenu  Menu( this );
    QAction *act;

    if(m_padhdr->bckgrType==Back::Linear)
    {
        Menu.addAction("Set Gradient from this point")->setData(1);
        Menu.addAction("Set Gradient to this point")->setData(2);

        if((act=Menu.exec(p)))
        {
            switch(act->data().toInt())
            {
            case 1: m_padhdr->attr[0]=x; m_padhdr->attr[1]=y; break;
            case 2: m_padhdr->attr[2]=x; m_padhdr->attr[3]=y;
            }
        }
    }
    else if(m_padhdr->bckgrType==Back::Radial)
    {
        Menu.addAction("Set the Center of Circle")->setData(1);
        Menu.addAction("Set the Radial of Circle")->setData(2);
        Menu.addAction("Set the Focus of Gradient")->setData(3);

        if((act=Menu.exec(p)))
        {
            switch(act->data().toInt())
            {
            case 1:
                m_padhdr->attr[0]=x; m_padhdr->attr[1]=y;
                break;
            case 3:
                m_padhdr->attr[2]=x; m_padhdr->attr[3]=y;
                break;
            case 2:
            {
                double dist = sqrt ( pow((m_padhdr->attr[0]-x), 2) + pow((m_padhdr->attr[1]-y), 2) );
                m_padhdr->attr[4] = dist;
            }
            }
        }
    }

    update();
}


void DlgSetBackground::OnChangeCombo(int idx)
{
    m_padhdr->bckgrType = idx;
    update();
}


void DlgSetBackground::setBtClr1()
{
    QColor color = QColorDialog::getColor(m_padhdr->color[0], this);

    if (color.isValid())
    {
        btn1->setPalette(QPalette(color));
        btn1->setAutoFillBackground(true);

        m_padhdr->color[0]=color.rgb();
        update();
    }
}


void DlgSetBackground::setBtClr2()
{
    QColor color = QColorDialog::getColor(m_padhdr->color[1], this);

    if (color.isValid())
    {
        btn2->setPalette(QPalette(color));
        btn2->setAutoFillBackground(true);

        m_padhdr->color[1]=color.rgb();
        update();
    }
}


void DlgSetBackground::setBtClr3()
{
    QColor color = QColorDialog::getColor(m_padhdr->color[2], this);

    if (color.isValid())
    {
        btn3->setPalette(QPalette(color));
        btn3->setAutoFillBackground(true);

        m_padhdr->color[2]=color.rgb();
        update();
    }
}

/*
void DlgSetBackground::setBtClr4()
{
    QColor color = QColorDialog::getColor(m_padhdr->color[3], this);

    if (color.isValid())
    {
        btn4->setPalette(QPalette(color));
        btn4->setAutoFillBackground(true);

        m_padhdr->color[3]=color.rgb();
        update();
    }
}
*/

void DlgSetBackground::OnChangeEdtClr1(const QString val)
{
    m_padhdr->attr[5] = val.toInt();
    update();
}

void DlgSetBackground::OnChangeEdtClr2(const QString val)
{
    m_padhdr->attr[6] = val.toInt();
    update();
}

void DlgSetBackground::OnChangeEdtClr3(const QString val)
{
    m_padhdr->attr[7] = val.toInt();
    update();
}
