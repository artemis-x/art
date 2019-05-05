#include "keypad_help_function.h"
#include "KeyPad.h"
#include "dlgnuminput.h"
#include <QApplication>
#include <QColorDialog>


////////////////////////////////////////////////////////
const char *pDrwM[4]={"Free Keypad","Grid Keypad","NEW - Grid","NEW - Free"};
const char *pWllM[7]={"Fill Color","Linear Gradient","Radial Gradient","Picture (keep size)","Picture (scale size)","Preset Gradient","2 x Fill Color"};
const char *pMsgK[4]={"no message", "send event message","call function()","call function(cmd,val,data)"};
const char *pMsg2[5]={", stay, ", ", keypad change, ",", keypad return, ",", delete, ",", hide, "};
const char *pDgnK[28]={"Empty Button","Fill Rect Button","3D Rect Button","Round Rect Button","Eliptic Button","Degrade Rectangles","Degrade Down Button","Gradient Button","Gradient Radial Button", "---","Line Edit","Plain Edit","Check Box","Spin Box","Combo Box","Date Time","Calentar","Order List","KeyPad","Table(w)List","Tree(w)List","Table(v)List","Tree(v)List","Button","Label","Widget","QML Widget","..."};
const char *pBtIc[4]={"no","Icon - no scale","Icon - scale","Icon - scale keeping aspect"};
const char *pStBt[8]={"push button","switch on/off (kpd.val=default state)","group 1 radio (kpd.val=default state)","group 2 radio (kpd.color[2]=off state color)","group 3 radio","group 4 radio","group 5 radio","group 6 radio"};
//const char *pGrpK[24]={"push button","switch on/off","group 1 radio","group 2 radio","group 3 radio","group 4 radio","group 5 radio","group 6 radio","....","....","....","....","....","....","....","....","push button (Member Function Text Structure)","switch on/off","group 1 radio","group 2 radio","group 3 radio","group 4 radio","group 5 radio","group 6 radio"};
//const char *pGrpK[14]={"push button","switch on/off","group 1 radio","group 2 radio","group 3 radio","group 4 radio","group 5 radio","group 6 radio","group A to all keypads","group B to all keypads","group C to all keypads","group D to all keypads","group E to all keypads","group F to all keypads"};
////////////////////////////////////////////////////////


int sizeLebel(KEY &ky)
{
    /////////////// grow the text size, according the button operation
    quint8 flags=0;
    if(ky.oper&0x02)  flags|=0x1; // has function name 15 in text
    if(ky.nfnt&0x0c)  flags|=0x2; // has icon name     25 in mem
    if(ky.oper==0x10) flags|=0x4; // has keypad name   15 in data

    int sz = sizeof(KEY::text);
    if(flags==0x00)      sz = sizeof(KEY::text)+sizeof(KEY::mem)+sizeof(KEY::data)+sizeof(KEY::cmd)+sizeof(KEY::val);
    else if(flags&0x01 ) sz = sizeof(KEY::text)-15;
    else if(flags==0x04) sz = sizeof(KEY::text)-+sizeof(KEY::mem);

    return sz;
}


/*
void copy_field(KEY *kfr, KEY *kto, int column)
{
    switch(column)
    {
    case 0:  qstrcpy(kto->text, kfr->text); break;///exclude function hadler name (last 15 char)
    case 1:  qstrncpy(kto->data, kfr->data, sizeof(KEY::data)); break;
    case 2:  qstrncpy(kto->mem,  kfr->mem,  sizeof(KEY::mem)); break;
    case 3:  kto->cmd = kfr->cmd; break;
    case 4:  kto->val = kfr->val; break;
    case 5:  kto->oper = kfr->oper; break;///kfr->oper&0x07  kfr->oper&0xf0  ((pkey->oper&0x07)>=2)?pkey->text+(sizeof(KEY::text)-15):"";
    case 6:  qstrncpy(kto->text+(sizeof(KEY::text)-15), kfr->text+(sizeof(KEY::text)-15), 15); break;

    case 7:  kto->onoff = kfr->onoff; break;
    case 8:  kto->btn = kfr->btn; break;

    case 9:  kto->color[0] = kfr->color[0]; break;
    case 10: kto->color[1] = kfr->color[1]; break;
    case 11: kto->color[2] = (kto->color[2]&0xff000000) | (kfr->color[2]&0x00ffffff); break;

    case 12:  kto->nfnt&=0xfc; kto->nfnt|=kfr->nfnt&0x03; break;///Font
    case 13:  kto->nfnt&=0x0f; kto->nfnt|=kfr->nfnt&0xf0; break;///Alighn
    case 14:  kto->nfnt&=0xf3; kto->nfnt|=kfr->nfnt&0x0c; break;///Icon

    case 15:  kto->attr1 = kfr->attr1; break;
    case 16:  kto->attr2 = kfr->attr2; break;
    case 17:  kto->attr3 = kfr->attr3; break;
    case 18:  kto->attr4 = kfr->attr4; break;

    case 19:  kto->cx = kfr->cx; break;
    case 20:  kto->cy = kfr->cy; break;
    case 21:  kto->x = kfr->x; break;
    case 22:  kto->y = kfr->y; break;

    case 23:  kto->color[2]&=0x00ffffff; kto->color[2]|=kfr->color[2]&0xff000000; break;
    }
}
*/


