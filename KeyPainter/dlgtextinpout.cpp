#include "dlgtextinpout.h"
#include "ui_dlgtextinpout.h"

DlgTextInpout::DlgTextInpout(char *text, int TextSize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTextInpout)
{
    ui->setupUi(this);
    m_pText = text;
    m_TextSize = TextSize;
    ui->edText->setPlainText(text);
}

DlgTextInpout::~DlgTextInpout()
{
    delete ui;
}

void DlgTextInpout::on_btOk_clicked()
{
    //qstrncpy(m_pText, ui->edText->toPlainText().toUtf8().constData(), m_TextSize);
    qstrcpy(m_pText, ui->edText->toPlainText().toUtf8().constData());

    done(1);
}

void DlgTextInpout::on_btCn_clicked()
{
    reject();
}
