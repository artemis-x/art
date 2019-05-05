#include "KeyData.h"
#include <QHeaderView>
#include "dlgtextinpout.h"



KeyDataView::KeyDataView(QWidget *parent, KEYHDR *kpd_hdr, KEY *kpd_keys) :
    QTableView(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose); // on close it deletes the pointer too

    setStyleSheet("*{font-size:13pt;}"
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
    keys = kpd_keys;

    ListIni();
    showMaximized();
}


void KeyDataView::ListIni()
{
    model = new KeyDataModel(this, hdr, keys);
    setModel(model);

    //verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //list->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //list->verticalHeader()->setDefaultSectionSize(40);
    //list->verticalHeader()->setMinimumWidth(25);
    //list->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //list->horizontalHeader()->setMinimumSectionSize(0);
    horizontalHeader()->setStretchLastSection(true);
    //list->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionsMovable(true);
    horizontalHeader()->hide();

    //list->setSelectionMode(QAbstractItemView::SingleSelection );
    //list->setSelectionBehavior(QAbstractItemView::SelectRows);
    //list->setEditTriggers(QAbstractItemView::NoEditTriggers);//stop the auto edit.
    //list->setEditTriggers(QAbstractItemView::DoubleClicked);
    //list->setSortingEnabled(true);

    //list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ////// moving the row using the vertical header
    //list->verticalHeader()->setSectionsMovable(true);
    //list->verticalHeader()->setDragEnabled(true);
    //list->verticalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    //connect(list->verticalHeader(), &QHeaderView::sectionMoved, this, &KeyListDlg::onHeaderReorder);

    connect(this, &QTableView::doubleClicked, this, &KeyDataView::onCellDblClicked);

    //list->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(list, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnContextMenu(const QPoint&)));
}


void KeyDataView::onCellDblClicked(const QModelIndex &index)
{
    if(model->m_idx<0) return;

    ////{"Label","Data","More","cmd","val 32","Operation","to function","State on/off","Design","Color Text","Color Fill","Color Out","Font","Text Align","Icon","Attribute 1","Attribute 2","Attribute 3","Attribute 4","width (F8,F9)","height (F10,F11)","x [◄-►]","y [▲-▼]","..."};

    switch(index.row())
    {
    case  0: {DlgTextInpout dlg(model->key->text, 0, this); if(dlg.exec()) break; else return;} //sizeLebel(key[m_row]) : overwrite data error
    case  3: setInt32(model->key->cmd); break;
    case  4: setInt32(model->key->val); break;
    case  5: if(setOperation(*model->key, this))  break; else return;
    case  7: if(setOnOffGroup(*model->key, this)) break; else return;
    case  8: if(setDesignType(*model->key, this)) break; else return;
    case  9: if(setColor(model->key->color[0])) break; else return;
    case 10: if(setColor(model->key->color[1])) break; else return;
    case 11: {quint32 clr=model->key->color[2]&0x00ffffff; if(setColor(clr)) {model->key->color[2]=(model->key->color[2]&0xff000000)|(clr&0x00ffffff); break;} else return;} // protect key position upper byte data of color[2]
    case 12: if(setFontK(*model->key, *hdr, this)) break; else return;
    case 13: if(setTxtAlign(*model->key, this)) break; else return;
    case 14: if(setIcon(*model->key, this))  break; else return;
    case 15: if(setAttr1(*model->key, this)) break; else return;
    case 16: if(setAttr2(*model->key, this)) break; else return;
    case 23: if(setBtPos(*model->key, *hdr, this)) break; else return;
    default: return;
    }

    emit model->kpd_update(1);
    update();
}


void KeyDataView::selectIdx(qint32 idx)
{
    model->newKey(idx);
}
