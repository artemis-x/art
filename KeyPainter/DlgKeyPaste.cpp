#include "DlgKeyPaste.h"
#include "ui_DlgKeyPaste.h"


DlgKeyPaste::DlgKeyPaste(int from, int to, KEYHDR *kpd_hdr, KEY *kpd_keys, int copy_idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgKeyPaste)
{
    ui->setupUi(this);

    ui->spinBox1->setRange(1, to);
    ui->spinBox2->setRange(1, to);
    ui->spinBox1->setValue(from);
    ui->spinBox2->setValue(to);

    keydata = new KeyDataView(this, kpd_hdr, kpd_keys);
    keydata->model->m_itemflagCheck = (Qt::ItemFlags)Qt::CheckStateRole | Qt::ItemIsUserCheckable;
    keydata->model->m_itemflagEdit = 0;//Qt::ItemIsEditable;
    keydata->setEditTriggers(QAbstractItemView::NoEditTriggers);//stop the auto edit.
    keydata->disconnect();
    keydata->setGeometry(0,0,300,640);
    keydata->selectIdx(copy_idx);
}


DlgKeyPaste::~DlgKeyPaste()
{
    delete ui;
}


void DlgKeyPaste::on_checkBox_clicked()
{    
    keydata->model->m_cflags = (ui->checkBox->isChecked())? 0x9fffff:0x0; // avoid x/y copy
    keydata->model->newKey(keydata->model->m_idx);
}


void DlgKeyPaste::on_BtOk_clicked()
{
    m_Fr = ui->spinBox1->value();
    m_To = ui->spinBox2->value();

    m_paste_flags = (ui->checkBox_C1_2->isChecked()?0x001:0)
                  | (ui->checkBox_C2_2->isChecked()?0x002:0)
                  | (ui->checkBox_C3_2->isChecked()?0x004:0)
                  | (ui->checkBox_DS_2->isChecked()?0x020:0)
                  | (ui->checkBox_FS_2->isChecked()?0x040:0);

    done(1);
}


void DlgKeyPaste::on_BtCn_clicked()
{
    reject();
}


/*

void DlgKeyPaste::on_checkBox_clicked()
{
    bool chk = ui->checkBox->isChecked();

    ui->checkBox_C1->setChecked(chk);
    ui->checkBox_C2->setChecked(chk);
    ui->checkBox_C3->setChecked(chk);
    ui->checkBox_OP->setChecked(chk);
    ui->checkBox_ST->setChecked(chk);
    ui->checkBox_DS->setChecked(chk);
    ui->checkBox_FS->setChecked(chk);
    ui->checkBox_AL->setChecked(chk);
    ui->checkBox_IC->setChecked(chk);
    ui->checkBox_AT->setChecked(chk);
    ui->checkBox_CX->setChecked(chk);
    ui->checkBox_CY->setChecked(chk);
}


void DlgKeyPaste::on_BtOk_clicked()
{
    m_Fr = ui->spinBox1->value();
    m_To = ui->spinBox2->value();

    m_flags = (ui->checkBox_C1->isChecked()?0x001:0)
            | (ui->checkBox_C2->isChecked()?0x002:0)
            | (ui->checkBox_C3->isChecked()?0x004:0)
            | (ui->checkBox_OP->isChecked()?0x008:0)
            | (ui->checkBox_ST->isChecked()?0x010:0)
            | (ui->checkBox_DS->isChecked()?0x020:0)
            | (ui->checkBox_FS->isChecked()?0x040:0)
            | (ui->checkBox_AL->isChecked()?0x080:0)
            | (ui->checkBox_IC->isChecked()?0x100:0)
            | (ui->checkBox_AT->isChecked()?0x200:0)
            | (ui->checkBox_CX->isChecked()?0x400:0)
            | (ui->checkBox_CY->isChecked()?0x800:0);

    m_paste_flags = (ui->checkBox_C1_2->isChecked()?0x001:0)
                  | (ui->checkBox_C2_2->isChecked()?0x002:0)
                  | (ui->checkBox_C3_2->isChecked()?0x004:0)
                  | (ui->checkBox_DS_2->isChecked()?0x020:0)
                  | (ui->checkBox_FS_2->isChecked()?0x040:0);

    done(1);
}

void DlgKeyPaste::on_BtCn_clicked()
{
    reject();
}
*/
