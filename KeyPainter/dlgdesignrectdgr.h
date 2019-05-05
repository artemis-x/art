#ifndef DLGDESIGNBUTTONAR_H
#define DLGDESIGNBUTTONAR_H

#include <QDialog>
#include "KeyPad.h"


namespace Ui {
class DlgDesignButtonAr;
}


class DlgDesignButtonAr : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDesignButtonAr(KEY *ikey, QWidget *parent = 0);
    ~DlgDesignButtonAr();

    KEY *pkey;

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();

private:
    Ui::DlgDesignButtonAr *ui;
};

#endif // DLGDESIGNBUTTONAR_H
