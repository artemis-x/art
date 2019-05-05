#include "dlgdesignrectdgr.h"
#include "ui_dlgdesignrectdgr.h"

DlgDesignButtonAr::DlgDesignButtonAr(KEY *ikey, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDesignButtonAr)
{
    ui->setupUi(this);

    pkey = ikey;

    ui->chx->setChecked(pkey->attr2&0x10);
    ui->chy->setChecked(pkey->attr2&0x20);
    ui->chcx->setChecked(pkey->attr2&0x40);
    ui->chcy->setChecked(pkey->attr2&0x80);
    ui->chmax->setChecked(pkey->attr2&0x1);
    ui->thickness->setValue(pkey->attr1);
    ui->step1->setValue(pkey->attr3);
    ui->step2->setValue(pkey->attr4);
}

DlgDesignButtonAr::~DlgDesignButtonAr()
{
    delete ui;
}

void DlgDesignButtonAr::on_ok_clicked()
{
    pkey->attr2 = (ui->chmax->isChecked()?0x01:0)
                | (ui->chx->isChecked()?0x10:0)
                | (ui->chy->isChecked()?0x20:0)
                | (ui->chcx->isChecked()?0x40:0)
                | (ui->chcy->isChecked()?0x80:0);

    pkey->attr1 = ui->thickness->value();
    pkey->attr3 = ui->step1->value();
    pkey->attr4 = ui->step2->value();

    done(1);
}

void DlgDesignButtonAr::on_cancel_clicked()
{
    reject();
}
