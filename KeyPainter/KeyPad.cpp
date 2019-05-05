#include "KeyPad.h"
#include "draw_functions.h"
#include <QtGui>
#include <QMessageBox>
#include "qfile.h"


KeyPad::KeyPad(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, Qt::Tool)
{
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    //sendToObj = parent;
    kpd.nKeys = 0;
}

KeyPad::~KeyPad()
{
    free( pkey );
    delete m_xmp;
}

///////////////////////////////////////////////////////////////////
// KeyPad = new KeyPad(this);
// KeyPad->m_32switchesstate = 0x42;
// KeyPad->Ini("3Main.kpd", 0, 0, DISPLAY_CX, DISPLAY_CY);


bool KeyPad::ReplaceKeypad(const QByteArray &FileName)
{
    if(LoadKeyPad(FileName))
    {
        setKpdGeometry(m_wnd_x, m_wnd_y, m_wnd_cx, m_wnd_cy, m_Position);
        return true;
    }
    return false;
}


bool KeyPad::setKeypad(const QByteArray &FileName, const QString &FilePath)
{
    m_FilePath = FilePath;

    return LoadKeyPad(FileName);
}


bool KeyPad::LoadKeyPad(const QByteArray &FileName)
{
    QFile f_cnf(m_FilePath+FileName);

    if (f_cnf.open(QIODevice::ReadOnly))
    {
        f_cnf.read((char*)&kpd, sizeof(KEYHDR));
        kpd.nKeys = KeysAllocate(kpd.nKeys);
        f_cnf.read((char*)pkey, sizeof(KEY)*kpd.nKeys);
        f_cnf.close();

        //-+-+-+-+-+-+-+-+-+-+-+-+-// handle the switch keypads
        //if(is_switch_kpd)                                   //save if it was a switch_keypad (1st load is always false and m_FileName=empty)
        //{
        //    if(m_map.contains(m_FileName))
        //        m_map[m_FileName] = m_32switchesstate;      //save the states before leave
        //    else
        //        m_map.insert(m_FileName, m_32switchesstate);//save the name+states before leave
        //}

        m_FileName = FileName;
        //is_switch_kpd = isSwitchKeypad();                 //retrieve states if the name exist in the saved list
                                                            //else load the default states from kpd.data
        //m_32switchesstate = (is_switch_kpd)? (m_map.contains(FileName)? m_map.value(FileName):kpd.data) : 0;

        //-+-+-+-+-+-+-+-+-+-+-+-+-// initialize text fonts
        for(int n=0; n<4; ++n)
        {
            if(kpd.font[n].Size>0)
            {
                m_fnt[n].setFamily(kpd.font[n].Name);
                m_fnt[n].setPointSize(kpd.font[n].Size);
                m_fnt[n].setWeight(kpd.font[n].Weight);
                m_fnt[n].setItalic(kpd.font[n].attr&0x01);
            }
        }

        return true;
    }

    if(m_xmp==nullptr) m_xmp = new QPixmap(); // protects from crashing on paint
    QMessageBox::warning(nullptr, "?", FileName);
    return false;
}


int KeyPad::KeysAllocate(int maxkeys)
{
    if(maxkeys < kpd.nKeys) return maxkeys;
    /////////////////////
    if(pkey==nullptr)
    {
        pkeySel = pkey = (KEY*) calloc(maxkeys, sizeof(KEY));	// calloc() zero-initializes the buffer, while malloc() leaves the memory uninitialized.
    }
    else
    {
        pkeySel = pkey = (KEY*) realloc(pkey, sizeof(KEY)*maxkeys );
		// for builder
        if(kpd.nKeys<maxkeys) memset(&pkey[kpd.nKeys], 0, sizeof(KEY)*(maxkeys-kpd.nKeys));
    }
    /////////////////////
    return (pkey==nullptr)? 0 : maxkeys;
}


void KeyPad::setKpdGeometry(int wnd_x, int wnd_y, int wnd_cx, int wnd_cy, quint8 position)
{
    m_wnd_x = wnd_x;
    m_wnd_y = wnd_y;
    m_wnd_cx = wnd_cx;
    m_wnd_cy = wnd_cy;

    setPositionOnDisplay(position);
    NewSizes(m_wnd_cx, m_wnd_cy);
    setGeometry(m_wnd_x, m_wnd_y, m_wnd_cx, m_wnd_cy);
    Redraw();
}


void KeyPad::NewSizes(int wnd_cx, int wnd_cy)
{
    switch(kpd.type)
    {
    case 1: ResizeKeyGrid(wnd_cx, wnd_cy); break;   /// grid
    //case 0: ResizeKeyFree(wnd_cx, wnd_cy);        /// free zoom
    }

    delete m_xmp;

    switch(kpd.bckgrType)
    {
    case 3: /// keep image size
            m_xmp = new QPixmap(m_FilePath+kpd.Wallpaper);
            break;
    case 4: /// scale image to window size
            m_xmp = new QPixmap(m_FilePath+kpd.Wallpaper);
            *m_xmp = m_xmp->scaled(wnd_cx, wnd_cy, Qt::IgnoreAspectRatio);
            break;
    default:
            m_xmp = new QPixmap(wnd_cx, wnd_cy);
    }
}


