#ifndef HDRLIST_H
#define HDRLIST_H


//[*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*]
#include <QAbstractTableModel>
#include "KeyPad.h"
#include "BuildKeypad.h"
#include "keypad_help_function.h"


class HdrListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    BuildKeypad *pad;
    KEYHDR  *hdr;

    explicit HdrListModel(QObject *parent, BuildKeypad *kpd_wnd, KEYHDR *kpd_hdr) : QAbstractTableModel(parent)
    {
        pad = kpd_wnd;
        hdr = kpd_hdr;
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
                const char *pname[]={"FILE NAME","KEYPAD TYPE","VAL(32)","WALL TYPE","WALL PICTURE","WALL COLOR","FONT 1","FONT 2","FONT 3","FONT 4","SCREEN X","SCREEN Y","WIDTH","HEIGHT","BUTTONS","ROWS","COLUMNS","Left Margin","Right Margin","Top Margin","Down Margin","Space Hor","Space Ver","Screen Position"};
                return pname[section];
            }
        }

        return QVariant();
    }


    QVariant data(const QModelIndex &index, int role) const
    {
        int row = index.row();

        if (!index.isValid() || row<0 || index.column()<0 || index.column()>0) return QVariant();

        if(role == Qt::DisplayRole || role == Qt::EditRole)
        {
            switch(row)
            {
            case 0: return pad->m_FileName;
            case 1: return pDrwM[hdr->type];
            case 2: return QString::number(hdr->data)+"  hex=0x"+QString::number(hdr->data,16);

            case 3: return pWllM[hdr->bckgrType];
            case 4: return hdr->Wallpaper;
            case 5: return QString::number(hdr->color[0],16);

            case 6: return QString("%1,%2").arg(hdr->font[0].Name).arg(hdr->font[0].Size);
            case 7: return QString("%1,%2").arg(hdr->font[1].Name).arg(hdr->font[1].Size);
            case 8: return QString("%1,%2").arg(hdr->font[2].Name).arg(hdr->font[2].Size);
            case 9: return QString("%1,%2").arg(hdr->font[3].Name).arg(hdr->font[3].Size);

            case 10: return QString::number(hdr->x);
            case 11: return QString::number(hdr->y);
            case 12: return QString::number(hdr->cx);
            case 13: return QString::number(hdr->cy);
            case 14: return QString::number(hdr->nKeys);
            case 15: return QString::number(hdr->rows);
            case 16: return QString::number(hdr->cols);

            case 17: return QString::number(hdr->margine_lf);
            case 18: return QString::number(hdr->margine_rt);
            case 19: return QString::number(hdr->margine_up);
            case 20: return QString::number(hdr->margine_dn);
            case 21: return QString::number(hdr->space_h); //space_hor
            case 22: return QString::number(hdr->space_v); //space_ver
            case 23: return hdr->position?".......":""; //space_ver
            }
        }
        /*
        if(role == Qt::TextAlignmentRole )
        {
            switch(index.column())
            {
            case 4:
            case 3:
            case 2: return int(Qt::AlignRight | Qt::AlignVCenter);
            case 5:
            case 9:
            case 1: return int(Qt::AlignCenter);
            }
        }
        */
        if(role==Qt::ForegroundRole)
        {
            switch(index.row())
            {
            case 22:
            case 21:
            case 20:
            case 19:
            case 18:
            case 17:
            case 15:
            case 16: if(hdr->type==1) break; /// greed type keys
            case 14:
            case  1:
            case  0: return QBrush(0x888888); // dim color for showing are not edditable
            }

            return QVariant();
        }
        if(role==Qt::BackgroundRole)
        {
            switch(row)
            {
            case 5:  return QBrush(hdr->color[0]);
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
        if(index.isValid() && role==Qt::EditRole && index.row()>=0 && index.column()==0)
        {
            switch(index.row())
            {
            //case 0:  strncpy( hdr->, value.toString().toUtf8().constData(), sizeof(KEYHDR::Wallpaper)); break;
            //case 2:  hdr->data = value.toInt(); break;
            //case 4:  strncpy( hdr->Wallpaper, value.toString().toUtf8().constData(), sizeof(KEYHDR::Wallpaper)); break;
            case 10: hdr->x  = value.toInt(); break;
            case 11: hdr->y  = value.toInt(); break;
            case 12: hdr->cx  = value.toInt(); pad->ResizeKeyPad(); break;
            case 13: hdr->cy  = value.toInt(); pad->ResizeKeyPad(); break;
            //case 14:  hdr->nKeys = value.toInt(); break;
            case 15: hdr->rows = value.toInt(); pad->setGreedKeys(hdr->cols, hdr->rows, hdr->subtp); break;
            case 16: hdr->cols = value.toInt(); pad->setGreedKeys(hdr->cols, hdr->rows, hdr->subtp); break;
            case 17: hdr->margine_lf = value.toInt(); pad->ResizeKeyPad(); break;
            case 18: hdr->margine_rt = value.toInt(); pad->ResizeKeyPad(); break;
            case 19: hdr->margine_up = value.toInt(); pad->ResizeKeyPad(); break;
            case 20: hdr->margine_dn = value.toInt(); pad->ResizeKeyPad(); break;
            case 21: hdr->space_h = value.toInt(); pad->ResizeKeyPad(); break;//space_hor
            case 22: hdr->space_v = value.toInt(); pad->ResizeKeyPad(); break;//space_ver
            default: return false;
            }
            emit kpd_update(0);
            return true;
        }
        return false;
    }


    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid() || index.column()>0) return 0;

        int flag = Qt::ItemIsEditable;

        switch(index.row())
        {
        case 23:
        case 14:
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
        case 3:
        case 2:
        case 1:
        case 0: flag = 0; break;
        }

        return QAbstractTableModel::flags(index) | (Qt::ItemFlags)flag;
    }

signals:
    void kpd_update(int level);
};



//[*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*]
#include <QTableView>
#include <QLineEdit>


class HdrTableView : public QTableView
{
    Q_OBJECT

public:
    explicit HdrTableView(QWidget *parent, BuildKeypad *kpd_wnd, KEYHDR *kpd_hdr);
    //~KeyListDlg();

    HdrListModel *model;
    BuildKeypad *kpd;
    KEYHDR  *hdr;

    void ListIni();
    void onCellDblClicked(const QModelIndex &index);
    void WallOptions();
    bool openFont(int n);
    void openScreenPos();

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *   Eδώ γίνεται ο έλεγχος του QLineEdit πριν περάσει στο model::setData().
    *   Εαν έγεινε αλλαγή κειμένου τότε καλώ την base member που καλεί το model::setData()
    *
    */
    void commitData(QWidget *editor)
    {
        if(((QLineEdit*)editor)->isModified()) QTableView::commitData(editor);
        //qDebug() << "commitData" << ((QLineEdit*)editor)->isModified() << ((QLineEdit*)editor)->text();
    }
};


#endif // HDRLIST_H
