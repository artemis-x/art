#include "dlgscreenpos.h"
#include "ui_dlgscreenpos.h"


DlgScreenPos::DlgScreenPos(quint32 flags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgScreenPos)
{
    ui->setupUi(this);

    m_flags = flags;

    ui->chXL->setChecked(flags&0x01);
    ui->chYL->setChecked(flags&0x02);
    ui->chXZ->setChecked(flags&0x04);
    ui->chYZ->setChecked(flags&0x08);
    ui->chZT->setChecked(flags&0x10);
    ui->chZB->setChecked(flags&0x20);
}

DlgScreenPos::~DlgScreenPos()
{
    delete ui;
}

void DlgScreenPos::on_buttonBox_accepted()
{
    m_flags = 0;

    m_flags|= (ui->chXL->isChecked()?0x01:0);
    m_flags|= (ui->chYL->isChecked()?0x02:0);
    m_flags|= (ui->chXZ->isChecked()?0x04:0);
    m_flags|= (ui->chYZ->isChecked()?0x08:0);
    m_flags|= (ui->chZT->isChecked()?0x10:0);
    m_flags|= (ui->chZB->isChecked()?0x20:0);

    done(1);
}
