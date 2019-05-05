#ifndef DLGTXTALIGN_H
#define DLGTXTALIGN_H

#include <QDialog>

namespace Ui {
class DlgTxtAlign;
}

class DlgTxtAlign : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTxtAlign(quint8 flags, QWidget *parent = nullptr);
    ~DlgTxtAlign();

    quint8 m_flags;

private slots:
    void on_buttonBox_accepted();

    void on_chL_clicked(bool checked);
    void on_chR_clicked(bool checked);
    void on_chT_clicked(bool checked);
    void on_chB_clicked(bool checked);

private:
    Ui::DlgTxtAlign *ui;
};

#endif // DLGTXTALIGN_H
