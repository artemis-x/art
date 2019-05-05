#ifndef DLGBUTTONX2_H
#define DLGBUTTONX2_H

#include <QDialog>

namespace Ui {
class dlgbuttonx2;
}

class dlgbuttonx2 : public QDialog
{
    Q_OBJECT

public:
    explicit dlgbuttonx2(quint32 flags, QWidget *parent = 0);
    ~dlgbuttonx2();

    quint32 m_flags;

private slots:
    void on_btnOk_clicked();
    void on_btnCn_clicked();

private:
    Ui::dlgbuttonx2 *ui;
};

#endif // DLGBUTTONX2_H
