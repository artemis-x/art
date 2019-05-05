#include "ButtonsBar.h"


ButtonsBar::ButtonsBar(QWidget *parent) : KeyPad(parent)
{
    m_idx = -1;
}


///////////////////////////////////////////////////////////  MOUSE
/*
  QPalette palette;
  palette.setBrush(QPalette::Foreground, QBrush(Qt::green));
  palette.setBrush(QPalette::Base, QBrush(Qt::red));

  rubberBand->setPalette(palette);
  rubberBand->resize(30, 30);
*/


void ButtonsBar::mousePressEvent(QMouseEvent *ev)
{
    //////////////////////////// MOUSE ON KEY
    m_idx=idx_from_pos(ev->pos().x(), ev->pos().y());
    if(m_idx<0)	return;
    pkeySel = pkey+m_idx;
    ////////////////////////////
    m_drag_x = ev->x();
    m_drag_y = ev->y();

    //////////////////////////////
    if (pkey[m_idx].oper&0x10)    // KeyPad Change
    {
        ReplaceKeypad(pkey[m_idx].data);
    }
}


/*
 * FIND OUT THE WINDGET CLASS
 * --------------------------
   BuildKeypad *child = static_cast<BuildKeypad*>(parentWidget()->childAt(ev->pos()));
   if (!child)  return;
   qDebug(child->objectName().toLatin1().constData());
   qDebug(child->metaObject()->className());
*/


void ButtonsBar::mouseReleaseEvent(QMouseEvent *ev)
{
    //emit KeyPressedMessage(m_idx, 0, "");
}


/*
void  setMouseTracking ( bool enable )
--------------------------------------
If mouse tracking is disabled (the default),
the widget only receives mouseMoveEvent when at least
one mouse button is pressed while the mouse is being moved.
If mouse tracking is enabled, the widget receives mouse move events
even if no buttons are pressed.
*/


void ButtonsBar::mouseMoveEvent(QMouseEvent *ev)
{
    if((abs(m_drag_x-ev->x())>12 || abs(m_drag_y-ev->y())>12))
    {
        ////////////////  Drag & Drop
        QPixmap pixmap = m_xmp->copy( pkey[m_idx].x, pkey[m_idx].y, pkey[m_idx].cx, pkey[m_idx].cy );

        QByteArray itemData;
        itemData.append((const char*)&pkey[m_idx],sizeof(KEY));

        QMimeData *mimeData = new QMimeData;
        mimeData->setData("keypad/insert-key", itemData);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);

        drag->setHotSpot(QPoint(pkey[m_idx].cx/2, pkey[m_idx].cy/2));
        //drag->setHotSpot(ev->pos()-QPoint(pkey[m_idx].x, pkey[m_idx].y));
        //drag->setHotSpot(ev->pos()-QPoint(pkey[m_idx].x-(pkey[m_idx].cx/2), pkey[m_idx].y-(pkey[m_idx].cy/2))-QPoint(pkey[m_idx].x-(pkey[m_idx].cx/2), pkey[m_idx].y-(pkey[m_idx].cy/2));
        //drag->setHotSpot(ev->pos()-QPoint(pkey[m_idx].x, pkey[m_idx].y)-QPoint(pkey[m_idx].cx/2, pkey[m_idx].cy/2));

        ////////////////  returns from exec() when destination widget receives the dropEvent()
        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        {
            //child->close();
        }
        else
        {
            //child->show();
        }
    }
}

/*
void ButtonsBar::FocusOnKey(int idx)
{
    if(idx<0)
    {
        m_rubber->hide();
    }
    else
    {
        m_rubber->setGeometry(QRect((QPoint(pkey[idx].x-m_or_x, pkey[idx].y-m_or_y)), QSize(pkey[idx].cx, pkey[idx].cy)));
        m_rubber->show();
    }
    m_idx = idx;
}
*/

/*
 *  Watch the difference between "Window" & "Widget"
 *  -----------------------------------------------
 *
void MyWidget::mousePressEvent(QMouseEvent* event)
{
    event->accept(); // do not propagate
    if (isWindow())
        offset = event->globalPos() - pos();
    else
        offset = event->pos();
}

void MyWidget::mouseMoveEvent(QMouseEvent* event)
{
    event->accept(); // do not propagate
    if (isWindow())
        move(event->globalPos() - offset);
    else
        move(mapToParent(event->pos() - offset));
}

void MyWidget::mouseReleaseEvent(QMouseEvent* event)
{
    event->accept(); // do not propagate
    offset = QPoint();
}
*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
