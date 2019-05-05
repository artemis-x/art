#ifndef KEYLIST_H
#define KEYLIST_H


//[*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*]
#include <QAbstractTableModel>
#include "KeyPad.h"
#include "keypad_help_function.h"
#include <QMimeData>
#include <qDebug>


class KeyListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    KEYHDR  *hdr;
    KEY     *key;
    int     m_drag_row=-1;
    int     m_drag_col=-1;
    bool    m_drag_type=false;


    explicit KeyListModel(QObject *parent, KEYHDR *kpd_hdr, KEY *keys) : QAbstractTableModel(parent)
    {
        hdr = kpd_hdr;
        key = keys;
    }


    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        return parent.isValid() ? 0 : 24;
    }


    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        return parent.isValid() ? 0 : hdr->nKeys;
    }


    /*
    void my_append_data(const string& first, int second)
    {
        int row = rowCount(QModelIndex());
        insertRow(row); // insert a empty row

        setData(createIndex(row, 0), QVariant::fromValue<string>(first), Qt::EditRole);
        setData(createIndex(row, 1), QVariant::fromValue<int>(second), Qt::EditRole);
    }
    // usage
    model.my_append_data("11111", 111);
    model.my_append_data("22222", 222);

    or...
    void my_append_data(const QVariantList &data)
    {
        insertRow(rowCount(QModelIndex()));

        foreach(const QVariant &item, data)
        {
            setData(createIndex(row, 0), item, Qt::EditRole);
        }
    }

    Usage:
    model.my_append_data(QVariantList() << "11111" << 111);

    and inside the beginInsertRows...
    void append_data(const string& first, int second)
    {
        int newRow = data_.count();
        beginInsertRows(QModelIndex(), newRow, newRow);
        my_append_data(QPair<string, int>(first, second));
        endInsertRows();
    }
    */

    /* * * * * * * * * * * * * * * * * * * * * * * *
     *   Mission imposible !!
     *   Ανανέωση Λίστας μετά από εισαγογή νέων Buttons
     *
    void reload()
    {
        //beginInsertRows(QModelIndex(), 0, rowCount());
        //endInsertRows();

        beginResetModel();
        //hdr->nKeys = 0;
        endResetModel();

        //emit layoutChanged();
        //emit dataChanged(QAbstractItemModel::createIndex(0,0), QAbstractItemModel::createIndex(hdr->nKeys-1, 23));
    }
    */


    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal)
            {
                const char *pname[]={"Label","Data","More","cmd","val32","Operation","To Function","State on/off","Design","Color Text","Color Fill","Color Out","Font","Text Align","Icon","Attribute 1","Attribute 2","Attribute 3","Attribute 4","Width","Height","x [◄-►]","y [▲-▼]","dimensions (color[2]upbyte)"};
                return pname[section];
            }
            if (orientation == Qt::Vertical)
            {
                return QString::number(section+1);
            }
        }

        return QVariant();
    }


    QVariant data(const QModelIndex &index, int role) const
    {
        int row = index.row();

        if(!index.isValid() || row>=hdr->nKeys) return QVariant(); //A valid index has non-negative row and column numbers.

        if(role == Qt::DisplayRole || role == Qt::EditRole)
        {
            switch(index.column())
            {
            case 0: return key[row].text;
            case 1: return key[row].data;
            case 2: return key[row].mem;
            case 3: return QString::number(key[row].cmd)+" = 0x"+QString::number(key[row].cmd,16);
            case 4: return QString::number(key[row].val)+" = 0x"+QString::number(key[row].val,16);
            //case 5: return operationName();
            case 5: return (QString)pMsgK[key[row].oper&0x07]+pMsg2[key[row].oper>>4];//+(((key.pkeySel->oper&0x07)>=2)?key.pkeySel->text+(sizeof(KEY::text)-15):"");
            case 6: return key[row].text+(sizeof(KEY::text)-15);

            case 7: return pStBt[key[row].onoff];
            case 8: return pDgnK[key[row].btn];

            case  9: return QString::number(key[row].color[0]&0x00ffffff,16);
            case 10: return QString::number(key[row].color[1]&0x00ffffff,16);
            case 11: return QString::number(key[row].color[2]&0x00ffffff,16);

            case 12: return QString("%1=%2,%3").arg(1+(key[row].nfnt&0x3)).arg(hdr->font[key[row].nfnt&0x3].Name).arg(hdr->font[key[row].nfnt&0x3].Size);
            case 13: return (key[row].nfnt&0xf0)?"...":"no";
            case 14: return pBtIc[(key[row].nfnt>>2)&0x3];

            case 15: return QString::number(key[row].attr1);
            case 16: return QString::number((quint8)key[row].attr2);
            case 17: return QString::number((quint8)key[row].attr3);
            case 18: return QString::number((quint8)key[row].attr4);

            case 19: return QString::number(key[row].cx);
            case 20: return QString::number(key[row].cy);
            case 21: return QString::number(key[row].x);
            case 22: return QString::number(key[row].y);

            case 23: return (key[row].color[2]&0xff000000)?QString("0x%1").arg(key[row].color[2]>>24, 0, 16):"";
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
        /*
        if(role==Qt::ForegroundRole)
        {
            quint32 clr[]={0xffff80,0x800000,0x005555,0x0,0x0,0x505080,0x80ff80,0x80ff80,0x80ff80,0xffffff,0x00};
            return QBrush(clr[index.column()]);
        }
        */
        if(role==Qt::BackgroundRole)
        {
            switch(index.column())
            {
            case 9:  return QBrush(key[row].color[0]);
            case 10: return QBrush(key[row].color[1]);
            case 11: return QBrush(key[row].color[2]);
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
        if(index.isValid() && role==Qt::EditRole)
        {
            int row = index.row();

            ////{"Label","Data","More","cmd","val 32","Operation","to function","State on/off","Design","Color Text","Color Fill","Color Out","Font","Text Align","Icon","Attribute 1","Attribute 2","Attribute 3","Attribute 4","width (F8,F9)","height (F10,F11)","x [◄-►]","y [▲-▼]","..."};

            switch(index.column())
            {
            //case 0:  qstrcpy( key[row].text, value.toString().toUtf8().constData()); break; // see sizeLebel() in keypad_function.cpp
            case 1:  qstrncpy( key[row].data, value.toString().toUtf8().constData(), sizeof(KEY::data)); break;
            case 2:  qstrncpy( key[row].mem, value.toString().toUtf8().constData(), sizeof(KEY::mem)); break;
            case 6:  qstrcpy( key[row].text+(sizeof(KEY::text)-15), value.toString().toUtf8().constData()); break;
            case 17: key[row].attr3 = value.toInt(); break;
            case 18: key[row].attr4 = value.toInt(); break;
            case 19: key[row].cx = value.toInt(); break;
            case 20: key[row].cy = value.toInt(); break;
            case 21: key[row].x = value.toInt(); break;
            case 22: key[row].y = value.toInt(); break;
            default: return false;
            }

            emit kpd_update(1);
            return true;
        }
        return false;
    }


    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid()) return 0;

        int flag = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

        switch(index.column())
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
            flag |= Qt::ItemIsEditable; break;
        }

        return QAbstractTableModel::flags(index) | (Qt::ItemFlags)flag;
    }
/*
    QStringList mimeTypes() const
     {
         QStringList types;
         types << "application/vnd.text.list";
         return types;
     }

    QMimeData *mimeData(const QModelIndexList &indexes) const
    {
        QMimeData *mimeData = new QMimeData();
        QByteArray encodedData;

        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        foreach (QModelIndex index, indexes) {
            if (index.isValid()) {
                QString text = data(index, Qt::DisplayRole).toString();
                stream << text;
            }
        }

        mimeData->setData("application/vnd.text.list", encodedData);
        return mimeData;
    }
*/
/*
    bool insertRows(int position, int rows, const QModelIndex &index)
     {
         Q_UNUSED(index);
         beginInsertRows(QModelIndex(), position, position+rows-1);

         qDebug() << "insertRows" << position << rows << index.column() << index.row() << index.data(Qt::DisplayRole).toString() << index.row() << index.column() << index.parent().row();

         //for (int row=0; row < rows; row++) {
         //    QPair<QString, QString> pair(" ", " ");
         //    listOfPairs.insert(position, pair);
         //}

         endInsertRows();
         return true;
     }

     bool removeRows(int position, int rows, const QModelIndex &index)
     {
         Q_UNUSED(index);
         beginRemoveRows(QModelIndex(), position, position+rows-1);

         qDebug() << "removeRows" << position << rows << index.column() << index.row() << index.data(Qt::DisplayRole).toString() << index.row() << index.column() << index.parent().row();

         //for (int row=0; row < rows; ++row) {
         //    listOfPairs.removeAt(position);
         //}

         endRemoveRows();
         return true;
     }
*/

    Qt::DropActions supportedDropActions() const
    {
          return Qt::MoveAction; //Qt::CopyAction |
    }


    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
     {
         if (action == Qt::IgnoreAction) return true;

         if(m_drag_type)
         {
             if(m_drag_row>=0)
             {
                 insert_key(m_drag_row, parent.row(), hdr, key);
             }
         }
         else
         {
             if(parent.column()==m_drag_col)
             {
                 copy_field(key+m_drag_row, key+parent.row(), m_drag_col);
             }
         }

         emit kpd_update(0);
         m_drag_row = m_drag_col = -1;
         //return QAbstractItemModel::dropMimeData ( data, action, row, column, parent);
         return true;
    }

signals:
    void kpd_update(int level);
};



