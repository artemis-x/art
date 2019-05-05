#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtWidgets"
#include "BuildKeypad.h"
#include "ButtonsBar.h"
#include "IconsBar.h"
#include "KeyList.h"
#include "HdrList.h"
#include "KeyData.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    QString OpenFilePath;

    BuildKeypad pad;
    QScrollArea kpdscrl;
    ButtonsBar keybar;
    IconsBar iconbar;
    QLabel *LbKey;
    HdrTableView *listhdr=0;
    KeyDataView *keydata=0;
    KeyListDlg *listkeys=0;

    void SetKeyData(qint32 idx);
    void setUpdate(int level);

    void Open();
    bool Save();
    bool maybeSave();
    void OpenIconBar();
    void OpenButtonBar();
    void onKeyList();
    void onKeyData();
    void onHdrList();
    void setDrawType();
    void OpenFile(const QString &filepath);
    void closeEvent(QCloseEvent *event);
};


/* * * * * * * * * * * * * * * * * * * * * * * * * *
implement vertical tabs in QT
have to implement a custom QTabBar overwriting the tabSizeHint() and paintEvent()

#include <QStyleOptionTab>
#include <QStylePainter>
#include <QTabBar>
#include <QTabWidget>

class TabBar: public QTabBar
{
public:
    QSize tabSizeHint(int index) const{
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        return s;
    }
protected:
    void paintEvent(QPaintEvent *){
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for(int i = 0;i < count();i++)
        {
            initStyleOption(&opt,i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
            painter.restore();
        }
    }
};

class TabWidget : public QTabWidget
{
public:
    TabWidget(QWidget *parent=0):QTabWidget(parent)
    {
        setTabBar(new TabBar);
        setTabPosition(QTabWidget::West);
    }
};

    /////////////////
    TabWidget *w = new TabWidget(this);
    w->setGeometry(1040, 0, DISPLAY_CX-1040, DISPLAY_CY);
    w->addTab(&m_inv, "Κατάλογος");
    w->addTab(new InvoiceForm(InvoiceID, reproduct, lock, this), "Κατάλογος");
    w->addTab(new QWidget, "Παραστατικό");
    w->show();
    /////////////////
*/


#endif // MAINWINDOW_H