void KeyPad::ResizeKeyGrid(qint16 tocx, qint16 tocy)
{
    int NewSzCol = (tocx-(kpd.margine_lf+kpd.margine_rt))/kpd.cols;
    int NewSzRow = (tocy-(kpd.margine_up+kpd.margine_dn))/kpd.rows;

    int col, row;

    ///////////////////// position the keys on grid
    for(int i=0; i<kpd.nKeys; ++i)
    {
        idx_to_cr(i, col, row);

        pkey[i].x = (NewSzCol*col)+kpd.margine_lf;
        pkey[i].y = (NewSzRow*row)+kpd.margine_up;
        pkey[i].cx= NewSzCol-kpd.space_h;
        pkey[i].cy= NewSzRow-kpd.space_v;

        ///////////// multiply the cell size * color[2] upper byte
        if(pkey[i].color[2]&0xff000000)
        {
            //pkey[i].cx *= ((pkey[i].color[2]>>24)&0xf)+1; // 0x0f000000
            //pkey[i].cy *= ((pkey[i].color[2]>>28)&0xf)+1; // 0xf0000000
            pkey[i].cx = (NewSzCol * (((pkey[i].color[2]>>24)&0xf)+1)) - kpd.space_h; // 0x0f000000
            pkey[i].cy = (NewSzRow * (((pkey[i].color[2]>>28)&0xf)+1)) - kpd.space_v; // 0xf0000000
        }
    }
}


void KeyPad::ResizeKeyFree(qint32 tocx, qint32 tocy)
{
    m_newratio_cx = float(kpd.cx)/tocx; // opposite for builder
    m_newratio_cy = float(kpd.cy)/tocy; // re-position by x/y display size ratio

    ////////////////////////////// key.color[2] (4th byte) = button's re-position
    for(int i=0; i<kpd.nKeys; ++i)
    {
        quint32 dim = pkey[i].color[2]&0xff000000;

        if(dim)
        {  
            if(dim&0x01000000)         // grow x,cx to window width (unlock from left side)
            {
                pkey[i].x  *= m_newratio_cx;
                pkey[i].cx *= m_newratio_cx;
            }
            else if(dim&0x02000000)    // lock x from right side
            {
                //pkey[i].x = (tocx - kpd.cx) + pkey[i].x;
                pkey[i].x = (kpd.cx - tocx) + pkey[i].x; // (keypainter only)
            }
            else if(dim&0x40000000)    // move x to window width
            {
                pkey[i].x *= m_newratio_cx;
            }

            if(dim&0x04000000)         // grow y,cy to window height (unlock from upper side)
            {
                pkey[i].y  *= m_newratio_cy;
                pkey[i].cy *= m_newratio_cy;
            }
            else if(dim&0x08000000)    // lock from down side
            {
                //pkey[i].y = (tocy - kpd.cy) + pkey[i].y;
                pkey[i].y = (kpd.cy - tocy) + pkey[i].y; // (keypainter only)
            }            
            else if(dim&0x80000000)    // move y to window width
            {
                pkey[i].y *= m_newratio_cy;
            }

            if(dim&0x10000000)         // lock x2 from right side
            {
                //pkey[i].cx = (tocx - kpd.cx) + pkey[i].cx;
                pkey[i].cx = (kpd.cx - tocx) + pkey[i].cx; // (keypainter only)
            }

            if(dim&0x20000000)         // lock y2 from right side
            {
                //pkey[i].cy = (tocy - kpd.cy) + pkey[i].cy;
                pkey[i].cy = (kpd.cy - tocy) + pkey[i].cy; // (keypainter only)
            }

            //pkey[i].color[2] &= 0x00ffffff;       // reset the position flags of color_val
        }
    }
}


void KeyPad::setPositionOnDisplay(quint8 position)
{
    /*
    if(kpd.position)
    {
        if(kpd.position&0x01) m_wnd_x=kpd.x;
        if(kpd.position&0x02) m_wnd_y=kpd.y;
        if(kpd.position&0x04) { m_wnd_cx=DISPLAY_CX-m_wnd_x; m_newratio_cx=float(m_wnd_cx)/kpd.cx; } else m_wnd_cx=kpd.cx;
        if(kpd.position&0x08) { m_wnd_cy=DISPLAY_CY-m_wnd_y; m_newratio_cy=float(m_wnd_cy)/kpd.cy; } else m_wnd_cy=kpd.cy;

        if(kpd.position&0x10) raise();
        if(kpd.position&0x20) lower();
    }
    else
    */
    {
        m_Position = position;

        if(position & KPDWND_CX)    m_newratio_cx = float(m_wnd_cx)/kpd.cx; // m_wnd_cx = m_wnd_cx;
        if(position & KPDWND_CY)    m_newratio_cy = float(m_wnd_cy)/kpd.cy; // m_wnd_cy = m_wnd_cy;
        if(position & KPDKPD_CX)    {m_newratio_cx = 1; m_wnd_cx = kpd.cx;} /// keep keypad size
        if(position & KPDKPD_CY)    {m_newratio_cy = 1; m_wnd_cy = kpd.cy;}
        if(position & KEYPAD_UP)    m_wnd_y = 0;
        if(position & KEYPAD_DN)	m_wnd_y = m_dsp_cy-m_wnd_cy;
        if(position & KEYPAD_LT)	m_wnd_x = 0;
        if(position & KEYPAD_RT)	m_wnd_x = m_dsp_cx-m_wnd_cx;
    }
}


// --------------------------- DRAW ------------------------------


