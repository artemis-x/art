#ifndef DLGSCREENPOS_H
#define DLGSCREENPOS_H

#include <QDialog>

namespace Ui {
class DlgScreenPos;
}

class DlgScreenPos : public QDialog
{
    Q_OBJECT

public:
    explicit DlgScreenPos(quint32 flags, QWidget *parent = nullptr);
    ~DlgScreenPos();

    quint32 m_flags;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgScreenPos *ui;
};

#endif // DLGSCREENPOS_H
