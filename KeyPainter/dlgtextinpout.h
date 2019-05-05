#ifndef DLGTEXTINPOUT_H
#define DLGTEXTINPOUT_H

#include <QDialog>

namespace Ui {
class DlgTextInpout;
}

class DlgTextInpout : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTextInpout(char *text, int TextSize, QWidget *parent = 0);
    ~DlgTextInpout();

    char* m_pText;
    int m_TextSize;

private slots:
    void on_btOk_clicked();
    void on_btCn_clicked();

private:
    Ui::DlgTextInpout *ui;
};

#endif // DLGTEXTINPOUT_H