void KeyPad::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, *m_xmp); // (0, 0, *m_xmp, m_or_x, m_or_y, -1, -1)

    //if(m_push)
    //{
 	//	p.setCompositionMode(QPainter::CompositionMode_HardLight);	// CompositionMode_Difference
    //    p.fillRect(pkeySel->x, pkeySel->y, pkeySel->cx, pkeySel->cy, 0x555555);
    //    m_push = false;
    //}
}


void KeyPad::Redraw()
{
    m_pnt.begin(m_xmp);

    //// BACKGROUND
    switch(kpd.bckgrType)
    {
    case 0:
    {
        m_pnt.fillRect(0, 0, m_wnd_cx, m_wnd_cy, kpd.color[0]);
        break;
    }
    case 1:
    {
        QLinearGradient gradient(kpd.attr[0], kpd.attr[1], kpd.attr[2], kpd.attr[3]);
        gradient.setColorAt(qreal(kpd.attr[5])/100, kpd.color[0]);
        if(kpd.attr[6]) gradient.setColorAt(qreal(kpd.attr[6])/100, kpd.color[1]);
        if(kpd.attr[7]) gradient.setColorAt(qreal(kpd.attr[7])/100, kpd.color[2]);
        m_pnt.fillRect(0, 0, m_wnd_cx, m_wnd_cy, QBrush(gradient));
        break;
    }
    case 2:
    {
        ////////////////////////          center point x/y                  radial,                 focus point x/y
        QRadialGradient gradient(kpd.attr[0], kpd.attr[1], kpd.attr[4], kpd.attr[2], kpd.attr[3]);
        gradient.setColorAt((float)kpd.attr[5]/100, kpd.color[0]);
        if(kpd.attr[6]) gradient.setColorAt((float)kpd.attr[6]/100, kpd.color[1]);
        if(kpd.attr[7]) gradient.setColorAt((float)kpd.attr[7]/100, kpd.color[2]);
        m_pnt.fillRect(0, 0, m_wnd_cx, m_wnd_cy, QBrush(gradient));
		break;
    }
    case 5:
    {
        if(kpd.attr[5]==0 || kpd.attr[5]>180) kpd.attr[5]=5; // protect to not crash
        m_pnt.fillRect(0, 0, m_wnd_cx, m_wnd_cy, (QGradient::Preset)kpd.attr[5]);
		break;
    }
    case 6:
    {
        m_pnt.fillRect(0, 0, m_wnd_cx, m_wnd_cy, kpd.color[0]);
        m_pnt.fillRect(0, 0, m_wnd_cx, m_wnd_cy, QBrush(kpd.color[1], (Qt::BrushStyle)kpd.attr[6]));
    }
    /*
    case 7:
    {
        //or: m_xmp->load(kpd.Wallpaper);
        //or: DrawBmp( 0, 0, m_wnd_cx, m_wnd_cy, 0, kpd.Wallpaper);
        //or: m_pnt.drawPixmap(0, 0, *m_xmp_wall);
    }
    */
    }

    //// DRAW KEYS    
    for(int idx=0; idx<kpd.nKeys; ++idx)
    {
        DrawKey(pkey+idx);
    }

    m_pnt.end();
    m_push = false;
    update();
}


void KeyPad::DrawKey(KEY *drkey)
{
    qint16 x = drkey->x;
    qint16 y = drkey->y;
    qint16 cx= drkey->cx;
    qint16 cy= drkey->cy;
    /////////////////////

    switch((drkey->btn))
    {
    case 0x1:	DrawButtonFl( x, y, cx, cy, drkey);	break;
    case 0x2:	DrawButtonRc( x, y, cx, cy, drkey);	break;
    case 0x3:	DrawButtonRd( x, y, cx, cy, drkey);	break;
    case 0x4:	DrawButtonEl( x, y, cx, cy, drkey);	break;
    case 0x5:	DrawButtonAr( x, y, cx, cy, drkey);	break;
    case 0x6:	DrawButtonAv( x, y, cx, cy, drkey);	break;
    case 0x7:	DrawButtonGr( x, y, cx, cy, drkey);	break;
    case 0x8:	DrawButtonGc( x, y, cx, cy, drkey);	break;
    case 0x0:   DrawBoundaries( x, y, cx, cy); break;
    default:    DrawButtonFl( x, y, cx, cy, drkey); // Widgets
    }

    if(drkey->nfnt&0xc) DrawBmp( x, y, cx, cy, drkey->nfnt&0xc, drkey->mem);
    DrawTxt( x, y, cx, cy, drkey, drkey->color[0]);

    ///////////////////// pointer to function
    /*
    (this->*pDrawButton[idx])(x, y, cx, cy, idx);

    if(Key[idx].nfnt>3)
        (this->*pDrawBmp)(x, y, cx, cy, (Key[idx].attr&0x40), Key[idx].mem);
    else
        (this->*pDrawText)(x, y, cx, cy, idx, pkey[idx].color[0]);
    */
}

/*
void KeyPad::RedrawKey(int idx)
{
    if(idx<kpd.nKeys)
    {
        m_pnt.begin(m_xmp);
        m_pnt.fillRect(pkey[idx].x, pkey[idx].y, pkey[idx].cx, pkey[idx].cy, kpd.color[0]); // for transparent key with new text or key hide from KeyPadX
        DrawKey(pkey+idx);
        m_pnt.end();
        m_push = false;
        update();
    }
}
*/

// -------------------------- DRAW KEY ---------------------------


