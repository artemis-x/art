#include "dlgmessageoper.h"
#include "ui_dlgmessageoper.h"

dlgmessageoper::dlgmessageoper(quint8 oper, char *function, char *keypad, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgmessageoper)
{
    ui->setupUi(this);

    m_oper = oper;

    ui->cmbMessage->setCurrentIndex(oper&0x07);
    ui->cmbChange->setCurrentIndex(oper>>4);
    ui->cmbEvent->setCurrentIndex((oper&0x08)>>3);

    m_function = function;
    m_keypad = keypad;
    ui->edFunction->setText(function);
    ui->edKeypad->setText(keypad);
    on_cmbMessage_currentIndexChanged(oper&0x07);
    on_cmbChange_currentIndexChanged(oper>>4);
}

dlgmessageoper::~dlgmessageoper()
{
    delete ui;
}

void dlgmessageoper::on_cmbMessage_currentIndexChanged(int index)
{
    ui->edFunction->setVisible((index>=2));
    //ui->label->setVisible((index>=2));
}

void dlgmessageoper::on_cmbChange_currentIndexChanged(int index)
{
    ui->edKeypad->setVisible((index==1));
    if(index==1) ui->cmbEvent->setCurrentIndex(1);
}

void dlgmessageoper::on_btnCancel_clicked()
{
    reject();
}

void dlgmessageoper::on_btnOk_clicked()
{
    m_oper = ui->cmbMessage->currentIndex();
    m_oper|= ui->cmbChange->currentIndex()<<4;
    m_oper|= ui->cmbEvent->currentIndex()<<3;

    if((m_oper&0x07)>=0x02) qstrncpy(m_function, ui->edFunction->text().toUtf8().constData(), 15);
    if((m_oper&0xf0)==0x10) qstrncpy(m_keypad, ui->edKeypad->text().toUtf8().constData(), 15);

    done(1);
}
