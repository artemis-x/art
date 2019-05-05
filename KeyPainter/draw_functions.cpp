#include "draw_functions.h"
#include <QPainter>
#include <QtGlobal>


/*
 *  δημιουργεί κάθετη διαβάθμιση χρωμάτων
 *
    it works both
    -------------
    1) up-to-down	>	RainbowH(pDC, 0, 200, 0, 100, icolor, dcolor, 2)
    2) down-to-up	>	RainbowH(pDC, 0, 200, -100, 0, icolor, dcolor, -2)
*/
void draw::RainbowV(QPainter &p, int x1, int cx, int y1, int y2, quint32 icolor, float db, float dg, float dr, int stepy)
{
    // initial value of each color
    float b =  icolor&0xff;
    float g = (icolor&0xff00)>>8;
    float r = (icolor&0xff0000)>>16;
    // degradetion value of each color
    for(int y3=y1; y3<y2; y3+=labs(stepy))
    {
        p.fillRect( x1, labs(y3), cx, stepy, QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)) );

        if((r+=dr)>=255) r=-255;
        if((g+=dg)>=255) g=-255;
        if((b+=db)>=255) b=-255;
    }
}

/*
void draw::Frame(QPainter &p, int x, int y, int cx, int cy)
{
    p.setBrush(Qt::NoBrush);
    //p.setPen(Qt::black);
    //p.drawRect(x+4, y+4, cx-8, cy-8);
    p.setPen(0xdddd);
    p.drawRect(x+3, y+3, cx-6, cy-6);
    p.setPen(0xbbbb);
    p.drawRect(x+2, y+2, cx-4, cy-4);
    p.setPen(0x999999);
    p.drawRect(x+1, y+1, cx-2, cy-2);
    p.setPen(0x777777);
    p.drawRect(x, y, cx, cy);
}
*/

/*
void PeripheralDgr(QPainter &p, int x1, int cx, int y1, int cy, quint32 icolor, float db, float dg, float dr, quint8 nthick, quint8 flags, quint8 rndrect)
{
    /// initial value of each color
    float b =  icolor&0xff;
    float g = (icolor&0xff00)>>8;
    float r = (icolor&0xff0000)>>16;

    int cny = cy/2;
    int cnx = cx/2;
    int msz = qMin(cny, cnx);

    bool stoph = flags&0x01;
    quint8 pnattr = (flags&0x02)? Qt::SolidLine : Qt::NoPen;
    quint8 brattr = (flags&0x04)? Qt::SolidPattern : Qt::NoBrush;
    quint8 btdsn = flags&0xf0;
    QColor clr;

    /// degradetion of each color
    for(int sy=0, sz=0; sy<cny && nthick; ++sy, sz+=2, --nthick)
    {
        clr.setColor(QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)));

        if(btdsn==0x10)
        {
            p.setPen(QPen(QBrush(clr), 1, (Qt::PenStyle)pnattr));
            p.setBrush(QBrush(clr, (Qt::BrushStyle)brattr));
            p.drawRoundRect(x1+sy, y1+sy, cx-sz, cy-sz, rndrect, rndrect*((float)cx/cy));
            //p.drawRect(x1+sy, y1+sy, cx-sz, cy-sz);
        }
        else if(btdsn==0x20)
        {
            p.setPen(QPen(QBrush(clr), 2, (Qt::PenStyle)pnattr));
            p.setBrush(QBrush(clr, (Qt::BrushStyle)brattr));
            p.drawEllipse(QPoint(x1+cnx, y1+cny), msz-sy, msz-sy);
        }
        else
        {
            p.fillRect( x1+sy, y1+sy, cx-sz, cy-sz, clr );
        }

        if((r+=dr)>255) r=(stoph)?255:-255; //r=-255;
        if((g+=dg)>255) g=(stoph)?255:-255; //g=-255;
        if((b+=db)>255) b=(stoph)?255:-255; //b=-255;
    }
}
*/


