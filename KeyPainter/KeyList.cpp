#include "KeyList.h"
#include <QPushButton>
#include <QHeaderView>
#include <QResizeEvent>
#include <QBoxLayout>
#include <QToolBar>
#include <QSizeGrip>
#include "dlgtextinpout.h"



KeyListDlg::KeyListDlg(QWidget *parent, int x, int y, int cx, int cy, KEYHDR *kpd_hdr, KEY *keys, int SelectedItem) :
    QWidget(parent)//Qt::FramelessWindowHint
{
    setAttribute(Qt::WA_DeleteOnClose); // on close it deletes the pointer too

    QVBoxLayout *Layout = new QVBoxLayout(this);
    Layout->setContentsMargins(0, 0, 0, 0);
    Layout->setSpacing(0);

    QToolBar *toolbar = new QToolBar;//("tools", this);
    Layout->addWidget(toolbar);
    list = new TableView;//(this);
    Layout->addWidget(list);
    //Layout->addWidget(new QSizeGrip(this), 0, Qt::AlignBottom | Qt::AlignRight);

    setLayout(Layout);

    //toolbar->addAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "Exit", [=] {deleteLater();});
    //toolbar->addAction(style()->standardIcon(QStyle::SP_TitleBarNormalButton), "min", [=] {showNormal();});
    //toolbar->addAction(style()->standardIcon(QStyle::SP_TitleBarMaxButton), "max", [=] {showMaximized();});
    toolbar->addAction("[reorder lines: DRAG THE ROW]", this, &KeyListDlg::onCheckRowMove);
    toolbar->addAction("[copy field: DRAG THE CELL]",  this, &KeyListDlg::onCheckFieldCopy);

    setStyleSheet("*{font-size:12pt;}"
                  "QHeaderView::section {background-color:#a0e0e0}");

    /*
    ///// set all buttons inheriting the style from parent
    setStyleSheet("QDialog {background-color:#003333} "
                  "QPushButton {font-size:34px; background-color:#002222; color:#00FFFF; border:3px solid #008080;} "
                  "QTableView {font-size:40px; border:none; selection-background-color:#a0ffff; selection-color:#000000; background-color:#003333; color:#000000} "
                  "QHeaderView::section {font-size:28px; border:none; background-color:#003333; color:#00ffff} "
                  "QHeaderView {background-color:#003333} "
                  "QTableCornerButton::section {background:#003333}"
                  );
    */

    /////////////////////////////////////////////////////  LIST
    hdr = kpd_hdr;
    key = keys;

    ListIni();
    list->selectRow(SelectedItem);

    showMaximized();
}


void KeyListDlg::ListIni()
{
    model = new KeyListModel(this, hdr, key);
    //list = new QTableView(this);
    list->setModel(model);
    //list->setGeometry(0, 40, m_cx, m_cy-40);
    //list->show();

    //list->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //list->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //list->verticalHeader()->setDefaultSectionSize(40);
    //list->verticalHeader()->setMinimumWidth(25);
    //list->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //list->horizontalHeader()->setMinimumSectionSize(0);
    //list->horizontalHeader()->setStretchLastSection(true);
    //list->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    list->horizontalHeader()->setSectionsMovable(true);
    //list->verticalHeader()->hide();

    list->setSelectionMode(QAbstractItemView::SingleSelection );
    //list->setSelectionBehavior(QAbstractItemView::SelectRows);
    //list->setEditTriggers(QAbstractItemView::NoEditTriggers);//stop the auto edit.
    //list->setEditTriggers(QAbstractItemView::DoubleClicked);
    //list->setSortingEnabled(true);

    list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ////// moving the row using the inernal list & model
    list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    list->setDragEnabled(true);
    list->setAcceptDrops(true);
    list->setDropIndicatorShown(true);

    //list->setDragDropMode(QAbstractItemView::InternalMove);
    //list->setDragDropMode(QAbstractItemView::DragDrop);
    //list->setDefaultDropAction(Qt::MoveAction);
    //list->setDragDropOverwriteMode(false);

    ////// moving the row using the vertical header
    //list->verticalHeader()->setSectionsMovable(true);
    //list->verticalHeader()->setDragEnabled(true);
    //list->verticalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    //connect(list->verticalHeader(), &QHeaderView::sectionMoved, this, &KeyListDlg::onHeaderReorder);

    connect(list, &QTableView::clicked, this, &KeyListDlg::onCellClicked);
    connect(list, &QTableView::doubleClicked, this, &KeyListDlg::onCellDblClicked);

    //list->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(list, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnContextMenu(const QPoint&)));
}


/* * * * * * * * * * * * * * * * * * * * * * * *
 *   Mission imposible !!
 *   Ανανέωση Λίστας μετά από εισαγογή νέων Buttons
 *
void KeyListDlg::reload()
{
    ///// methode 1
    //model->deleteLater();
    //model = new KeyListModel(this, hdr, key);
    //list->setModel(model);
    //list->viewport()->update();


    ///// methode 3
    //QItemSelectionModel *m = list->selectionModel();
    //model = new KeyListModel(this, hdr, key);
    //list->setModel(model);
    //delete m;

    ///// methode 4
    //model->reload(); // my member
}
*/


void KeyListDlg::selectIdx(qint32 idx)
{
    list->selectRow(idx);
}


