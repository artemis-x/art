#include "dlgnuminput.h"
#include "ui_dlgnuminput.h"
int m_type=0;

DlgNumInput::DlgNumInput(qint32 val, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgNumInput)
{
    ui->setupUi(this);

    m_val = val;
    ui->lineEdit->setText( QString::number( m_val, 10 ) );
    //ui->label->setText( "= 0x"+QString::number( m_val, 16 ) );

    switch (m_type)
    {
    case 0: ui->rdBt1->setChecked(true); break;
    case 1: ui->rdBt2->setChecked(true); break;
    case 2: ui->rdBt3->setChecked(true);
    }
}

DlgNumInput::~DlgNumInput()
{
    delete ui;
}

void DlgNumInput::on_btOk_clicked()
{
    done(1);
}

void DlgNumInput::on_btCncl_clicked()
{
    reject();
}

void DlgNumInput::on_rdBt1_clicked(bool checked)
{
    m_type = 0;
    on_lineEdit_textChanged(ui->lineEdit->text());
}

void DlgNumInput::on_rdBt2_clicked(bool checked)
{
    m_type = 1;
    on_lineEdit_textChanged(ui->lineEdit->text());
}

void DlgNumInput::on_rdBt3_clicked(bool checked)
{
    m_type = 2;
    on_lineEdit_textChanged(ui->lineEdit->text());
}

void DlgNumInput::on_lineEdit_textChanged(const QString &arg1)
{
    bool ok;

    switch (m_type)
    {
    case 0:
        m_val = arg1.toInt(&ok, 10);
        break;
    case 1:
        m_val = arg1.toInt(&ok, 16);
        break;
    case 2:
        {
            QChar qch(arg1.at(0));
            m_val = qch.unicode();
        }
    }

    ui->label->setText( "= 0x"+QString::number( m_val, 16 ) );

/**************
    notes on my experiance
    UNICODE -to-> STRING CHAR

    "1" = 0x0031 uni = 0x0031 str   (2char: s[0L]=1, s[0H]=3, s[1L]=0, s[1H]=0)
    "Θ" = 0x0398 uni = 0x98CE str   (2char: s[0L]=E, s[0H]=C, s[1L]=8, s[1H]=9)

   quint32 pi = 0x398;          // unicode of "Θ"
   quint16 a1 = (pi&0xff)<<8;
   quint16 a3 = (pi>>4)&0xf0;
   quint16 a4 = (pi>>8)&0xf0;
   pi = a1 | (0xFE - (a4|a3));

   drawtype( (char*)&pi );      // string of "Θ" = CE,98
   ui->label->setText( QString::number( pi, 16 )+" "+(char*)&pi );
**************/
}
