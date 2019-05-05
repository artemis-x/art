#ifndef PRESETBTNS_H
#define PRESETBTNS_H

#include <QWidget>
#include <QtWidgets>
#include <QRubberBand>
#include <QPoint>
#include "KeyPad.h"


/*
class RubberBand : public QRubberBand
{
public:
    RubberBand(QRubberBand::Shape s, QWidget * p = 0):QRubberBand(s, p)
    {
        setAttribute(Qt::WA_Hover);
        setMouseTracking(true); // sent mouseMoveEvent for hover message
    }
    //~RubberBand();

protected:

    void hoverEnter(QHoverEvent *ev)
    {
        setCursor(Qt::SizeAllCursor);
    }


    void hoverLeave(QHoverEvent *ev)
    {
        setCursor(Qt::ArrowCursor);
    }


    void hoverMove(QHoverEvent *ev)
    {
        switch(onRectPosition(ev->pos(), geometry().width(), geometry().height(), 10))
        {
        case 'L':
        case 'R':
            setCursor(Qt::SizeHorCursor);
            break;
        case 'T':
        case 'B':
            setCursor(Qt::SizeVerCursor);
            break;
        case 'I':
            setCursor(Qt::SizeAllCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
        }
    }


    bool event(QEvent *event)
    {
        qDebug() << event->type() << event;

        switch(event->type())
        {
        case QEvent::HoverEnter:
            hoverEnter(static_cast<QHoverEvent*>(event));
            return true;
            break;
        case QEvent::HoverLeave:
            hoverLeave(static_cast<QHoverEvent*>(event));
            return true;
            break;
        case QEvent::HoverMove:
            hoverMove(static_cast<QHoverEvent*>(event));
            return true;
        }
        return QWidget::event(event);
    }

    virtual void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE
{
 QRubberBand::paintEvent(event);
 QPainter p(this);
 p.setPen(QPen(Qt::black,2));
 if(size().width()> 10 && size().height()> 10)
 {
 p.drawText(20,20,QString("%1,%2").arg(size().width()).arg(size().height()));
 }
}
    virtual void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    //void changeEvent(QEvent *) Q_DECL_OVERRIDE;
    void moveEvent(QMoveEvent *) Q_DECL_OVERRIDE
    {

    }

    int onRectPosition(const QPoint &pos, int wcx, int wcy, int borderWidth)
    {
        if(pos.x()<borderWidth)          return 'L'; // onLeft
        if(pos.y()<borderWidth)          return 'T'; // onTop
        if(pos.x()>wcx-borderWidth)      return 'R'; // onRight
        if(pos.y()>wcy-borderWidth)      return 'B'; // onBottom

        //if(x1<0 || y1<0 || x2>0 || y2>0)  return 'O'; // onOutside
        //if(x1>0 && y1>0 && x2<0 && y2<0)  return 'I'; // onInside

        return 'I'; // onInside
    }
};
*/


class BuildKeypad : public KeyPad
{
    Q_OBJECT

public:
    explicit BuildKeypad(QWidget *parent = 0);
    //~BuildKeypad();

    bool m_update_file = false;

    int		m_drag_x;
    int		m_drag_y;
    bool    m_draging;
    //int     m_orbk_x;
    //int     m_orbk_y;

    QRubberBand *m_rubber;
    int m_rctPos=0;

    KEY key;  // copy key-memory
    int copy_idx = -1;

    void Redraw2();
    void ResizeKeyPad();
    void EmptyKeyPad(int size_x, int size_y);
    void BtnOptions(int idx, int x, int y);
    void BtnOptions2(QPoint p);
    void removeArrayKey(int idx);
    void moveArrayKey(int idx1, int idx2);
    void flipPosition(KEY *key1, KEY *key2);
    void flipData(int idx1, int idx2);
    void copyData(KEY *kp1, KEY *kp2);
    void FocusOnKey(int idx);
    void AddKey(int x, int y);
    void AddKey(int x, int y, KEY *keystyle);
    void ChangeKey(int idx, KEY *keystyle);
    void correctKeyGradient(KEY *keystyle_from, KEY *keystyle_to);
    void SetProduct(int idx);

    //int onRectPosition(const QPoint &pos, QRect rct, int borderWidth);
    int onRectPosition(const QPoint &pos, int rx, int ry, int rcx, int rcy, int borderWidth);

    QLineEdit *m_pEditor;
    int m_EditIdx;
    void EditText(int idx);

    void setKeypadType();
    void setGreedKeys(int cols, int rows, int direction);
    void setWallpaper();
    void setBkColor();
    void CreateEmptyKeyPad();
    void EditClosed();

private:
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseHoverEvent(QMouseEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

protected:
    void dragEnterEvent(QDragEnterEvent *ev) Q_DECL_OVERRIDE;
    //void dragMoveEvent(QDragMoveEvent *ev) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *ev) Q_DECL_OVERRIDE;

signals:
    void kpd_update(int level);
 };


#endif // PRESETBTNS_H
