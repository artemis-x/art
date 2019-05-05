#ifndef DLGNUMINPUT_H
#define DLGNUMINPUT_H

#include <QDialog>

namespace Ui {
class DlgNumInput;
}

class DlgNumInput : public QDialog
{
    Q_OBJECT

public:
    explicit DlgNumInput(qint32 val, QWidget *parent = 0);
    ~DlgNumInput();

    //int m_type;
    qint32 m_val;

private slots:
    void on_btOk_clicked();
    void on_btCncl_clicked();
    void on_rdBt1_clicked(bool checked);
    void on_rdBt2_clicked(bool checked);
    void on_rdBt3_clicked(bool checked);
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::DlgNumInput *ui;
};

#endif // DLGNUMINPUT_H
