#include "dlgbuttonx2.h"
#include "ui_dlgbuttonx2.h"

dlgbuttonx2::dlgbuttonx2(quint32 flags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgbuttonx2)
{
    ui->setupUi(this);
    m_flags = flags;

    //ui->sldrCX->setRange(0,4);
    ui->sldrCX->setValue((m_flags>>24)&0xf);
    //ui->sldrCY->setRange(0,4);
    ui->sldrCY->setValue((m_flags>>28)&0xf);
}

dlgbuttonx2::~dlgbuttonx2()
{
    delete ui;
}

void dlgbuttonx2::on_btnOk_clicked()
{
    m_flags = (ui->sldrCX->value()<<24) | (ui->sldrCY->value()<<28);

    done(1);
}

void dlgbuttonx2::on_btnCn_clicked()
{
    reject();
}