void KeyPad::DrawButtonFl(int x, int y, int cx, int cy, KEY *drkey)
{
    if(quint8(drkey->attr2)<51)
        m_pnt.fillRect(x, y, cx, cy, QBrush(drkey->color[1], Qt::BrushStyle(drkey->attr2)));
    else
    	m_pnt.fillRect(x, y, cx, cy, QGradient::Preset(((quint8)drkey->attr2-50))); // (Qt5.12 - https://webgradients.com/)
}


void KeyPad::DrawButtonRc(int x, int y, int cx, int cy, KEY *drkey)
{
    m_pnt.setPen(drkey->color[1]);
    m_pnt.drawLine(x, y, x+cx, y);
    m_pnt.drawLine(x, y, x, y+cy);
    m_pnt.setPen(drkey->color[2]);
    m_pnt.drawLine(x, y+cy, x+cx, y+cy);
    m_pnt.drawLine(x+cx, y, x+cx, y+cy);
}


void KeyPad::DrawBoundaries(int x, int y, int cx, int cy)
{
    m_pnt.setPen(QPen(Qt::DashLine));
    m_pnt.setBrush(Qt::NoBrush);
    m_pnt.drawRect(x, y, cx, cy);
}


/* * * * * * * * * * * * * *
 * Button Styles
 *
 *
 *  attr1:   round the corners
 *  ---------------
 *  attr2:   Qt::BrushStyle
 *  ---------------
 *  attr3:   Qt::PenStyle
 *  ---------------
 *  attr4:   line width
 *
 * * * * * * * * * * * * * */

void KeyPad::DrawButtonRd(int x, int y, int cx, int cy, KEY *drkey)
{
    m_pnt.setPen(QPen(QBrush(drkey->color[2]), drkey->attr4, Qt::PenStyle(drkey->attr3)));

    if(quint8(drkey->attr2)<51)
        m_pnt.setBrush(QBrush(drkey->color[1], Qt::BrushStyle(drkey->attr2)));
    else
        m_pnt.setBrush(QBrush(QGradient(QGradient::Preset((quint8(drkey->attr2)-50))))); // (Qt5.12 - https://webgradients.com/)

    m_pnt.drawRoundedRect(x, y, cx, cy, drkey->attr1, drkey->attr1);
}


void KeyPad::DrawButtonEl(int x, int y, int cx, int cy, KEY *drkey)
{
    m_pnt.setPen(QPen(QBrush(drkey->color[2]), drkey->attr4, Qt::PenStyle(drkey->attr3)));
    m_pnt.setBrush(QBrush(drkey->color[1], Qt::BrushStyle(drkey->attr2)));
    m_pnt.drawEllipse(x, y, cx-1, cy-1);
}


/* * * * * * * * * * * * * *
 * Gradient design button
 *
attr1:   draw direction  (bit 1-4)
----------------
0001  x1=go left
0010  y1=go down
0100  x2=go right
1000  y2=go down

attr1:   color up/midle/down  (bit 5-6)
----------------
0000.0000  color[0]/color[1]/color[2] (text has the upper color)
0011.0000  color[2]/color[1]/color[0] (text has the down color)
0010.0000  color[2]/color[1]/color[2] (text has free color)

attr2:   midle point posittion  (n=0-100)
----------------
000/2 = 0.0 midle point go up
050/2 = 0.5 midle point go midle
100/2 = 1.0 midle point go down
* * * * * * * * * * * * * * */

void KeyPad::DrawButtonGr(int x, int y, int cx, int cy, KEY *drkey)
{
    quint8 direction = drkey->attr1;
    QLinearGradient gradient(x+((direction&0x1)?cx:0), y+((direction&0x2)?cy:0), x+((direction&0x4)?cx:0), y+((direction&0x8)?cy:0));
    gradient.setColorAt(0, drkey->color[2]);
    gradient.setColorAt(qreal(drkey->attr2)/100, drkey->color[1]);
    gradient.setColorAt(qreal(drkey->attr3)/100, drkey->color[1]);
    gradient.setColorAt(1, drkey->color[2]);
    m_pnt.fillRect(x, y, cx, cy, QBrush(gradient));
}


/* * * * * * * * * * * * * *
 * RadialGradient design button
 *
 *
 *  attr1:   radial of the circle  (n*2)
 *  ---------------
 *  attr2:   focus (-/+)x point in the circle  (n*2)
 *  ---------------
 *  attr3:   focus (-/+)y point in the circle  (n*2)
 *
 *
 * * * * * * * * * * * * * */

void KeyPad::DrawButtonGc(int x, int y, int cx, int cy, KEY *drkey)
{
    //////////////////////// center point x/y         radial,          focus point x/y
    QRadialGradient gradient(x+(cx/2), y+(cy/2), drkey->attr1*2, x+(cx/2)+(drkey->attr2*2), y+(cy/2)+(drkey->attr3*2));
    gradient.setColorAt(0, drkey->color[1]);
    gradient.setColorAt(1, drkey->color[2]);

    //m_pnt.fillRect(x, y, cx, cy, QBrush(gradient));

    m_pnt.setPen(Qt::NoPen);
    m_pnt.setBrush(QBrush(gradient));
    m_pnt.drawEllipse(x, y, cx, cy);
}


