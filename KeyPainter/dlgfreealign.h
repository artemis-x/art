#ifndef DLGFREEALIGN_H
#define DLGFREEALIGN_H

#include <QDialog>

namespace Ui {
class dlgfreealign;
}

class dlgfreealign : public QDialog
{
    Q_OBJECT

public:
    explicit dlgfreealign(quint32 flags, QWidget *parent = 0);
    ~dlgfreealign();

    quint32 m_flags;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_chXZ_stateChanged(int arg1);
    void on_chXL_stateChanged(int arg1);
    void on_chYZ_stateChanged(int arg1);
    void on_chYL_stateChanged(int arg1);

    void on_chXM_stateChanged(int arg1);

    void on_chYM_stateChanged(int arg1);

    void on_chY2_stateChanged(int arg1);

    void on_chX2_stateChanged(int arg1);

private:
    Ui::dlgfreealign *ui;
};

#endif // DLGFREEALIGN_H
