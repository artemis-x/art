#include "dlgtxtalign.h"
#include "ui_dlgtxtalign.h"

DlgTxtAlign::DlgTxtAlign(quint8 flags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTxtAlign)
{
    ui->setupUi(this);

    m_flags = flags;

    ui->chL->setChecked(flags&0x10);
    ui->chR->setChecked(flags&0x20);
    ui->chT->setChecked(flags&0x40);
    ui->chB->setChecked(flags&0x80);
}

DlgTxtAlign::~DlgTxtAlign()
{
    delete ui;
}

void DlgTxtAlign::on_buttonBox_accepted()
{
    m_flags = (ui->chL->isChecked()?0x10:0);
    m_flags|= (ui->chR->isChecked()?0x20:0);
    m_flags|= (ui->chT->isChecked()?0x40:0);
    m_flags|= (ui->chB->isChecked()?0x80:0);
}

void DlgTxtAlign::on_chL_clicked(bool checked)
{
    if(checked) ui->chR->setChecked(false);
}

void DlgTxtAlign::on_chR_clicked(bool checked)
{
    if(checked) ui->chL->setChecked(false);
}

void DlgTxtAlign::on_chT_clicked(bool checked)
{
    if(checked) ui->chB->setChecked(false);
}

void DlgTxtAlign::on_chB_clicked(bool checked)
{
    if(checked) ui->chT->setChecked(false);
}