void KeyPad::DrawButtonAv(int x, int y, int cx, int cy, KEY *drkey)
{
    float db = (drkey->color[2]&0xff);
    float dg = (drkey->color[2]&0xff00)>>8;
    float dr = (drkey->color[2]&0xff0000)>>16;

    //// d/10 = 0.1-25.5 //// d/20 = 0.05-12.75 //// d/15 = 0.06666-17
    //DrawRainbowV(x, cx, y, y+cy, drkey->color[1], (db/m_newratio_cy)/10, (dg/m_newratio_cy)/10, (dr/m_newratio_cy)/10, 1);
    draw::RainbowV(m_pnt, x, cx, y, y+cy, drkey->color[1], db/10, dg/10, dr/10, 1);
    //draw::RainbowV(m_pnt, x, cx, y, y+cy, drkey->color[1], (db/m_newratio_cy)/10, (dg/m_newratio_cy)/10, (dr/m_newratio_cy)/10, 1);
}


void KeyPad::DrawButtonAr(int x, int y, int cx, int cy, KEY *drkey)
{
    float db = (drkey->color[2]&0xff);
    float dg = (drkey->color[2]&0xff00)>>8;
    float dr = (drkey->color[2]&0xff0000)>>16;

    //// d/10 = 0.1-25.5 //// d/20 = 0.05-12.75 //// d/15 = 0.0666666-17
    draw::RectsDgr(m_pnt, x, cx, y, cy, drkey->color[1], db/10, dg/10, dr/10, quint8(drkey->attr1), (drkey->attr2&0x1), float(((drkey->attr2&0x10)?drkey->attr4:drkey->attr3))/10, float(((drkey->attr2&0x20)?drkey->attr4:drkey->attr3))/10, float(((drkey->attr2&0x40)?drkey->attr3:drkey->attr4))/10, float(((drkey->attr2&0x80)?drkey->attr3:drkey->attr4))/10);
    //DrawRectsDgr(x, cx, y, cy, drkey->color[1], db/10, dg/10, dr/10, drkey->attr1, (drkey->attr2&0x1), (float)((drkey->attr2&0x10)?drkey->attr4:drkey->attr3)/10, (float)((drkey->attr2&0x20)?drkey->attr4:drkey->attr3)/10, (float)((drkey->attr2&0x40)?drkey->attr3:drkey->attr4)/10, (float)((drkey->attr2&0x80)?drkey->attr3:drkey->attr4)/10);
}

/*
//// δημιουργεί ομόκεντρα τετράγωνα διαβάθμισης χρωμάτων
void KeyPad::DrawRectsDgr(int x1, int cx, int y1, int cy, quint32 icolor, float db, float dg, float dr, quint8 nthick, bool stoph, float stepx, float stepy, float stepcx, float stepcy)
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
        m_pnt.fillRect( x1+sx, y1+sy, scx, scy, QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)) );
        scx -= stepcx;
        scy -= stepcy;

        if((r+=dr)>255) r=(stoph)?255:-255; //r=-255;
        if((g+=dg)>255) g=(stoph)?255:-255; //g=-255;
        if((b+=db)>255) b=(stoph)?255:-255; //b=-255;
    }
}
*/

/*
void KeyPad::DrawPeripheralDgr(int x1, int cx, int y1, int cy, quint32 icolor, float db, float dg, float dr, quint8 nthick, quint8 flags, quint8 rndrect)
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

    /// degradetion of each color
    for(int sy=0, sz=0; sy<cny && nthick; ++sy, sz+=2, --nthick)
    {
        if(btdsn==0x10)
        {
            m_pnt.setPen(QPen(QBrush(QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b))), 1, (Qt::PenStyle)pnattr));
            m_pnt.setBrush(QBrush(QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)), (Qt::BrushStyle)brattr));
            m_pnt.drawRoundRect(x1+sy, y1+sy, cx-sz, cy-sz, rndrect, rndrect*((float)cx/cy));
        }
        else if(btdsn==0x20)
        {
            m_pnt.setPen(QPen(QBrush(QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b))), 2, (Qt::PenStyle)pnattr));
            m_pnt.setBrush(QBrush(QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)), (Qt::BrushStyle)brattr));
            m_pnt.drawEllipse(QPoint(x1+cnx, y1+cny), msz-sy, msz-sy);
        }
        else
        {
            m_pnt.fillRect( x1+sy, y1+sy, cx-sz, cy-sz, QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)) );
        }

        if((r+=dr)>255) r=(stoph)?255:-255; //r=-255;
        if((g+=dg)>255) g=(stoph)?255:-255; //g=-255;
        if((b+=db)>255) b=(stoph)?255:-255; //b=-255;
    }
}
*/


//// δημιουργεί κάθετη διαβάθμιση χρωμάτων
/*
    it works both
    -------------
    1) up-to-down	>	RainbowH(pDC, 0, 200, 0, 100, icolor, dcolor, 2)
    2) down-to-up	>	RainbowH(pDC, 0, 200, -100, 0, icolor, dcolor, -2)
*
void KeyPad::DrawRainbowV(int x1, int cx, int y1, int y2, quint32 icolor, float db, float dg, float dr, int stepy)
{
    // initial value of each color
    float b =  icolor&0xff;
    float g = (icolor&0xff00)>>8;
    float r = (icolor&0xff0000)>>16;
    // degradetion of each color
    for(int y3=y1; y3<y2; y3+=labs(stepy))
    {
        m_pnt.fillRect( x1, labs(y3), cx, stepy, QColor((quint8)labs(r), (quint8)labs(g), (quint8)labs(b)) );

        if((r+=dr)>=255) r=-255;
        if((g+=dg)>=255) g=-255;
        if((b+=db)>=255) b=-255;
    }
}
*/

// ---------------------------------------------------------------