void KeyListDlg::onCellClicked(const QModelIndex &index)
{
    model->m_drag_row = index.row();
    model->m_drag_col = index.column();
    emit focusKey(model->m_drag_row);
}


void KeyListDlg::onCellDblClicked(const QModelIndex &index)
{
    KEY *k = key+index.row();

    ////{"Label","Data","More","cmd","val 32","Operation","to function","State on/off","Design","Color Text","Color Fill","Color Out","Font","Text Align","Icon","Attribute 1","Attribute 2","Attribute 3","Attribute 4","width (F8,F9)","height (F10,F11)","x [◄-►]","y [▲-▼]","..."};

    switch(index.column())
    {
    case  0: {DlgTextInpout dlg(k->text, 0, this); if(dlg.exec()) break; else return;} //sizeLebel(key[m_row]) : overwrite data error
    case  3: setInt32(k->cmd); break;
    case  4: setInt32(k->val); break;
    case  5: if(setOperation(*k, this))  break; else return;
    case  7: if(setOnOffGroup(*k, this)) break; else return;
    case  8: if(setDesignType(*k, this)) break; else return;
    case  9: if(setColor(k->color[0])) break; else return;
    case 10: if(setColor(k->color[1])) break; else return;
    case 11: {quint32 clr=k->color[2]&0x00ffffff; if(setColor(clr)) {k->color[2]=(k->color[2]&0xff000000)|(clr&0x00ffffff); break;} else return;} // protect key position upper byte data of color[2]
    case 12: if(setFontK(*k, *hdr, this)) break; else return;
    case 13: if(setTxtAlign(*k, this)) break; else return;
    case 14: if(setIcon(*k, this))  break; else return;
    case 15: if(setAttr1(*k, this)) break; else return;
    case 16: if(setAttr2(*k, this)) break; else return;
    case 23: if(setBtPos(*k, *hdr, this)) break; else return;
    default: return;
    }

    emit model->kpd_update(1);
    list->update();
}


/*
void KeyListDlg::onHeaderReorder(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    qDebug() << logicalIndex << oldVisualIndex << newVisualIndex;

    if((kpd->type==0 || kpd->type==2)) /// free keys only
    {
        model->insertkey(oldVisualIndex, newVisualIndex, kpd, pkey);
        //list->
    }
}
*/
/*
#include <QDragEnterEvent>

void KeyListDlg::dragEnterEvent(QDragEnterEvent *ev)
{
    ev->acceptProposedAction();

    model->m_drag_row = list->rowAt(ev->pos().y());
    model->m_drag_col = list->columnAt(ev->pos().x());
}

#include <QDropEvent>

void KeyListDlg::dropEvent ( QDropEvent * ev )
{
    KeyListDlg::dropEvent(ev);

    // this cancels removeRows
    KeyListModel *pModel = dynamic_cast<KeyListModel *>(model());
    if (pModel != NULL)
    {
        if (pModel->getErrorFlag())
        {
            ev->setDropAction(Qt::IgnoreAction);
        }
    }
}
*/
/*
#include <stdarg.h>

void setTableCol(QTableView &lst, int max, ...)
{
    va_list arg;
    va_start(arg, max);

    //while ((val = va_arg(arg,int)) >= 0)
    for(int c=0; c<max; ++c)
    {
        //lst.setColumnWidth(c, va_arg(arg,int));
        lst.setColumnHidden(c, (val<0));
    }

    //va_end(ap);
}


void KeyListDlg::setListDetails(qint16 Details)
{
    list->resizeColumnsToContents();

    m_ListDetails=Details%3;

    switch(m_ListDetails)
    {
    case 0:
    {
        setTableCol(*list, 24, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        break;
    }
    case 1:
    {
        setTableCol(*list, 11, sz, sz, sz, sz, sz, sz, 0, 0, 0, 0, 0);
        break;
    }
    case 2:
    {
        setTableCol(*list, 11, sz, 0, 0, 0, 0, 0, sz, sz, sz, sz, 0);
    }
    /*
    case 0: // VERTICAL HEADERS: Names
    {
        list->verticalHeader()->show();
        setTableCol(*list, 11, 0, 110, 110, 110, 110, 150, 230, 230, 230, 200, 0);
        break;
    }
    case 0: // VERTICAL HEADERS: Names
    {
        int sz = (m_cx-list->verticalHeader()->sizeHint().width())/5;
        setTableCol(*list, 11, 0, sz, 0, 0, 0, 0, sz, 0, 0, sz, sz*2);
        break;
    }
    //}

    /// ROW HEIGHT
    if(pln.m_nPoints)
    {
        int sz = (m_cy-128)/(pln.m_nPoints+1);
        sz = (sz>80)?80:((sz<40)?40:sz);
        list->verticalHeader()->setDefaultSectionSize(sz);
    }
}


void KeyListDlg::onListDetails()
{
    setListDetails(m_ListDetails+1);
    //button_ls->setText(set.file_flightplan);
}
*/


void KeyListDlg::onCheckRowMove()
{
    list->setSelectionBehavior(QAbstractItemView::SelectRows);
    model->m_drag_type = true;
}


void KeyListDlg::onCheckFieldCopy()
{
    list->setSelectionBehavior(QAbstractItemView::SelectItems);
    model->m_drag_type = false;
}
