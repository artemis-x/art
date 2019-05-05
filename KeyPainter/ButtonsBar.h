#ifndef BUTTONSBAR_H
#define BUTTONSBAR_H

#include <QWidget>
#include <QtWidgets>
#include <QRubberBand>
#include <QPoint>
#include "KeyPad.h"


class ButtonsBar : public KeyPad
{
    Q_OBJECT

public:
    explicit ButtonsBar(QWidget *parent = 0);
    //~ButtonsBar();

    int		m_drag_x;
    int		m_drag_y;
    //QRubberBand *m_rubber;

private:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

protected:
    //void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    //void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    //void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
};


#endif // BUTTONSBAR_H