quint16 KeyPad::TxtAlign(quint8 flags)
{
    return ((flags&0x3)? (flags&0x3) : Qt::AlignHCenter) | ((flags&0xc)? (flags&0xc)<<3 : Qt::AlignVCenter);
}


void KeyPad::DrawTxt(int x, int y, int cx, int cy, KEY *drkey, quint32 clr)
{
    m_pnt.setPen(clr);
    m_pnt.setFont(m_fnt[drkey->nfnt&0x3]);
    ////////////// kpd has 8bit Greek LOCAL encoding language
    m_pnt.drawText(QRect(x+4, y, cx-8, cy), TxtAlign(drkey->nfnt>>4)|Qt::TextWordWrap, drkey->text);
}


void KeyPad::DrawBmp(int x, int y, int cx, int cy, quint8 scale, const char* FileName)
{
    QPixmap pxmp(m_FilePath+FileName);

    if(scale==0x4)      // draw the picture as it is
    {
        x+=(cx-pxmp.width())>>1; // set bitmap to center
        y+=(cy-pxmp.height())>>1;// fast Multiplication or Division by 2
        m_pnt.drawPixmap(x, y, pxmp);
    }
    else if(scale==0x8) // draw scaling according key size
    {
        m_pnt.drawPixmap(x, y, cx, cy, pxmp, 0, 0, pxmp.width(), pxmp.height());
    }
    else if(scale==0xc) // draw scaling the key size keeping the horizontal aspect ratio
    {
        if(pxmp.height()==0) return;
        float ratio = float(pxmp.width())/pxmp.height();
        m_pnt.drawPixmap(x, y, cy*ratio, cy, pxmp, 0, 0, pxmp.width(), pxmp.height());
    }
}


// ---------------------------------------------------------------


/*
void KeyPad::SetKeyColor(int idx, long clr_fll, long clr_out)
{
    if(clr_out>=0)	pkey[idx].color[2] = clr_out;
    if(clr_fll>=0)	pkey[idx].color[1] = clr_fll;

    m_pnt.begin(m_xmp);
    DrawKey(idx);
    m_pnt.end();
    update();
}
*/


/*  -----------------------------

    col = idx%kpd->col;
    row = idx/kpd->col;
    idx = ((y/m_szRow)*kpd->col)+(x/m_szCol);

    -----------------------------

    KEY STATE (KEY.onoff)
    0000 = push button
    0001 = switch on/off
    0111 = local groups of radio buttons
    1111 = global groups of radio buttons for all keypads

    -----------------------------
*/

/*
bool KeyPad::isSwitchKeypad()
{
    for(int idx=0; idx<kpd.nKeys; ++idx)
    {
        if(pkey[idx].onoff) return true;
    }
    return false;
}


void KeyPad::SetKeysState(quint32 switches32state)
{
    m_pnt.begin(m_xmp);

    for(short idx=0; idx<kpd.nKeys; ++idx)	// set on/off color only on group buttons
    {
        if(pkey[idx].onoff)
        {
            DrawTxt( pkey[idx].x, pkey[idx].y, pkey[idx].cx, pkey[idx].cy, pkey+idx, (switches32state & (1<<idx))?pkey[idx].color[0]:kpd.color[2]);
        }
    }

    m_pnt.end();
}


// ------------------ on/off type switches ------------------


void KeyPad::SwitchOnOff(quint8 idx)
{    
    //// flip state
    m_32switchesstate ^= 1 << idx;
    //// set off: m_32switchesstate &= ~(1 << idx);
    //// set on : m_32switchesstate |= 1 << idx;
    //// print new txt state in screen memory
    m_pnt.begin(m_xmp);
    DrawTxt( pkey[idx].x, pkey[idx].y, pkey[idx].cx, pkey[idx].cy, pkey+idx, (m_32switchesstate&(1<<idx))? pkey[idx].color[0] : kpd.color[2]);
    m_pnt.end();
}
*/
/*
void KeyPad::SwitchSet(bool on, quint8 idx)
{
    if(on)
        m_32switchesstate |= 1 << idx;
    else
        m_32switchesstate &= ~(1 << idx);

    //// print new txt state in screen memory
    m_pnt.begin(m_xmp);
    DrawTxt( pkey[idx].x, pkey[idx].y, pkey[idx].cx, pkey[idx].cy, pkey+idx, (m_32switchesstate&(1<<idx))? pkey[idx].color[0] : kpd.color[2]);
    m_pnt.end();
}
*/

// -------------------- group type keys ---------------------

/*
void KeyPad::SwitchGroup(quint8 group_id, quint8 on_idx)
{
    m_pnt.begin(m_xmp);

    //// set selected button to -on- state
    m_32switchesstate |= 1 << on_idx;

    for(quint8 idx=0; idx<kpd.nKeys; ++idx)
    {
        if(pkey[idx].onoff == group_id)
        {
            //// set the others to -off- state
            if(on_idx!=idx) m_32switchesstate &= ~(1 << idx);

            DrawKey(pkey+idx, (m_32switchesstate&(1<<idx))); // redraw the background for better text
          //DrawTxt(pkey[idx].x, pkey[idx].y, pkey[idx].cx, pkey[idx].cy, pkey+idx, (set_on_idx==idx)? pkey[idx].color[0]:kpd.color[2]);
        }
    }

    m_pnt.end();
    update();
}
*/

// --------------------------------------------------------

/*
void KeyPad::buttonPush()
{
    m_push = true;
    update(pkeySel->x, pkeySel->y, pkeySel->cx, pkeySel->cy);
}


void KeyPad::buttonRelease()
{
    update(pkeySel->x, pkeySel->y, pkeySel->cx, pkeySel->cy);
}
*/

