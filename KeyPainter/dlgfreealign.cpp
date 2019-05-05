#include "dlgfreealign.h"
#include "ui_dlgfreealign.h"


dlgfreealign::dlgfreealign(quint32 flags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgfreealign)
{
    ui->setupUi(this);

    m_flags = flags;

    ui->chXZ->setChecked(flags&0x01000000);
    ui->chXL->setChecked(flags&0x02000000);
    ui->chYZ->setChecked(flags&0x04000000);
    ui->chYL->setChecked(flags&0x08000000);
    ui->chX2->setChecked(flags&0x10000000);
    ui->chY2->setChecked(flags&0x20000000);
    ui->chXM->setChecked(flags&0x40000000);
    ui->chYM->setChecked(flags&0x80000000);
}


dlgfreealign::~dlgfreealign()
{
    delete ui;
}


void dlgfreealign::on_buttonBox_accepted()
{
    m_flags = 0;

    m_flags|= (ui->chXZ->isChecked()?0x01000000:0);
    m_flags|= (ui->chXL->isChecked()?0x02000000:0);
    m_flags|= (ui->chYZ->isChecked()?0x04000000:0);
    m_flags|= (ui->chYL->isChecked()?0x08000000:0);
    m_flags|= (ui->chX2->isChecked()?0x10000000:0);
    m_flags|= (ui->chY2->isChecked()?0x20000000:0);
    m_flags|= (ui->chXM->isChecked()?0x40000000:0);
    m_flags|= (ui->chYM->isChecked()?0x80000000:0);

    done(1);
}


void dlgfreealign::on_buttonBox_rejected()
{
    reject();
}


void dlgfreealign::on_chXZ_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chXL->setChecked(false);
        ui->chX2->setChecked(false);
        ui->chXM->setChecked(false);
    }
}

void dlgfreealign::on_chXL_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chXZ->setChecked(false);
        ui->chXM->setChecked(false);
    }
}

void dlgfreealign::on_chXM_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chXZ->setChecked(false);
        ui->chXL->setChecked(false);
    }
}

void dlgfreealign::on_chX2_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chXZ->setChecked(false);
    }
}



void dlgfreealign::on_chYZ_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chYL->setChecked(false);
        ui->chY2->setChecked(false);
        ui->chYM->setChecked(false);
    }
}

void dlgfreealign::on_chYL_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chYZ->setChecked(false);
        ui->chYM->setChecked(false);
    }
}

void dlgfreealign::on_chYM_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chYZ->setChecked(false);
        ui->chYL->setChecked(false);
    }
}

void dlgfreealign::on_chY2_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->chYZ->setChecked(false);
    }
}
