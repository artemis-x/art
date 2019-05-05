#ifndef DLGSETBACKGROUND_H
#define DLGSETBACKGROUND_H

#include <QDialog>
#include "QtWidgets"
#include "BuildKeypad.h"


class DlgSetBackground : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSetBackground(KEYHDR *padhdr, QWidget *parent = nullptr);
    //~DlgSetBackground();

    ////////////////// COMBO {"Fill Color","Linear Gradient","Radial Gradient","Preset Gradient","2 x Fill Color","Picture (keep size)","Picture (scale size)"};
    enum Back { Fill, Linear, Radial, Pict, PictScale, Preset, Fill2 };

    KEYHDR *m_padhdr;
    int m_cx;
    int m_cy;

    QPushButton *btn1,*btn2,*btn3;
    QLineEdit m_EdtClr1;
    QLineEdit m_EdtClr2;
    QLineEdit m_EdtClr3;

    void ContextMenu(int x, int y);

private:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private slots:
    void setBtClr1();
    void setBtClr2();
    void setBtClr3();
    void OnChangeEdtClr1(const QString val);
    void OnChangeEdtClr2(const QString val);
    void OnChangeEdtClr3(const QString val);
    void OnChangeCombo(int idx);
};

#endif // DLGSETBACKGROUND_H