/*
void KeyPad::buttonPush()
{
    m_push = true;
    update(pkeySel->x, pkeySel->y, pkeySel->cx, pkeySel->cy);
}


void KeyPad::buttonRelease()
{
    if(pkeySel->onoff)
    {
        if(pkeySel->onoff == 1)     // switch on/off (show now the color)
        {
            SwitchOnOff(m_idx);
        }
        else                        // radio group
        {
            SwitchGroup(pkeySel->onoff, m_idx);
            return;
        }
    }

    update(pkeySel->x, pkeySel->y, pkeySel->cx, pkeySel->cy);
}
*/


/*
 *   col = pnt.x/m_szCol;
 *   row = pnt.y/m_szRow;
 *   idx = (row*kpd.col)+col;
 *
int KeyPad::idx_from_pos(int x, int y)
{
    return ((y/m_szRow)*kpd.col)+(x/m_szCol);
}
*/


int KeyPad::idx_from_pos(int x, int y)
{
    for(int i=kpd.nKeys-1; i>=0; --i)   // reverce for overlaped visible buttons
    {
        if(//pkey[i].oper &&            // remove for builder
            x > pkey[i].x && x < pkey[i].x+pkey[i].cx &&
            y > pkey[i].y && y < pkey[i].y+pkey[i].cy ) return i;
    }

    return -1;
}


int KeyPad::idx_from_id(int id)
{
    for(int i=kpd.nKeys-1; i>=0; --i) // reverce for overlaped visible buttons
    {
        if( pkey[i].cmd == id ) return i;
    }

    return -1;
}


void KeyPad::idx_to_cr(int idx, int &col, int &row)
{
    if(kpd.subtp)   // vertical scan of buttons
    {
        col = idx/kpd.rows;
        row = idx%kpd.rows;
    }
    else            // horizontal scan of buttons
    {
        col = idx%kpd.cols;
        row = idx/kpd.cols;
    }
}


// ------------------------ MOUSE --------------------------
// long nExit = (long)*((long*)pkeySel->data);
// if (nExit==KEYPAD_DELETE)					// Delete Keypad ("DEL"=4998468)
// if (nExit==KEYPAD_HIDE)						// Hide	  Keypad ("HID"=4475208)

/*
void KeyPad::mousePressEvent(QMouseEvent *ev)
{
    ///////////KEY-NUMBER
    m_idx=idx_from_pos(ev->pos().x(), ev->pos().y());
    if (m_idx<0) return;
    pkeySel = pkey+m_idx;
    /////////////////////

    if (pkeySel->oper>0) buttonPush(); else return;
    if (!(pkeySel->oper&0x08)) mouseOperation(pkeySel->oper);   // send message Key Down
}


void KeyPad::mouseReleaseEvent(QMouseEvent *)
{
    if (m_idx<0) {emit KeyPressedMessage(0,0,0); return;}                                        // out of keys
    if (pkeySel->oper>0) buttonRelease(); else return;
    if (pkeySel->oper&0x08) mouseOperation(pkeySel->oper);      // send message Key Up
}


////////////////////////////////////////
////    oper = 1111 1111 bits
////                 --- message id
////                -    message up/down
////           ----      KeyPad change
void KeyPad::mouseOperation(quint8 oper)
{
    switch(oper&0x07)                           // Message
    {
    case 0x01: emit KeyPressedMessage(oper, pkeySel->cmd, pkeySel); break;
    case 0x02: QMetaObject::invokeMethod(sendToObj, pkeySel->text+65, Qt::DirectConnection); break;
    case 0x03: QMetaObject::invokeMethod(sendToObj, pkeySel->text+65, Qt::DirectConnection, Q_ARG(long, pkeySel->cmd), Q_ARG(long, pkeySel->val), Q_ARG(const char*, pkeySel->data));
    }

    switch(oper&0xf0)                           // KeyPad Change
    {
    case 0x10: ReplaceKeypad(pkeySel->data); break;
    case 0x20: ReplaceKeypad(m_FileNameFirst);
    //case 0x30: //deleteLater(); break;        // delete outside for setting pointer=0
    //case 0x40: //hide(); break;
    }
}
*/


///////////////////////////////////////////////////////////////////////////////////
//V6 10/06/2016 biger text & correct DrawButtonAv() degradetion*10 & new design button DrawRainbowR()
//V5 (free key map) (has a shorter header:  25 keys 5x5 file size V5=2560  v4=3120)
//V4 (new font structure from 60 to 44 bytes) (key cmd from qint32 to qint16, making 2 new qint8 attr3&attr4) (key fonts from 2 to 4 optionts) (2 new pad backgrounds)
//V3 (come from AirArt Navigation)
//struct KEY{char text[30]; char mem[15]; char data[15]; qint16 lp; qint8 attr1; qint8 attr2; qint8 attr3; qint8 attr4; qint16 cmd; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint8 btn; quint8 nfnt; quint8 onoff; quint8 oper;};
//struct KEY{char text[30]; char mem[15]; char data[15]; qint16 lp; qint8 attr1; qint8 attr2; qint8 attr3; qint8 attr4; qint16 cmd; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint8 btn; quint8 nfnt; quint8 onoff; quint8 oper;};
//struct KEY{char text[30]; char mem[15]; char data[15]; qint16 lp; qint8 attr1; qint8 attr2;                           qint32 cmd; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint8 btn; quint8 nfnt; quint8 onoff; quint8 oper;};

