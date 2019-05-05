#ifndef DLGKEYPASTE_H
#define DLGKEYPASTE_H

#include <QDialog>
#include "KeyPad.h"
#include "KeyData.h"

namespace Ui {
class DlgKeyPaste;
}


class DlgKeyPaste : public QDialog
{
    Q_OBJECT

public:
    explicit DlgKeyPaste(int from, int to, KEYHDR *kpd_hdr, KEY *kpd_keys, int copy_idx, QWidget *parent = 0);
    ~DlgKeyPaste();

    int m_Fr, m_To;
    quint32 m_paste_flags;
    KeyDataView *keydata;

private slots:
    void on_BtOk_clicked();
    void on_BtCn_clicked();
    void on_checkBox_clicked();

private:
    Ui::DlgKeyPaste *ui;
};

#endif // DLGKEYPASTE_H
