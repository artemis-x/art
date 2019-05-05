#include "DlgGrid.h"
#include "ui_dlgkeypad.h"
//#include "db_functions.h"


DlgKeypad::DlgKeypad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgKeypad)
{
    ui->setupUi(this);
}

DlgKeypad::~DlgKeypad()
{
    delete ui;
}

void DlgKeypad::SetDlgData()
{
    ui->col->setText(QString::number(m_col));
    ui->row->setText(QString::number(m_row));
    ui->space_hor->setText(QString::number(m_hor));
    ui->space_ver->setText(QString::number(m_ver));
    ui->mrg_lf->setText(QString::number(m_lft));
    ui->mrg_up->setText(QString::number(m_up));
    ui->mrg_rt->setText(QString::number(m_rgt));
    ui->mrg_dn->setText(QString::number(m_dn));
    ui->cmbScan->setCurrentIndex(m_scan);

    //LoadDBComboBox("SELECT Name, SubCatID FROM ProductSubCat WHERE CategoryID=1", ui->cmbCateg);
}

void DlgKeypad::GetDlgData()
{
    m_col = ui->col->text().toUInt();
    m_row = ui->row->text().toUInt();
    m_hor = ui->space_hor->text().toUInt();
    m_ver = ui->space_ver->text().toUInt();
    m_lft = ui->mrg_lf->text().toInt();
    m_up  = ui->mrg_up->text().toInt();
    m_rgt = ui->mrg_rt->text().toInt();
    m_dn  = ui->mrg_dn->text().toInt();
    m_scan= ui->cmbScan->currentIndex();

    ///////////////////////////// auto products loading
    //if(ui->chkCateg->isChecked())
    //{
    //    m_prod_categ = ui->cmbCateg->currentData().toInt();
    //}
    //else
    //{
    //    m_prod_categ = -1;
    //}
}


/*
#include "KeyPad.h"


int KeypadDBLoad(const char* sql, KEY *keys, int max)
{
    if(!db.isOpen())
    {
        LoadConfig();
        if(!db.isOpen()) return 0;
    }
    /////////////////

    QSqlQuery qr(db);
    qr.exec(sql);

    int idx=0;

    while( qr.next() )
    {
        keys[idx].cmd= -21;	//	KEY_PID = -21	(LONG WITH PRODUCT_ID in DB)
        //pkey[idx].lp = ProductID;
        strncpy(keys[idx].data, qr.value(1).toString().toLatin1().constData(), sizeof(KEY::data));
        strncpy(keys[idx].text, qr.value(0).toString().toLocal8Bit().constData(), sizeof(KEY::text));

        idx++;
        if(idx>=max) return idx;
    }

    // clear the rest keys
    for(int i=idx; i<max; i++)
    {
        keys[i].cmd = 0;
        keys[i].text[0]=0;
        keys[i].data[0]=0;
    }

    return idx;
}
*/