void copy_field(KEY *kfr, KEY *kto, int column)
{
    copy_fields(kfr, kto, 0x000001<<column);
}


void copy_fields(KEY *kfr, KEY *kto, quint32 flags)
{
    if(flags&0x000001)  qstrcpy(kto->text, kfr->text); ///exclude function hadler name (last 15 char)
    if(flags&0x000002)  qstrncpy(kto->data, kfr->data, sizeof(KEY::data));
    if(flags&0x000004)  qstrncpy(kto->mem,  kfr->mem,  sizeof(KEY::mem));
    if(flags&0x000008)  kto->cmd = kfr->cmd;
    if(flags&0x000010)  kto->val = kfr->val;
    if(flags&0x000020)  kto->oper = kfr->oper; ///kfr->oper&0x07  kfr->oper&0xf0  ((pkey->oper&0x07)>=2)?pkey->text+(sizeof(KEY::text)-15):"";
    if(flags&0x000040)  qstrncpy(kto->text+(sizeof(KEY::text)-15), kfr->text+(sizeof(KEY::text)-15), 15);

    if(flags&0x000080)  kto->onoff = kfr->onoff;
    if(flags&0x000100)  kto->btn = kfr->btn;

    if(flags&0x000200)  kto->color[0] = kfr->color[0];
    if(flags&0x000400)  kto->color[1] = kfr->color[1];
    if(flags&0x000800)  kto->color[2] = (kto->color[2]&0xff000000) | (kfr->color[2]&0x00ffffff);

    if(flags&0x001000)  {kto->nfnt&=0xfc; kto->nfnt|=kfr->nfnt&0x03;} ///Font
    if(flags&0x002000)  {kto->nfnt&=0x0f; kto->nfnt|=kfr->nfnt&0xf0;} ///Alighn
    if(flags&0x004000)  {kto->nfnt&=0xf3; kto->nfnt|=kfr->nfnt&0x0c;} ///Icon

    if(flags&0x008000)  kto->attr1 = kfr->attr1;
    if(flags&0x010000)  kto->attr2 = kfr->attr2;
    if(flags&0x020000)  kto->attr3 = kfr->attr3;
    if(flags&0x040000)  kto->attr4 = kfr->attr4;

    if(flags&0x080000)  kto->cx = kfr->cx;
    if(flags&0x100000)  kto->cy = kfr->cy;
    if(flags&0x200000)  kto->x = kfr->x;
    if(flags&0x400000)  kto->y = kfr->y;

    if(flags&0x800000)  {kto->color[2]&=0x00ffffff; kto->color[2]|=kfr->color[2]&0xff000000;}
}


void insert_key(int idxfr, int idxto, KEYHDR *hdr, KEY *keys) /// moves 'idx1' above 'idx2'
{
    if(idxfr!=idxto && idxto>=0)
    {
        KEY key1;
        memcpy(&key1, &keys[idxfr], sizeof(KEY));

        memmove(&keys[idxfr], &keys[idxfr+1], ((hdr->nKeys-1)-idxfr)*sizeof(KEY) ); /// close hole of idxfr
        memmove(&keys[idxto+1], &keys[idxto], ((hdr->nKeys-1)-idxto)*sizeof(KEY) ); /// open hole before idxto for idx1

        memcpy(&keys[idxto], &key1, sizeof(KEY)); /// copy in the opened hole of idxto
    }
}


bool setColor(quint32 &clr)
{
    QColor color = QColorDialog::getColor(clr, QApplication::activeWindow(), "Set Color", QColorDialog::DontUseNativeDialog);

    if (color.isValid())
    {
        clr=color.rgb();
        return true;
    }
    return true;
}


bool setInt32(int &val)
{
    DlgNumInput dlg(val, QApplication::activeWindow());

    if(dlg.exec())
    {
        val=dlg.m_val;
        return true;
    }
    return false;
}


/////////////
#include "dlgmessageoper.h"
#include "dlgtxtalign.h"
#include "dlgfreealign.h"
#include "dlgbuttonx2.h"
#include "dlgdesignrectdgr.h"
#include <QMenu>

/*
#include "dlgtextinpout.h"

void MainWindow::setBtLabel()
{
    DlgTextInpout dlg(key.pkeySel->text, sizeLebel(key.pkeySel), this);

    if(dlg.exec())
    {
        m_update_file = true;
        key.Redraw2();
    }

//    bool ok;

//    // My CONTROL CHAR for NewLine: '^'= \n = 0xA
//    QString text = QString::fromLocal8Bit(key.pkey[m_idx].text);
//    text.replace(0xA, '^'); // do it visible

//    text = QInputDialog::getText(this, "Button", "Label:   [ New Line Char (0xA) = '^' ]",
//                    QLineEdit::Normal, text, &ok);

//    if (ok)
//    {
//        //Convert the '^' character to newline code = 0xA
//        qstrncpy(key.pkey[m_idx].text, text.replace('^', 0xA).toLocal8Bit().constData(), sizeof(KEY::text));
//        //strncpy: If the end of the source string (null) is found before num characters have been copied,
//        //destination is padded with zeros until a total of num characters have been written to it.
//    }
}
*/


