#ifndef DLGMESSAGEOPER_H
#define DLGMESSAGEOPER_H

#include <QDialog>

namespace Ui {
class dlgmessageoper;
}

class dlgmessageoper : public QDialog
{
    Q_OBJECT

public:
    explicit dlgmessageoper(quint8 oper, char *function, char *keypad, QWidget *parent = 0);
    ~dlgmessageoper();

    quint8 m_oper;
    char *m_function;
    char *m_keypad;

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_cmbMessage_currentIndexChanged(int index);
    void on_cmbChange_currentIndexChanged(int index);

private:
    Ui::dlgmessageoper *ui;
};

#endif // DLGMESSAGEOPER_H
