#include "IconsBar.h"


IconsBar::IconsBar(QWidget *parent) : KeyPad(parent)
{
    m_idx = -1;
    //m_rubber = new QRubberBand(QRubberBand::Rectangle, this);
}


bool IconsBar::SetIconBar(const QByteArray &FileName, const QString &IconsPath)
{
    m_FilePath = IconsPath;
    ///////////////////////  LOAD KEYPAD FILE
    if(LoadKeyPad(FileName))
    {
        setPositionOnDisplay(KPDKPD_CX | KPDKPD_CY); // set m_wnd_cx/cy according m_Position
        NewSizes(kpd.cx, kpd.cy);

        IniIcons(IconsPath);
        LoadNextIcons();

        setGeometry(m_wnd_x, m_wnd_y, m_wnd_cx, m_wnd_cy);
        Redraw();
        return true;
    }

    return false;
}


void IconsBar::IniIcons(const QString &DataPath)
{
    ///////////////// set key color for any "*.ord" file in disk
    QDir finder(DataPath);
    m_files = finder.entryList(QStringList("*.png"), QDir::Files|QDir::NoSymLinks, QDir::Name);

    m_maxPic = m_files.count();
    m_fromPic = 0;
    m_toPic = 0;
    m_nKeys = kpd.nKeys-4;
}


void IconsBar::LoadNextIcons()
{
    m_toPic = qMin(m_fromPic+m_nKeys, m_maxPic);

    for(short i=0; i<m_nKeys; i++)
    {
        int nicon = i+m_fromPic;
        if(nicon >= m_toPic) break;
        QString filename = m_files.at(nicon);

        filename.chop(4); // keep the name: "icon.png" -> "icon"
        //if(filename.size()>=14){}
        strncpy(pkey[i].mem, filename.toUtf8().constData(), sizeof(KEY::mem)); // use mem+data size
        pkey[i].nfnt = 0x2c; // font 0 + icon aspect ratio + text align right
        pkey[i].text[0] = 0;
    }

    /////// BEFORE BUTTON
    if(m_fromPic==0)
    {
        pkey[kpd.nKeys-3].text[0]=0;
        pkey[kpd.nKeys-4].text[0]=0;
    }
    else
    {
        sprintf(pkey[kpd.nKeys-3].text, "%d", m_fromPic);
        strcpy(pkey[kpd.nKeys-4].text, "<<");
    }

    /////// NEXT BUTTON
    int restPic = m_maxPic - m_toPic;

    if(restPic<=0)
    {
        pkey[kpd.nKeys-2].text[0]=0;
        pkey[kpd.nKeys-1].text[0]=0;
    }
    else
    {
        sprintf(pkey[kpd.nKeys-2].text, "%d", restPic);
        strcpy(pkey[kpd.nKeys-1].text, ">>");
    }
}


/*
void IconsBar::LoadIcons(const char *DataPath)
{
    ///////////////// set key color for any "*.ord" file in disk
    QDir finder(DataPath);
    QStringList files = finder.entryList(QStringList("*.png"), QDir::Files|QDir::NoSymLinks, QDir::Name);

    m_maxPic = files.count();
    int max = qMin((int)kpd.nKeys, m_maxPic);
    QString filename;

    for(short i=0; i<max; i++)
    {
        filename = files.at(i);
        filename.chop(4); // keep the name: "icon.png" -> "icon"
        //if(filename.size()>=14){}
        strncpy(pkey[i].mem, filename.toLatin1().constData(), 15);
        pkey[i].nfnt = 0x8c; // icon + text align bottom
        pkey[i].text[0] = 0;
    }

    //m_maxPic
    //m_fromPic = m_toPic;
    //m_toPic  += max;
}
*/

///////////////////////////////////////////////////////////  MOUSE
/*
  QPalette palette;
  palette.setBrush(QPalette::Foreground, QBrush(Qt::green));
  palette.setBrush(QPalette::Base, QBrush(Qt::red));

  rubberBand->setPalette(palette);
  rubberBand->resize(30, 30);
*/


void IconsBar::mousePressEvent(QMouseEvent *ev)
{
    ////////////////// MOUSE ON KEY
    m_idx = idx_from_pos(ev->pos().x(), ev->pos().y());
    //////////////////
    FocusOnKey(m_idx);
    m_drag_x = ev->x();
    m_drag_y = ev->y();
}


/*
 * FIND OUT THE WINDGET CLASS
 * --------------------------
   BuildKeypad *child = static_cast<BuildKeypad*>(parentWidget()->childAt(ev->pos()));
   if (!child)  return;
   qDebug(child->objectName().toLatin1().constData());
   qDebug(child->metaObject()->className());
*/


void IconsBar::mouseReleaseEvent(QMouseEvent *ev)
{
    //emit KeyPressedMessage(m_idx, 0, "");

    /////// NEXT BUTTON
    if(m_idx==kpd.nKeys-1 && m_toPic<m_maxPic)
    {
        m_fromPic += m_nKeys;
        if(m_fromPic+m_nKeys >= m_maxPic) m_fromPic = m_maxPic-m_nKeys;
        LoadNextIcons();
        Redraw();
        return;
    }
    /////// BEFORE BUTTON
    if(m_idx==kpd.nKeys-4 && m_fromPic>0)
    {
        m_fromPic -= m_nKeys;
        if(m_fromPic < 0) m_fromPic = 0;
        LoadNextIcons();
        Redraw();
        return;
    }
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


void IconsBar::mouseMoveEvent(QMouseEvent *ev)
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


void IconsBar::FocusOnKey(int idx)
{
    /*
    if(idx<0)
    {
        m_rubber->hide();
    }
    else
    {
        m_rubber->setGeometry(QRect((QPoint(pkey[idx].x-m_or_x, pkey[idx].y-m_or_y)), QSize(pkey[idx].cx, pkey[idx].cy)));
        m_rubber->show();
    }
    */
    m_idx = idx;
}


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
