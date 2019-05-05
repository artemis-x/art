#include "HdrList.h"
#include <QMenu>
#include <QHeaderView>
#include <QResizeEvent>
#include "dlgnuminput.h"
#include "dlgscreenpos.h"


HdrTableView::HdrTableView(QWidget *parent, BuildKeypad *kpd_wnd, KEYHDR *kpd_hdr) :
    QTableView(parent)
{
    setStyleSheet("*{font-size:13pt;}"
                  "QHeaderView::section {background-color:#e0e0a0}");

    /*
    setStyleSheet(//"QDialog {background-color:#000000} "
                  "QPushButton {font-size:34px; background-color:#002222; color:#00FFFF; border:3px solid #008080;} "
                  "QTableView {font-size:40px; border:none; selection-background-color:#000000; selection-color:#FFFFFF; background-color:#00ffff; color:#000000} "
                  "QHeaderView::section {font-size:28px; border:none; background-color:#002222; color:#00ffff} "
                  "QHeaderView {background-color:#002222} "
                  "QTableCornerButton::section {background:#002222}"
                  );
    */

    /////////////////////////////////////////////////////  LIST
    kpd = kpd_wnd;
    hdr = kpd_hdr;

    ListIni();
    //setGeometry(x, y, 500, 500);
    showMaximized();
    //show();
}


void HdrTableView::ListIni()
{
    model = new HdrListModel(this, kpd, hdr);
    setModel(model);

    //list->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //list->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //list->verticalHeader()->setDefaultSectionSize(40);
    //list->verticalHeader()->setMinimumWidth(25);
    //list->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //list->horizontalHeader()->setMinimumSectionSize(0);
    horizontalHeader()->setStretchLastSection(true);
    //list->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //list->horizontalHeader()->setSectionsMovable(true);
    verticalHeader()->setSectionsMovable(true);
    horizontalHeader()->hide();

    //list->setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection );
    //list->setSelectionBehavior(QAbstractItemView::SelectRows);
    //list->setEditTriggers(QAbstractItemView::NoEditTriggers);//stop the auto edit.
    //list->setEditTriggers(QAbstractItemView::DoubleClicked);
    //list->setSortingEnabled(true);

    //list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ////// moving the row using the inernal list & model
    //list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //list->setDragEnabled(true);
    //list->setAcceptDrops(true);
    //list->setDropIndicatorShown(true);

    //list->setDragDropMode(QAbstractItemView::InternalMove);
    //list->setDragDropMode(QAbstractItemView::DragDrop);
    //list->setDefaultDropAction(Qt::MoveAction);
    //list->setDragDropOverwriteMode(false);

    ////// moving the row using the vertical header
    //list->verticalHeader()->setSectionsMovable(true);
    //list->verticalHeader()->setDragEnabled(true);
    //list->verticalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    //connect(list->verticalHeader(), &QHeaderView::sectionMoved, this, &KeyListDlg::onHeaderReorder);

    //connect(list, &QTableView::clicked, this, &KeyListDlg::onCellClicked);
    connect(this, &HdrTableView::doubleClicked, this, &HdrTableView::onCellDblClicked);

    //list->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(list, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnContextMenu(const QPoint&)));
}


void HdrTableView::onCellDblClicked(const QModelIndex &index)
{
    switch(index.row())
    {
    case 2: setInt32(hdr->data); break;
    case 3: WallOptions(); break;       // redraws inside
    case 4: kpd->setWallpaper(); break; // redraws inside
    case 5: kpd->setBkColor();   break; // redraws inside
    case 6: openFont(0); break;
    case 7: openFont(1); break;
    case 8: openFont(2); break;
    case 9: openFont(3); break;
    case 23:openScreenPos(); return;
    default: return;
    }

    emit model->kpd_update(0);
    update();
}


void HdrTableView::WallOptions()
{
    QMenu  Menu( this );
    Menu.addAction(pWllM[0],  this , [=] {hdr->bckgrType=0;});
    Menu.addAction(pWllM[1],  this , [=] {hdr->bckgrType=1;});
    Menu.addAction(pWllM[2],  this , [=] {hdr->bckgrType=2;});
    Menu.addAction(pWllM[3],  this , [=] {hdr->bckgrType=3;});
    Menu.addAction(pWllM[4],  this , [=] {hdr->bckgrType=4;});
    Menu.addAction(pWllM[5],  this , [=] {hdr->bckgrType=5;});
    Menu.addAction(pWllM[6],  this , [=] {hdr->bckgrType=6;});
    if(Menu.exec( QCursor::pos() ))
    {
        emit model->kpd_update(2);
    }
}


bool HdrTableView::openFont(int n)
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, kpd->m_fnt[n], this);

    if (ok)
    {
        memset(&hdr->font[n], 0, sizeof(FONTSET2));
        strncpy(hdr->font[n].Name, font.family().toLatin1().constData(), sizeof(FONTSET2::Name));
        hdr->font[n].Size = font.pointSize();
        hdr->font[n].Weight = font.weight();
        hdr->font[n].attr = (font.italic())?0x01:0 | (font.underline())?0x02:0;

        kpd->m_fnt[n].setFamily(hdr->font[n].Name);
        kpd->m_fnt[n].setPointSize(hdr->font[n].Size);
        kpd->m_fnt[n].setWeight(hdr->font[n].Weight);
        kpd->m_fnt[n].setItalic(hdr->font[n].attr&0x01);

        emit model->kpd_update(1);

        return true;
    }

    return false;
}


void HdrTableView::openScreenPos()
{
    DlgScreenPos dlg(hdr->position, this);

    if(dlg.exec())
    {
        hdr->position=dlg.m_flags;
    }
}
