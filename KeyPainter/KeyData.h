#ifndef KEYDATA_H
#define KEYDATA_H


//[*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*]
#include <QAbstractTableModel>
#include "KeyPad.h"
#include "keypad_help_function.h"
#include <QMimeData>
#include <qDebug>


class KeyDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    KEYHDR  *hdr;
    KEY     *key;
    KEY     *keys;
    KEY     keyb;
    int     m_idx=-1;
    quint32 m_cflags=0;
    Qt::ItemFlags m_itemflagCheck = 0; // Qt::CheckStateRole | Qt::ItemIsUserCheckable
    Qt::ItemFlags m_itemflagEdit = Qt::ItemIsEditable;


    explicit KeyDataModel(QObject *parent, KEYHDR *kpd_hdr, KEY *kpd_keys) : QAbstractTableModel(parent)
    {
        hdr  = kpd_hdr;
        keys = key = kpd_keys;
        memset(&keyb, 0, sizeof(KEY));
    }


    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        return parent.isValid() ? 0 : 1;
    }


    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        return parent.isValid() ? 0 : 24;
    }


    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Vertical)
            {
                const char *pname[]={"LABEL","DATA","MORE","cmd","val32","Operation","To Function","State on/off","Design","Color Text","Color Fill","Color Out","Font","Text Align","Icon","Attribute 1","Attribute 2","Attribute 3","Attribute 4","Width","Height","x [◄-►]","y [▲-▼]","dimensions\ncolor[2]4byte"};
                return pname[section];
            }
        }

        return QVariant();
    }


    QVariant data(const QModelIndex &index, int role) const
    {
        int row = index.row();

        if(!index.isValid() || m_idx<0 || index.column()>0) return QVariant(); //A valid index has non-negative row and column numbers.

        if(role == Qt::DisplayRole || role == Qt::EditRole)
        {
            switch(row)
            {
            case 0: return key->text;
            case 1: return key->data;
            case 2: return key->mem;
            case 3: return QString::number(key->cmd)+" = 0x"+QString::number(key->cmd,16);
            case 4: return QString::number(key->val)+" = 0x"+QString::number(key->val,16);
            //case 5: return operationName();
            case 5: return (QString)pMsgK[key->oper&0x07]+pMsg2[key->oper>>4];//+(((key.pkeySel->oper&0x07)>=2)?key.pkeySel->text+(sizeof(KEY::text)-15):"");
            case 6: return key->text+(sizeof(KEY::text)-15);

            case 7: return pStBt[key->onoff];
            case 8: return pDgnK[key->btn];

            case  9: return QString::number(key->color[0]&0x00ffffff,16);
            case 10: return QString::number(key->color[1]&0x00ffffff,16);
            case 11: return QString::number(key->color[2]&0x00ffffff,16);

            case 12: return QString("%1=%2,%3").arg(1+(key->nfnt&0x3)).arg(hdr->font[key->nfnt&0x3].Name).arg(hdr->font[key->nfnt&0x3].Size);
            case 13: return (key->nfnt&0xf0)?"...":"no";
            case 14: return pBtIc[(key->nfnt>>2)&0x3];

            case 15: return QString::number(key->attr1);
            case 16: return QString::number((quint8)key->attr2);
            case 17: return QString::number((quint8)key->attr3);
            case 18: return QString::number((quint8)key->attr4);

            case 19: return QString::number(key->cx);
            case 20: return QString::number(key->cy);
            case 21: return QString::number(key->x);
            case 22: return QString::number(key->y);

            case 23: return (key->color[2]&0xff000000)?QString("0x%1").arg(key->color[2]>>24, 0, 16):"";
            }
        }

        if (role == Qt::CheckStateRole && m_itemflagCheck && row!=21 && row!=22)
        {
            return (m_cflags&(0x000001<<row))? Qt::Checked : Qt::Unchecked;
        }

        /*
        if(role == Qt::TextAlignmentRole )
        {
            switch(index.column())
            {
            case 1: return int(Qt::AlignCenter);
            }
        }
        */
        /*
        if(role==Qt::ForegroundRole)
        {
            quint32 clr[]={0xffff80,0x800000,0x005555,0x0,0x0,0x505080,0x80ff80,0x80ff80,0x80ff80,0xffffff,0x00};
            return QBrush(clr[index.column()]);
        }
        */
        if(role==Qt::BackgroundRole)
        {
            switch(row)
            {
            case 9:  return QBrush(key->color[0]);
            case 10: return QBrush(key->color[1]);
            case 11: return QBrush(key->color[2]);
            }
        }
        /*
        if(role==Qt::FontRole)
        {
            if (row == 0 && col == 0) //change font only for cell(0,0)
            {
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
        }
        */

        return QVariant();
    }


    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if(!index.isValid() || m_idx<0) return false;

        if (role == Qt::EditRole)
        {
            ////{"Label","Data","More","cmd","val 32","Operation","to function","State on/off","Design","Color Text","Color Fill","Color Out","Font","Text Align","Icon","Attribute 1","Attribute 2","Attribute 3","Attribute 4","width (F8,F9)","height (F10,F11)","x [◄-►]","y [▲-▼]","..."};

            switch(index.row())
            {
            //case 0:  qstrcpy( key->text, value.toString().toUtf8().constData()); break; // see sizeLebel() in keypad_function.cpp
            case 1:  qstrncpy( key->data, value.toString().toUtf8().constData(), sizeof(KEY::data)); break;
            case 2:  qstrncpy( key->mem, value.toString().toUtf8().constData(), sizeof(KEY::mem)); break;
            case 6:  qstrcpy( key->text+(sizeof(KEY::text)-15), value.toString().toUtf8().constData()); break;
            case 17: key->attr3 = value.toInt(); break;
            case 18: key->attr4 = value.toInt(); break;
            case 19: key->cx = value.toInt(); break;
            case 20: key->cy = value.toInt(); break;
            case 21: key->x = value.toInt(); break;
            case 22: key->y = value.toInt(); break;
            default: return false;
            }

            emit kpd_update(1);
            return true;
        }

        if (role == Qt::CheckStateRole)
        {
            //// flip state: flags ^= 1 << idx;
            //// set off:    flags &= ~(1 << idx);
            //// set on :    flags |= 1 << idx;

            if ((Qt::CheckState)value.toInt() == Qt::Checked)
                m_cflags |= (1<<index.row());
            else
                m_cflags &= ~(1<<index.row());

            return true;
        }

        return false;
    }


    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid() || m_idx<0 ) return 0;

        int row = index.row();
        Qt::ItemFlags flag = Qt::ItemIsSelectable |  Qt::ItemIsEnabled;

        if(row!=21 && row!=22) // avoid x/y copy
        {
            flag |= m_itemflagCheck;
        }

        switch(row)
        {
        case 22:
        case 21:
        case 20:
        case 19:
        case 18:
        case 17:
        case 6:
        case 2:
        case 1:
            flag |= m_itemflagEdit; break;
        }

        return flag;
        //return QAbstractTableModel::flags(index) | (Qt::ItemFlags)flag;
    }


    void newKey(int idx)
    {
        m_idx = idx;

        if(idx>=0)
            key = keys+m_idx;   // work key
        else
            key = &keyb;        // blank key

        emit dataChanged(QAbstractItemModel::createIndex(0,0), QAbstractItemModel::createIndex(23,0));
    }


signals:
    void kpd_update(int level);
};


//[*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*]
#include <QTableView>
#include <QLineEdit>


class KeyDataView : public QTableView
{
    Q_OBJECT

public:
    explicit KeyDataView(QWidget *parent, KEYHDR *kpd_hdr, KEY *kpd_keys);

    KeyDataModel *model;
    KEYHDR  *hdr;
    KEY     *keys;

    void ListIni();
    void onCellDblClicked(const QModelIndex &index);
    void selectIdx(qint32 idx);
};


#endif // KEYDATA_H
