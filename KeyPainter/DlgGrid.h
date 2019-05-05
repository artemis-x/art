#ifndef DLGKEYPAD_H
#define DLGKEYPAD_H

#include <QDialog>

namespace Ui {
class DlgKeypad;
}

class DlgKeypad : public QDialog
{
    Q_OBJECT

public:
    explicit DlgKeypad(QWidget *parent = 0);
    ~DlgKeypad();

    int m_col;
    int m_row;
    int m_hor;
    int m_ver;
    int m_lft;
    int m_up;
    int m_rgt;
    int m_dn;
    int m_scan;

    void SetDlgData();
    void GetDlgData();

private:
    Ui::DlgKeypad *ui;
};

#endif // DLGKEYPAD_H