//[*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*]
#include <QTableView>
#include <QLineEdit>
#include <QMouseEvent>


class TableView : public QTableView
{
    Q_OBJECT

public:
    explicit TableView(QWidget *parent = 0) : QTableView(parent){}

//signals:
    //void dragStart(int drag_row, int drag_col);
    //void dragStop(int drop_row, int drop_col);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *   Eδώ παίρνω τις αρχικές τιμες του drag item [row] & [column].
    *   Γιατί στο model::dropMimeData() υπάρχει μόνον του drop item row&column.
    *   Και από το KeyListDlg::onCellClicked που μπορούμε να πάρουμε αυτά
    *   μόνο με επιλογή του item και μετά σύρσιμο αλλοιώς δεν καλείται με απευθείας σύρσιμο.
    *
    *   How to find the row from which drag has started QTableView
    *   https://stackoverflow.com/questions/27773385/how-to-find-the-row-from-which-drag-has-started-qtableview
    *   και εδώ διαπιστώνουν ότι στο startDrag() το rowAt() στις άκρες του item δίνει κάποιες φωρές το πάνω row item
    *   Aυτό λύθικε μέσα από το QMouseEvent του mousePressEvent() που δίνει ασφαλέστερο row number
    *
    */
    void mousePressEvent( QMouseEvent *ev )
    {
        if ( ev->button() == Qt::LeftButton )
        {
            ((KeyListModel*)model())->m_drag_row = rowAt(ev->y());   // στις ακρες ανακριβές !
            ((KeyListModel*)model())->m_drag_col = columnAt(ev->x());
        }

        QTableView::mousePressEvent( ev );
    }

protected:

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *   Eδώ παίρνω τις αρχικές τιμες του drag item [row] & [column].
    *   Γιατί στο model::dropMimeData() υπάρχει μόνον του drop item row&column.
    *   Και στο KeyListDlg::onCellClicked που μπορούμε να πάρουμε αυτά
    *   μόνο με επιλογή του item και μετά σύρσιμο αλλοιώς δεν καλείται με απευθείας σύρσιμο.
    *
    *   Υπάρχει όμως θέμα με το rowAt() στις άκρες των item που δίνει κάποιες φωρές το πάνω row item
    *   Aυτό λύθικε μέσα από το QMouseEvent του mousePressEvent() που δίνει ασφαλέστερο row number
    *
    void startDrag( Qt::DropActions supportedActions )
    {
        QPoint posxy = viewport()->mapFromGlobal( QCursor::pos() );
        ((KeyListModel*)model())->m_drag_row = rowAt(posxy.y());   // στις ακρες ανακριβές !
        ((KeyListModel*)model())->m_drag_col = columnAt(posxy.x());
        //emit dragStart(m_drag_row, m_drag_col);

        QTableView::startDrag(supportedActions);

        //CODE HERE WILL BE EXECUTED ONLY WHEN THE MOUSE BUTTON HAS BEEN RELEASED

        //posxy = viewport()->mapFromGlobal( QCursor::pos() );
        //m_drop_row = rowAt(posxy.y());
        //m_drop_col = columnAt(posxy.x());
        //emit dragStop(m_drop_row, m_drop_col);
    }
    */


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *   Τελικός έλεγχος του QLineEdit πριν περάσει στο model::setData().
    *   Ελέγχω εαν υπάρχει αλλαγή κειμένου για να εκτελέσω την base member που καλεί το model::setData()
    */
    void commitData(QWidget *editor)
    {
        if(((QLineEdit*)editor)->isModified()) QTableView::commitData(editor);
    }


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    * virtual member you might want to open the next editor by TAB etc
    *
    *   Eδώ μπορεί να γίνει ο έλεγχος εαν κατά το edit έχει γίνει αλλαγή του κειμένου
    *      ((QLineEdit*)editor)->isModified()
    *   καθώς ακόμα και στην μη αλλαγή με enter ή mouse γίνεται list/keypad update στο model
    *      hint = 0    (hitting mouse out the edditor)
    *      hint = 3    (hitting keyboard enter)
    *   αλλά δυστυχώς εκτελούνται με την παρακάτω σειρά:
    *      1st:    model::setData()
    *      2nd:    tableview::closeEditor()
    *   και δεν είναι δυνατόν να ειδοποιηθεί το model.
    *
    *   Λύση: το commitData() είναι η λύση στο πρόβλημα !
    *
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
    {
        QTableView::closeEditor(editor, hint);
        ((KeyListModel*)model())->m_isModified = ((QLineEdit*)editor)->isModified();
        //qDebug() << "onCloseEditor" << ((QLineEdit*)editor)->isModified() << hint;
    }
    */
};



//[*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*] [*]
#include <QDialog>
#include <QCursor>


class KeyListDlg : public QWidget
{
    Q_OBJECT

public:
    explicit KeyListDlg(QWidget *parent, int x, int y, int cx, int cy, KEYHDR *kpd_hdr, KEY *keys, int SelectedItem=-1);
    //~KeyListDlg();

    TableView    *list;
    KeyListModel *model;

    KEYHDR  *hdr;
    KEY     *key;

    void ListIni();
    void onCellClicked(const QModelIndex &index);
    void onCellDblClicked(const QModelIndex &index);
    void onHeaderReorder(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onCheckRowMove();
    void onCheckFieldCopy();
    void selectIdx(qint32 idx);

signals:
    void focusKey(int idx);
};


#endif // KEYLIST_H