bool setOperation(KEY &ky, QWidget *parent)
{
    dlgmessageoper dlg(ky.oper, ky.text+(sizeof(KEY::text)-15), ky.data, parent);

    if(dlg.exec())
    {
        ky.oper=dlg.m_oper;
        return true;
    }
    return false;
}


bool setDesignType(KEY &ky, QWidget *parent)
{
    QMenu  Menu( parent ); // "Design Type:" key.pkeySel->btn
    for(int i=0; i<28; ++i) Menu.addAction(pDgnK[i])->setData(i);
    QAction *act= Menu.exec(QCursor::pos());

    if (act)
    {
        ky.btn=act->data().toInt();
        return true;
    }
    return false;
}


bool setOnOffGroup(KEY &ky, QWidget *parent)
{
    QMenu  Menu( parent ); // "Button State:" key.pkeySel->onoff
    for(int i=0; i<8; ++i) Menu.addAction(pStBt[i])->setData(i);
    QAction *act= Menu.exec(QCursor::pos());

    /*
    0000 = push button
    0001 = switch on/off
    0111 = local groups of radio buttons
    1111 = global groups of radio buttons for all keypads
    */

    if (act)
    {
        ky.onoff=act->data().toInt();
        return true;
    }
    return false;
}


bool setFontK(KEY &ky, KEYHDR &hdr, QWidget *parent)
{
    QMenu  Menu( parent );
    Menu.addAction(QString("1=%1,%2").arg(hdr.font[0].Name).arg(hdr.font[0].Size))->setData(0);
    Menu.addAction(QString("2=%1,%2").arg(hdr.font[1].Name).arg(hdr.font[1].Size))->setData(1);
    Menu.addAction(QString("3=%1,%2").arg(hdr.font[2].Name).arg(hdr.font[2].Size))->setData(2);
    Menu.addAction(QString("4=%1,%2").arg(hdr.font[3].Name).arg(hdr.font[3].Size))->setData(3);
    QAction *act= Menu.exec(QCursor::pos());

    if (act)
    {
        ky.nfnt&=0xfc; // reset the last 2 bits
        ky.nfnt|=act->data().toInt();
        return true;
    }
    return false;
}


bool setTxtAlign(KEY &ky, QWidget *parent)
{
    DlgTxtAlign dlg(ky.nfnt&0xf0, parent);

    if(dlg.exec())
    {
        ky.nfnt&=0x0f; // reset the upper 4 bits
        ky.nfnt|=dlg.m_flags;
        return true;
    }
    return false;
}


bool setIcon(KEY &ky, QWidget *parent)
{
    QMenu  Menu( parent ); // "Icon Type:"
    Menu.addAction(pBtIc[0])->setData(0);
    Menu.addAction(pBtIc[1])->setData(1);
    Menu.addAction(pBtIc[2])->setData(2);
    Menu.addAction(pBtIc[3])->setData(3);
    QAction *act= Menu.exec(QCursor::pos());

    if (act)
    {
        ky.nfnt&=0xf3; // reset 2 bits
        ky.nfnt|=act->data().toInt()<<2;
        return true;
    }
    return false;
}


bool setAttr1(KEY &ky, QWidget *parent)
{
    if(ky.btn==0x5)
    {
        DlgDesignButtonAr dlg(&ky, parent);
        if(dlg.exec())
        {
            return true;
        }
    }
    else
    {
        qint32 val = ky.attr1;

        if(setInt32(val)) // quint8 = 0-255
        {
            ky.attr1 = val;
            return true;
        }
    }
    return false;
}


bool setAttr2(KEY &ky, QWidget *parent)
{
    if(ky.btn==0x5)
    {
        DlgDesignButtonAr dlg(&ky, parent);
        if(dlg.exec())
        {
            return true;
        }
    }
    else
    {
        qint32 val = ky.attr2;
        if(setInt32(val)) // quint8 = -128+128
        {
            ky.attr2 = val;
            return true;
        }
    }
    return false;
}


bool setBtPos(KEY &ky, KEYHDR &hdr, QWidget *parent)
{
    if(hdr.type==1) //// grid keys
    {
        dlgbuttonx2 dlg(ky.color[2], parent);

        if(dlg.exec())
        {
            ky.color[2]&=0x00ffffff; // reset the upper 8 bits
            ky.color[2]|=dlg.m_flags;
            return true;
        }
    }

    if(hdr.type==0) //// free keys
    {
        dlgfreealign dlg(ky.color[2], parent);

        if(dlg.exec())
        {
            ky.color[2]&=0x00ffffff; // reset the upper 8 bits
            ky.color[2]|=dlg.m_flags;
            return true;
        }
    }
    return false;
}