void draw::RectsDgr(QPainter &p, int x1, int cx, int y1, int cy, quint32 icolor, float db, float dg, float dr, quint8 nthick, bool stoph, float stepx, float stepy, float stepcx, float stepcy)
{
    /// initial value of each color
    float b =  icolor&0xff;
    float g = (icolor&0xff00)>>8;
    float r = (icolor&0xff0000)>>16;

    float scx = cx;
    float scy = cy;
    //int max = qMin(cy/2, cx/2);
    //nthick  = qMin(max,nthick);

    /// degradetion of each color
    for(float sy=0, sx=0; nthick; sy+=stepy, sx+=stepx, --nthick)
    {
        p.fillRect( x1+sx, y1+sy, scx, scy, QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)) );
        scx -= stepcx;
        scy -= stepcy;

        if((r+=dr)>255) r=(stoph)?255:-255; //r=-255;
        if((g+=dg)>255) g=(stoph)?255:-255; //g=-255;
        if((b+=db)>255) b=(stoph)?255:-255; //b=-255;
    }
}


/*
void DrawRotatedText(QPainter &p, float degrees, int x, int y, const char* text)
{
    p.save();
    p.translate(x, y);
    p.rotate(degrees);
    p.drawText(0, 0, text);
    p.restore();
}
*/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
float sgm0[8]={0,0, 100,0, 80,20, 20,0};
float sgm1[8]={100,0, 100,100, 80,80, 80,20};
float sgm2[8]={100,100, 100,200, 80,180, 80,120};
float sgm3[8]={0,200, 100,200, 80,180, 20,180};
float sgm4[8]={0,100, 0,200, 20,180, 20,120};
float sgm5[8]={0,0, 0,100, 20,80, 20,20};
float sgm6[12]={10,100, 20,80, 80,80, 90,100, 80,110, 20,110};
//		0
//	5		1
//		6
//	4		2
//		3
//
//  Segment's Number
qint8 sgmnum[10]={0x3f, 0x6, 0x5a, 0x4f, 0x66, 0xd6, 0x7d, 0x7, 0x7f, 0x6f};
float *sgmptr[7];
qint8 sgmsz[7]={4,4,4,4,4,4,6};


void DrawSegment(QPainter &p, const char* str, qint16 x, qint16 y)
{
    sgmptr[0] = sgm0;
    sgmptr[1] = sgm1;
    sgmptr[2] = sgm2;
    sgmptr[3] = sgm3;
    sgmptr[4] = sgm4;
    sgmptr[5] = sgm5;
    sgmptr[6] = sgm6;

    //qint16 dx = x;
    //qint16 y = m_txt_y;

    p.setPen(0xff00);
    p.setBrush((QBrush)0xff00);

    for(qint16 i=0; str[i]!=0; i++)
    {
        qint8 n = str[i]-'0';

        x+=20;

        //p.setViewport(x+(i*20), y, 30, 30);
        //p.setWindow(0, 0, 200, 200);

        for(qint16 s=0; s<7; s++)	// load keys on/off state and color
        {
            if(sgmnum[n] & (1 << s))
            {
                float ar[20];
                for(qint16 d=0; d<sgmsz[s]*2; d+=2)
                {
                    ar[d]=(sgmptr[s][d]/10)+x;
                    ar[d+1]=(sgmptr[s][d+1]/10)+y;
                }

                p.drawPolygon((QPointF*)ar, sgmsz[s]);
            }
        }
    }
}
*/

/*
QPixmap *dgts;
//dgts = new QPixmap("digits.bmp");

void DrawSegment(QPainter &p, const char* str, qint16 x, qint16 y)
{
    //qint16 dx = x;
    //qint16 y = m_txt_y;

    for(qint16 i=0; str[i]!=0; i++)
    {
        qint8 n = str[i]-'0';
        p.drawPixmap(x, y, 40, 30, *dgts, n*60, 0, 60, 75);
        x+=40;
    }
}
*/