//struct KEYHDR{quint8 type; quint8 subtp; quint8 cols; quint8 rows; qint8 margine_lf; qint8 margine_up; qint8 margine_rt; qint8 margine_dn; qint16 orX; qint16 orY; qint16 cx; qint16 cy; quint32 color[4]; FONTSET2 font[4]; quint8 bckgrType; quint8 nKeys; qint16 attr[10]; qint32 data; char Wallpaper[40];};
//struct KEYHDR{quint8 type; quint8 subtp; quint8 row; quint8 col;  qint8 offlf;  qint8 offrt; qint8 offup;  qint8 offdn;  qint16 cx; qint16 cy; quint8 spx; quint8 spy; quint32 color[4]; FONTSET font[4]; quint8 backpaint; quint8 attr1; qint16 attr[30]; qint32 msg; qint32 data; char text[40]; char BmpKeypad[40];};
//struct KEYHDR{quint8 type; quint8 subtp; quint8 row; quint8 col; quint8 offlf; quint8 offrt; quint8 offup; quint8 offdn; qint16 cx; qint16 cy; quint8 spx; quint8 spy; quint32 color[4]; lgf fnt[4];                                                       qint32 msg; qint32 data; char text[40]; char BmpKeypad[40];};


/*
 *  CONVERT Keypad V5 To Keymap V6
 *
/////////////////////////////////////////////////////////////////
// new version 10/06/2016 correct DrawButtonAv() degradetion * 10
for(int idx=0; idx<key.kpd.nKeys; idx++)
{
    if(key.pkey[idx].btn==0x6) // DrawButtonAv()
    {
        quint32 db = (key.pkey[idx].color[2]&0xff);
        quint32 dg = (key.pkey[idx].color[2]&0xff00)>>8;
        quint32 dr = (key.pkey[idx].color[2]&0xff0000)>>16;

        db = (db*10)&0xff;
        dg = (dg*10)&0xff;
        dr = (dr*10)&0xff;

        key.pkey[idx].color[2] = db | (dg<<8) | (dr<<16);
    }
}
key.Redraw();
/////////////////////////////////////////////////////////////////
*/


/* FROM V5 to V6
 * -------------
 *
 *
    oper from
    "Message Cmd Down","Message Cmd Up","- - - -","Keypad Change","- - - -","Title (not push button)"};
    to
    "Title (not button)","Message Cmd Down","Message Cmd Up","Keypad Change"
 *
 *
    struct KEYNEW{char text[80]; char mem[25]; char data[15]; qint32 cmd; qint32 id; qint16 x; qint16 y; qint16 cx; qint16 cy; quint8 btn; quint8 oper; quint8 onoff; quint8 nfnt; qint8 attr1; qint8 attr2; qint8 attr3; qint8 attr4; quint32 color[3];};
    struct KEYOLD{char text[30]; char mem[15]; char data[15]; qint16 lp; qint8 attr1; qint8 attr2; qint8 attr3; qint8 attr4; qint16 cmd; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint8 btn; quint8 nfnt; quint8 onoff; quint8 oper;};
 */
/*
void ConvertBtn(KEYOLD *key1, KEYNEW *key2)
{
    memset(key2, 0, sizeof(KEYNEW));
    /////////////////////////////////// COPY KEY DATA
    key2->attr1 = key1->attr1;
    key2->attr2 = key1->attr2;
    key2->attr3 = key1->attr3;
    key2->attr4 = key1->attr4;

    key2->cmd = key1->cmd;
    key2->id = key1->lp;
    key2->btn = key1->btn;
    key2->onoff = key1->onoff;
    key2->nfnt = key1->nfnt;

    switch(key1->oper)
    {
    case 0: key2->oper = 1; break;
    case 1: key2->oper = 2; break;
    case 3: key2->oper = 3; break;
    case 5: key2->oper = 0; break;
    }

    key2->x = key1->x;
    key2->y = key1->y;
    key2->cx = key1->cx;
    key2->cy = key1->cy;

    key2->color[0] = key1->color[0];
    key2->color[1] = key1->color[1];
    key2->color[2] = key1->color[2];

    strcpy(key2->text, QString::fromLocal8Bit(key1->text).toUtf8().constData());
    strcpy(key2->data, key1->data);
    strcpy(key2->mem, key1->mem);
}

void ConvertFile(const QString &filepath)
{
    //QMessageBox::information(0, "", QString::asprintf("size old %d    size new %d", sizeof(KEYOLD), sizeof(KEYNEW)));

    QFile f_cnf(filepath);
    if (f_cnf.open(QIODevice::ReadOnly))
    {
        KEYHDR  kh1;
        KEYOLD  ky1[200];
        KEYNEW  ky2[200];

        f_cnf.read((char*)&kh1, sizeof(KEYHDR));
        f_cnf.read((char*)ky1, sizeof(KEYOLD)*kh1.nKeys);
        f_cnf.close();

        /////////////////////////////
        for(int i=0; i<kh1.nKeys; i++)
        {
            ConvertBtn(ky1+i, ky2+i);
        }
        /////////////////////////////
        {
            QFile f_cnn(filepath);
            if (f_cnn.open(QIODevice::WriteOnly))
            {
                f_cnn.write((char*)&kh1, sizeof(KEYHDR));
                f_cnn.write((char*)ky2, sizeof(KEYNEW)*kh1.nKeys);
            }
        }
    }
}
*/
