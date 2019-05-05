#include "BuildKeypad.h"
#include "keypad_help_function.h"
#include "DlgKeyPaste.h"


BuildKeypad::BuildKeypad(QWidget *parent) : KeyPad(parent)
{
    m_idx = -1;
    m_draging = false;
    m_rubber = new QRubberBand(QRubberBand::Rectangle, this);

    m_pEditor = NULL;
    m_EditIdx = -1;
    ////////////////////
    setAcceptDrops(true);
    setMouseTracking(true); // sent mouseMoveEvent for hover message
}


void BuildKeypad::Redraw2()
{   
    if(kpd.bckgrType==3 || kpd.bckgrType==4)    // keep image size
    {
        m_xmp = new QPixmap(m_FilePath+kpd.Wallpaper);

        if(kpd.bckgrType==4)                    // scale image to window size
        {
            *m_xmp = m_xmp->scaled(m_wnd_cx, m_wnd_cy, Qt::IgnoreAspectRatio);
        }
    }

    if(kpd.type==1) ResizeKeyGrid(m_wnd_cx, m_wnd_cy); // in case of key size multiplication
    Redraw();
}


void BuildKeypad::ResizeKeyPad()
{
    if(kpd.type==0 && kpd.nKeys) // free keys
    {
        if(QMessageBox::question(this, "new size", "Do you want to adjust the <zoomed or right lock buttons> to the new window size?", QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            ResizeKeyFree(m_wnd_cx, m_wnd_cy);
    }

    m_wnd_cx = kpd.cx;
    m_wnd_cy = kpd.cy;

    NewSizes(kpd.cx, kpd.cy);
    resize(kpd.cx, kpd.cy);
    Redraw();
}


/*
void BuildKeypad::resizeEvent(QResizeEvent *ev)
{
    kpd.cx = m_wnd_cx = ev->size().width();
    kpd.cy = m_wnd_cy = ev->size().height();
}
*/


void BuildKeypad::keyPressEvent(QKeyEvent *ev )
{
    //QWidget::keyPressEvent(ev);
    //Qt::KeyboardModifiers KeyboardModifier = ev->modifiers(); // Qt::AltModifier Qt::ControlModifier

    if(kpd.type==0)     /// free keys
    {
        switch( ev->key() )
        {
        case 16777235:  /// up
            if(pkeySel->y>=0)
            {
                pkeySel->y--;
                if(ev->modifiers() == Qt::AltModifier) pkeySel->cy++;
            }
            break;

        case 16777237:  /// down
            if(pkeySel->y+pkeySel->cy<=kpd.cy)
            {
                if(ev->modifiers() == Qt::AltModifier)
                    pkeySel->cy++;
                else
                    pkeySel->y++;
            }
            break;

        case 16777234:  /// left
            if(pkeySel->x>=0)
            {
                pkeySel->x--;
                if(ev->modifiers() == Qt::AltModifier) pkeySel->cx++;
            }
            break;

        case 16777236:  /// right
            if(pkeySel->x+pkeySel->cx<=kpd.cx)
            {
                if(ev->modifiers() == Qt::AltModifier)
                    pkeySel->cx++;
                else
                    pkeySel->x++;
            }
            break;

        default: return;
        }

        FocusOnKey(m_idx);
    }
}


void BuildKeypad::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->isAutoRepeat()) return; /// Qt by design fires keyReleaseEvent continoualy after keyPressEvent as long you keep down a key

    if(kpd.type==0)    /// free keys
    {
        switch(ev->key())
        {
        case 16777235: /// up
        case 16777237: /// down
        case 16777234: /// left
        case 16777236: /// right
            emit kpd_update(0);
            Redraw();
        }
    }
}


///////////////////////////////////////////////////////////  MOUSE
/*
  QPalette palette;
  palette.setBrush(QPalette::Foreground, QBrush(Qt::green));
  palette.setBrush(QPalette::Base, QBrush(Qt::red));

  rubberBand->setPalette(palette);
  rubberBand->resize(30, 30);
*/

void BuildKeypad::mousePressEvent(QMouseEvent *ev)
{
    //////////////////////////// MOUSE ON KEY
    m_idx = idx_from_pos(ev->pos().x(), ev->pos().y());
    //if(m_idx<0)	return;
    pkeySel = pkey+m_idx;
    ////////////////////////////

    FocusOnKey(m_idx);
    m_drag_x = ev->x();
    m_drag_y = ev->y();

    //////////////////
    if(m_idx<0)     // out of keys
    {
        //m_orbk_x = m_or_x;
        //m_orbk_y = m_or_y;

        if((ev->buttons()&Qt::RightButton)) BtnOptions2(ev->globalPos());
    }
    else            // on key
    {
        if((ev->buttons()&Qt::RightButton)) BtnOptions(m_idx, ev->x(), ev->y());
    }
}


void BuildKeypad::mouseReleaseEvent(QMouseEvent *ev)
{
    if(m_draging && m_idx>=0)   // on key: END of DRAG
    {
        ////////////////// get from rubber x/y + sizes
        int x = m_rubber->geometry().x();
        int y = m_rubber->geometry().y();
        int cx= m_rubber->geometry().width();
        int cy= m_rubber->geometry().height();
        ////////////////// get from mouse x/y
        //int x = pkey[m_idx].x - (m_drag_x - ev->x());
        //int y = pkey[m_idx].y - (m_drag_y - ev->y());
        //int cx= pkey[m_idx].cx;
        //int cy= pkey[m_idx].cy;
        int toidx = idx_from_pos(ev->pos().x(), ev->pos().y());

        if(kpd.type==0) /// free keys
        {
            // protect min size
            if(cx<30) cx=30;
            if(cy<30) cy=30;
            // protect key in window limits
            if(x+cx>kpd.cx) x=kpd.cx-cx;
            if(y+cy>kpd.cy) y=kpd.cy-cy;
            if(x<0) x = 0;
            if(y<0) y = 0;

            pkeySel->x = x;
            pkeySel->y = y;
            pkeySel->cx= cx;
            pkeySel->cy= cy;

            if(toidx>=0 && m_idx!=toidx)
            {
                const QMessageBox::StandardButton ret = QMessageBox::question(this, "exchange buttons",
                                         "Yes:____Flip Position keeping the button's array idx.\n"
                                         "No:_____Flip Position&Data excanging the button's array idx.\n"
                                         "Apply:__Overlap Button.",
                                         QMessageBox::Yes|QMessageBox::No|QMessageBox::Apply);

                switch(ret)
                {
                case QMessageBox::Yes:
                    flipPosition(&pkey[m_idx], &pkey[toidx]);   // 1:keep the array idx
                    break;
                case QMessageBox::No:
                    flipData(m_idx, toidx);                     // 2:excange idx
                }                                               // 3:overlap
            }
        }
        else            /// greed keys
        {
            if(toidx<0) return;

            flipData(m_idx, toidx);
        }

        emit kpd_update(0);
        Redraw2();
    }

    if(m_draging)
    {
        m_draging = false;
        unsetCursor();
    }

    emit KeyPressed(m_idx, 0);
}


void BuildKeypad::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons()&Qt::LeftButton)
    {
        int x = ev->x();
        int y = ev->y();
        if((abs(m_drag_x-x)>8 || abs(m_drag_y-y)>8)) m_draging = true;
        ////////////////    DRAG

        if(m_draging)
        {
            if(m_idx<0)		// out of key moves the view
            {
                //m_or_x = m_drag_x - ev->x() + m_orbk_x;
                //m_or_y = m_drag_y - ev->y() + m_orbk_y;

                // protect map in window limits
                //if(kpd.cx - m_or_x < m_wnd_cx) m_or_x = kpd.cx-m_wnd_cx;
                //if(kpd.cy - m_or_y < m_wnd_cy) m_or_y = kpd.cy-m_wnd_cy;
                //if(m_or_x<0) m_or_x = 0;
                //if(m_or_y<0) m_or_y = 0;

                update();

                setCursor(Qt::SizeAllCursor);
            }
            else            // on key
            {
                if(kpd.type==0) // free keys
                {
                    switch(m_rctPos)
                    {
                    case 'L':
                        m_rubber->setGeometry(QRect((QPoint(x, m_rubber->geometry().top())), m_rubber->geometry().bottomRight()));
                        break;
                    case 'R':
                        m_rubber->setGeometry(QRect(m_rubber->geometry().topLeft(), QPoint(x, m_rubber->geometry().bottom())));
                        break;
                    case 'T':
                        m_rubber->setGeometry(QRect((QPoint(m_rubber->geometry().left(), y)), m_rubber->geometry().bottomRight()));
                        break;
                    case 'B':
                        m_rubber->setGeometry(QRect(m_rubber->geometry().topLeft(), QPoint(m_rubber->geometry().right(), y)));
                        break;
                    case 'I':
                        m_rubber->move( x-(m_rubber->geometry().width()/2), y-(m_rubber->geometry().height()/2) );
                    }
/*
                    int x1 = (m_drag_x + m_or_x) - pkeySel->x;
                    int y1 = (m_drag_y + m_or_y) - pkeySel->y;
                    int x2 = (m_drag_x + m_or_x) - (pkeySel->x + pkeySel->cx);
                    int y2 = (m_drag_y + m_or_y) - (pkeySel->y + pkeySel->cy);

                    int bx1 = pkeySel->x - m_or_x;
                    int by1 = pkeySel->y - m_or_y;
                    int bx2 = pkeySel->x + pkeySel->cx - m_or_x - 1;
                    int by2 = pkeySel->y + pkeySel->cy - m_or_y - 1;

                    if(x1>0 && x1<12)
                    {
                        setCursor(Qt::SizeHorCursor);

                        m_rubber->setGeometry(QRect((QPoint(x, by1)), QPoint(bx2, by2)));
                    }
                    else if(y1>0 && y1<12)
                    {
                        setCursor(Qt::SizeVerCursor);

                        m_rubber->setGeometry(QRect((QPoint(bx1, y)), QPoint(bx2, by2)));
                    }
                    else if(x2<0 && x2>-12)
                    {
                        setCursor(Qt::SizeHorCursor);

                        m_rubber->setGeometry(QRect((QPoint(bx1, by1)), QPoint(x, by2)));
                    }
                    else if(y2<0 && y2>-12)
                    {
                        setCursor(Qt::SizeVerCursor);

                        m_rubber->setGeometry(QRect((QPoint(bx1, by1)), QPoint(bx2, y)));
                    }
                    else
                    {
                        setCursor(Qt::SizeAllCursor);

                        x = (pkeySel->x - (m_drag_x + m_or_x)) + ev->x();
                        y = (pkeySel->y - (m_drag_y + m_or_y)) + ev->y();

                        // protect key in window limits
                        if(x+pkeySel->cx>kpd.cx) x=kpd.cx-pkeySel->cx;
                        if(y+pkeySel->cy>kpd.cy) y=kpd.cy-pkeySel->cy;
                        if(x+m_or_x<0) x = 0;
                        if(y+m_or_y<0) y = 0;

                        m_rubber->move( x, y );
                        //m_rubber->setGeometry(QRect((QPoint(x, y)), QSize(pkey[m_idx].cx, pkey[m_idx].cy)));
                    }
                    */
                }
                else    // greed keys
                {
                    setCursor(Qt::SizeAllCursor);

                    x = (pkeySel->x - m_drag_x) + ev->x();
                    y = (pkeySel->y - m_drag_y) + ev->y();

                    // protect key in window limits
                    if(x+pkeySel->cx>kpd.cx) x=kpd.cx-pkeySel->cx;
                    if(y+pkeySel->cy>kpd.cy) y=kpd.cy-pkeySel->cy;
                    if(x<0) x = 0;
                    if(y<0) y = 0;

                    m_rubber->setGeometry(QRect((QPoint(x, y)), QSize(pkeySel->cx, pkeySel->cy)));
                }
            }
        }
    }
    else
    {
        if(m_idx>=0)		// out of key: the MAP
        {
            if(kpd.type==0) // free keys
            {
                switch(m_rctPos=onRectPosition(ev->pos(), pkeySel->x, pkeySel->y, pkeySel->cx, pkeySel->cy, 10))
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
            else
            {
                switch(m_rctPos=onRectPosition(ev->pos(), pkeySel->x, pkeySel->y, pkeySel->cx, pkeySel->cy, 10))
                {
                case 'L':
                case 'R':
                case 'T':
                case 'B':
                case 'I':
                    setCursor(Qt::PointingHandCursor);
                    break;
                default:
                    setCursor(Qt::ArrowCursor);
                }
            }
        }
    }
}


int BuildKeypad::onRectPosition(const QPoint &pos, int rx, int ry, int rcx, int rcy, int borderWidth)
{
    int x1 = pos.x() - rx;
    int y1 = pos.y() - ry;
    int x2 = pos.x() - (rx + rcx);
    int y2 = pos.y() - (ry + rcy);

    if(x1<0 || y1<0 || x2>0 || y2>0)    return 'O'; // onOutside
    if(x1< borderWidth)                 return 'L'; // onLeft
    if(y1< borderWidth)                 return 'T'; // onTop
    if(x2>-borderWidth)                 return 'R'; // onRight
    if(y2>-borderWidth)                 return 'B'; // onBottom
    //if(x1>0 && y1>0 && x2<0 && y2<0)  return 'I'; // onInside

    return 'I'; // onInside
}


void BuildKeypad::FocusOnKey(int idx)
{
    if(idx<0)
    {
        m_rubber->hide();
    }
    else
    {
        m_rubber->setGeometry(QRect((QPoint(pkey[idx].x, pkey[idx].y)), QSize(pkey[idx].cx, pkey[idx].cy)));
        m_rubber->show();
    }
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


void BuildKeypad::BtnOptions(int idx, int x, int y)
{
    //QPoint p( (idx%kpd.col)*m_szCol, (idx/kpd.col)*m_szRow+m_szRow );
    QPoint p( x, y );
    p=mapToGlobal(p);

    /////////////////////////////// select by action return
    QMenu  Menu( this );
    Menu.addAction("Copy")->setData(1);                     //"Αντιγραφή"
    Menu.addSeparator();
    Menu.addAction("Paste")->setData(2);                    //"Επικόλληση"
    Menu.addAction("Paste Special")->setData(3);
    Menu.addAction("Paste Button Design")->setData(4);
    Menu.addAction("Exchange with Copy Button")->setData(5);//"Εναλλαγή θέσης με το προς Αντιγραφή"
    Menu.addSeparator();
    Menu.addAction("Remove Button")->setData(6);            //"Διαγραφή θέσης"
    Menu.addSeparator();
    Menu.addAction("Keys IDs counting from 1st key ID")->setData(7);
    if((kpd.type==0) && idx>0)                              // free keys move
        Menu.addAction("Move to 1st idx position")->setData(12);
    if((kpd.type==0) && idx<(kpd.nKeys-1))                  // free keys move
        Menu.addAction("Move to Lst idx position")->setData(13);
    Menu.addSeparator();
    Menu.addAction("Button = Selection")->setData(8);
    Menu.addAction("Button = Keypad Changing")->setData(9);
    Menu.addSeparator();
    Menu.addAction("Set Icon")->setData(10);
    //Menu.addAction("Set Product")->setData(11);

    QAction *act= Menu.exec(p);

    if(act)
    {
    switch(act->data().toInt())
    {
    case 1:
        {
            copy_idx = idx;
            memcpy(&key, &pkey[idx], sizeof(KEY));
            return;
        }
    case 2:	if(copy_idx>=0)
        {
            copyData(&key, &pkey[idx]);
            //pkey[idx].cx=key.cx;
            //pkey[idx].cy=key.cy;
            //memcpy(&pkey[idx], &key, sizeof(KEY));
        }
        break;

    case 3:	if(copy_idx>=0)
        {
            DlgKeyPaste dlg(1, kpd.nKeys, &kpd, pkey, copy_idx, this);

            KEY keyS; // for special paste on that only color etc selected key attr
            memcpy(&keyS, &pkey[idx], sizeof(KEY));

            if(dlg.exec())
            {
                for(int i=dlg.m_Fr-1; i<dlg.m_To; i++)
                {
                    if(dlg.m_paste_flags&0x001 && pkey[i].color[0]!=keyS.color[0]) continue;
                    if(dlg.m_paste_flags&0x002 && pkey[i].color[1]!=keyS.color[1]) continue;
                    if(dlg.m_paste_flags&0x004 && (pkey[i].color[2]&0x00ffffff)!=(keyS.color[2]&0x00ffffff)) continue;
                    if(dlg.m_paste_flags&0x020 && pkey[i].btn!=keyS.btn) continue;
                    if(dlg.m_paste_flags&0x040 && pkey[i].nfnt!=keyS.nfnt) continue;
                    /////////////////////////////////////////////////////

                    copy_fields(&key, &pkey[i], dlg.keydata->model->m_cflags);
                }

                break;
            }
        }
        return;

    case 4:	if(copy_idx>=0)
        {
            // btn + color[1] + kto->color[2] + attr1 + attr2 + attr3 + attr4
            copy_fields(&key, &pkey[idx], 0x000100|0x000400|0x000800|0x008000|0x010000|0x020000|0x040000);
        }
        break;

    case 5:	if(copy_idx>=0)
        {
            flipData(copy_idx, idx);
        }
        break;

    case 6:
        {        
            if(kpd.type==0)
            {
                removeArrayKey(idx);
            }
            else
            {
                int cx=pkey[idx].cx;
                int cy=pkey[idx].cy;
                int x=pkey[idx].x;
                int y=pkey[idx].y;
                memset(&pkey[idx], 0, sizeof(KEY));
                pkey[idx].cx=cx;
                pkey[idx].cy=cy;
                pkey[idx].x=x;
                pkey[idx].y=y;
            }
            break;
        }
    case 7:
        {
            for(int i=1; i<kpd.nKeys; i++) pkey[i].cmd = pkey[0].cmd+i;
            return;
        }
    case 8:
        {
            pkey[idx].cmd = 0;        // id
            pkey[idx].oper = 0x1;     // message: event type + mouse button down
            return;
        }
    case 9:
        {
            QString filepath = QFileDialog::getOpenFileName(this,
                                    "Set Button Change KeyPad",
                                    m_FilePath,
                                    "KeyPad (*.kpd)",
                                    0,	//&selectedFilter,
                                    0);	//options);

            if (!filepath.isEmpty())
            {
                qstrncpy(pkey[idx].data, filepath.section('/', -1).toUtf8().constData(), sizeof(KEY::data));
                pkey[idx].cmd = 0;      // id
                pkey[idx].oper = 0x18;  // message: change keypad + mouse button up
            }
            return;
        }
    case 10:
        {
            QString filepath = QFileDialog::getOpenFileName(this,
                                    "Set an Image on Button",
                                    m_FilePath,
                                    "Images (*.png *.jpg)",
                                    0,	//&selectedFilter,
                                    0);	//options);

            if (!filepath.isEmpty())
            {
                filepath = filepath.section('/', -1);
                filepath.chop(4); // keep the name: "icon.png" -> "icon"
                if(filepath.size()>=24)
                {
                    QMessageBox::warning(this, "Σφάλμα", "Μέγιστο μήκος ονόματος είναι 24 (αγγλικοί) χαρακτήρες !");
                    return;
                }

                qstrncpy(pkey[idx].mem, filepath.toUtf8().constData(), sizeof(KEY::mem)); // use mem+data max size
                pkey[idx].nfnt = 4;    // id for file icon
                break;
            }
            return;
        }
    //case 11:
    //    {
    //        SetProduct(idx);
    //        return;
    //    }
    case 12:
        {
            if((kpd.type==0) && idx>0) /// free keys only
            {
                moveArrayKey(idx, 0);
                /*
                KEY keyS;
                memcpy(&keyS, &pkey[idx], sizeof(KEY));
                memmove(&pkey[1], &pkey[0], idx*sizeof(KEY));
                memcpy(&pkey[0], &keyS, sizeof(KEY));
                */
                m_EditIdx = -1;
            }
            return;
        }
    case 13:
        {
            if(kpd.type==0 && idx<(kpd.nKeys-1)) /// free keys only
            {
                moveArrayKey(idx, kpd.nKeys-1);
                /*
                KEY keyS;
                memcpy(&keyS, &pkey[idx], sizeof(KEY));
                memmove(&pkey[idx], &pkey[idx+1], ((kpd.nKeys-1)-idx)*sizeof(KEY) );
                memcpy(&pkey[kpd.nKeys-1], &keyS, sizeof(KEY));
                */
                m_EditIdx = -1;
            }
            return;
        }
    default:
        {
            return;
        }
    }
    }

    emit kpd_update(2);
}


void BuildKeypad::BtnOptions2(QPoint p)
{
    if(kpd.type!=0) return;

    QMenu  Menu( this );
    Menu.addAction("Add New Button")->setData(1);
    QAction *act = Menu.exec(p);

    if(act)
    {
        if(act->data().toInt()==1)
        {
            QPoint p2 = mapFromGlobal(p);
            AddKey(p2.x(), p2.y());
        }
    }
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


void BuildKeypad::mouseDoubleClickEvent(QMouseEvent *ev)
{
    ////////////////// MOUSE ON KEY
    m_idx = idx_from_pos(ev->pos().x(), ev->pos().y());

    //////////////////
    if(m_idx>=0)    // on key
    {
        EditText(m_idx);
    }
    else
    {
        if(m_pEditor)
        {
            m_pEditor->deleteLater();
            m_pEditor = NULL;
        }
    }
}


void BuildKeypad::EditText(int idx)
{
    if(m_pEditor)  EditClosed();  // 1 click close&reopen

    m_pEditor = new QLineEdit(this);
    m_pEditor->setGeometry(pkey[idx].x, pkey[idx].y, pkey[idx].cx, 30);

    // set CHAR for NewLine: '^'= \n = 0xA
    QString text = pkey[idx].text;
    text.replace(0xA, '^'); // do it visible
    m_pEditor->setText( text );

    m_pEditor->setFont(m_fnt[pkey[idx].nfnt&0x3]);
    m_pEditor->setFocus();
    m_pEditor->show();

    connect(m_pEditor, &QLineEdit::editingFinished, this, &BuildKeypad::EditClosed);// Enter/Retern/FocusOut

    m_EditIdx = idx;
}


void BuildKeypad::EditClosed()
{
    if(m_pEditor->isModified() && m_EditIdx>=0)
    {
        //qstrncpy(pkey[m_EditIdx].text, m_pEditor->text().replace('^', 0xA).toUtf8().constData(), sizeLebel(pkey[m_EditIdx]));
        qstrcpy(pkey[m_EditIdx].text, m_pEditor->text().replace('^', 0xA).toUtf8().constData());
        emit kpd_update(2);
    }

    if(m_pEditor) m_pEditor->deleteLater();
    m_pEditor = NULL;
    m_EditIdx=-1;
}

/*
void BuildKeypad::SetProduct(int idx)
{
    char SelectedName[200]="-";
    quint32 ProductID = ProductsMenu(SelectedName, 1, 1);

    if(ProductID)
    {
        //pkey[idx].cmd= ProductID;

        pkey[idx].cmd= -21;	//	KEY_PID = -21	(LONG WITH PRODUCT_ID in DB)
        //pkey[idx].lp = ProductID;
        //pkey[idx].btn = 3;
        //pkey[idx].nfnt= 0;
        strncpy(pkey[idx].data, TXT("%d",ProductID), sizeof(KEY::data));
        strncpy(pkey[idx].text, SelectedName, sizeof(KEY::text));

        Redraw2();
    }
}
*/

void BuildKeypad::flipPosition(KEY *key1, KEY *key2) /// flip 'idx1' with 'idx2' place
{
    int cx=key2->cx;
    int cy=key2->cy;
    int x=key2->x;
    int y=key2->y;
    key2->cx=key1->cx;
    key2->cy=key1->cy;
    key2->x=key1->x;
    key2->y=key1->y;
    key1->cx=cx;
    key1->cy=cy;
    key1->x=x;
    key1->y=y;
}


void BuildKeypad::moveArrayKey(int idx1, int idx2) /// move 'idx1' key to 'idx2' place
{
    if(idx2!=idx1 && idx2>=0)
    {
        KEY key1;
        memcpy(&key1, &pkey[idx1], sizeof(KEY));

        memmove(&pkey[idx1], &pkey[idx1+1], ((kpd.nKeys-1)-idx1)*sizeof(KEY) ); /// close hole of idx1
        memmove(&pkey[idx2+1], &pkey[idx2], ((kpd.nKeys-1)-idx2)*sizeof(KEY) ); /// open hole before idx2 for idx1

        memcpy(&pkey[idx2], &key1, sizeof(KEY)); /// copy in the opened hole of idx2
    }
}

/*
void BuildKeypad::insertArrayKey(int idx, KEY *key) /// insert a new 'key' in 'idx' place
{
        kpd.nKeys = KeysAllocate(kpd.nKeys+1);
        memmove(&pkey[idx+1], &pkey[idx], ((kpd.nKeys-1)-idx)*sizeof(KEY) ); /// open hole before idx for the new key
        memcpy(&pkey[idx], &key, , sizeof(KEY)); /// copy in the opened hole the new key
}
*/

void BuildKeypad::removeArrayKey(int idx) /// remove this 'idx' key from memory
{
    memmove(&pkey[idx], &pkey[idx+1], ((kpd.nKeys-1)-idx)*sizeof(KEY) );
    kpd.nKeys = KeysAllocate(kpd.nKeys-1);
}


void BuildKeypad::flipData(int idx1, int idx2)
{
    if(idx2!=idx1 && idx2>=0)
    {
        KEY k;
        //////////////////////////////////////  EXCHANGE BUTTONS DATA
        memcpy(&k, &pkey[idx2], sizeof(KEY));
        copyData(&pkey[idx1],&pkey[idx2]); ///  copy data keeping the position of key2
        copyData(&k,         &pkey[idx1]); ///  copy data keeping the position of key1
    }
}


void BuildKeypad::copyData(KEY *key1, KEY *key2) /// copy data keeping the position of key2
{
    /////////////////////////////////// COPY KEY DATA
    int cx=key2->cx;
    int cy=key2->cy;
    int x=key2->x;
    int y=key2->y;
    memcpy(key2, key1, sizeof(KEY));
    key2->cx=cx;
    key2->cy=cy;
    key2->x=x;
    key2->y=y;
}


void BuildKeypad::AddKey(int x, int y)
{
    kpd.nKeys = KeysAllocate(kpd.nKeys+1);

    m_idx = kpd.nKeys-1;

    memset(&pkey[m_idx], 0, sizeof(KEY));
    strcpy(pkey[m_idx].text, "new");

    pkey[m_idx].x = x;
    pkey[m_idx].y = y;
    pkey[m_idx].cx = 60;
    pkey[m_idx].cy = 60;

    pkey[m_idx].btn = 2;        // "3d Rect"
    pkey[m_idx].cmd = 0;        // id
    pkey[m_idx].oper = 1;       // message: event type + mouse button down
    pkey[m_idx].color[0] = 0xffffff;
    pkey[m_idx].color[1] = 0x888888;
    pkey[m_idx].color[2] = 0xffffff;

    Redraw();
}


void BuildKeypad::AddKey(int x, int y, KEY *keystyle)
{
    kpd.nKeys = KeysAllocate(kpd.nKeys+1);

    m_idx = kpd.nKeys-1;

    memcpy(&pkey[m_idx], keystyle, sizeof(KEY));
    //strcpy(pkey[m_idx].text, "new");

    pkey[m_idx].x = x;
    pkey[m_idx].y = y;
    pkey[m_idx].cx = 60;
    pkey[m_idx].cy = 60;

    switch((pkey[m_idx].btn))
    {
    //case 0x5:	//DrawButton2Y( x, y, cx, cy, idx);	break;
    case 0x6:	//DrawButtonAx( x, y, cx, cy, idx);	break;
        correctKeyGradient(keystyle, pkey+m_idx);
    }

    emit kpd_update(0);
    Redraw();
}


void BuildKeypad::ChangeKey(int idx, KEY *keystyle)
{    
    if(keystyle->mem[0]>' ')    // copy picture only
    {
        strcpy(pkey[idx].mem, keystyle->mem); // dont use strncpy for letting free the last byte for (free zoom keypad flags)
        if(!(pkey[idx].nfnt&0xc)) pkey[idx].nfnt = keystyle->nfnt;
    }
    else                        // copy button only
    {
        // btn + color[0] + color[1] + kto->color[2] + attr1 + attr2 + attr3 + attr4
        copy_fields(keystyle, &pkey[idx], 0x000100|0x000200|0x000400|0x000800|0x008000|0x010000|0x020000|0x040000);

        switch((pkey[idx].btn))
        {
        //case 0x5:	//DrawButtonAm()
        case 0x6:	//DrawButtonAv()
        case 0x9:	//DrawButtonAr()
            correctKeyGradient(keystyle, pkey+idx);
        }
    }

    emit kpd_update(0);
    Redraw2();
}


void BuildKeypad::correctKeyGradient(KEY *keystyle_from, KEY *keystyle_to)
{
    //float newratio_cx = (float)keystyle_to->cx / (float)keystyle_from->cx;
    float newratio_cy = (float)keystyle_to->cy / (float)keystyle_from->cy;

    // degration value of each color (ratio keeps the initial button design)
    quint32 db = (keystyle_from->color[2]&0xff);
    quint32 dg = (keystyle_from->color[2]&0xff00)>>8;
    quint32 dr = (keystyle_from->color[2]&0xff0000)>>16;

    switch((keystyle_from->btn))
    {
//  case 0x5://DrawButtonAm() //DrawButtonAm corrects by it's self the design according the size
//  case 0x9://DrawButtonAr() //DrawButtonAr maybe doesn't need correction
    case 0x6://DrawButtonAv()
        {
            db = (quint32)((float)db/newratio_cy)&0xff;
            dg = (quint32)((float)dg/newratio_cy)&0xff;
            dr = (quint32)((float)dr/newratio_cy)&0xff;
            keystyle_to->color[2] = db | (dg<<8) | (dr<<16);
        }
    }
}


void BuildKeypad::EmptyKeyPad(int size_x, int size_y)
{
    memset(&kpd, 0, sizeof(KEYHDR));
    KeysAllocate(0);

    m_idx = -1;
    kpd.cx = m_wnd_cx = size_x;
    kpd.cy = m_wnd_cy = size_y;
    m_wnd_x = 0;
    m_wnd_y = 0;

    m_FileName = "New.kbd";

    ResizeKeyPad();
}


//////////////////////////////////////// DRAG & DROP


void BuildKeypad::dropEvent(QDropEvent *ev)
{
    if (ev->mimeData()->hasFormat("keypad/insert-key"))
    {
        QByteArray itemData = ev->mimeData()->data("keypad/insert-key");

        KEY* newkey = (KEY*)itemData.constData();
        int x = ev->pos().x();
        int y = ev->pos().y();
        m_idx = idx_from_pos(x, y);
        x -= 30; // 60/2 // (newkey->cx/2);
        y -= 30; // 60/2 // (newkey->cy/2);

        if(kpd.type==0 && m_idx<0)
            AddKey(x, y, newkey);       // for free keys out of key do add
        else if(m_idx>=0)
            ChangeKey(m_idx, newkey);   // for grid keys or free in key do change

        //if (event->source() == this)
        //{
        //    event->setDropAction(Qt::MoveAction); // returns the drag.exec with Qt::MoveAction
        //    event->accept();
        //}
        //else
        //{
        //    event->acceptProposedAction();
        //}
    }
    else
    {
        ev->ignore();
    }
}


void BuildKeypad::dragEnterEvent(QDragEnterEvent *ev)
{
    ev->acceptProposedAction();
}


/*
#include "dragwidget.h"
#include "draglabel.h"


void BuildKeypad::NewDragKey(int x, int y)
{
    DragWidget *wordLabel = new DragWidget(this);
    wordLabel->setGeometry(x, y, 50, 40);
    //wordLabel->move(x, y);
    wordLabel->show();
    wordLabel->setAttribute(Qt::WA_DeleteOnClose);
}
*/

/*
void BuildKeypad::CreateDragWidget(int idx)
{
    if(pkey[idx].oper==6) return;	// hiden button

    ///////////////////// buildkeypad version button position
    qint16 x, y, cx, cy;
    ButtonPosition(idx, x, y, cx, cy);
    ///////////////////// button position
    //int x = pkey[idx].x;
    //int y = pkey[idx].y;
    //int cx = pkey[idx].cx;
    //int cy = pkey[idx].cy;
    /////////////////////

    DragLabel *wordLabel = new DragLabel(pkey[idx].text, this);
    wordLabel->setGeometry(x, y, cx, cy);
    //wordLabel->move(x, y);
    wordLabel->show();
    wordLabel->setAttribute(Qt::WA_DeleteOnClose);
}
*/


/*
void BuildKeypad::NewDragKey(int x, int y)
{
    DragLabel *wordLabel = new DragLabel("New Key", this);
    wordLabel->setGeometry(x, y, 50, 30);
    //wordLabel->move(x, y);
    wordLabel->show();
    wordLabel->setAttribute(Qt::WA_DeleteOnClose);
}


void BuildKeypad::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}


void BuildKeypad::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}


void BuildKeypad::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
        const QMimeData *mime = event->mimeData();

        QByteArray itemData = mime->data("application/x-fridgemagnet");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString text;
        QPoint offset;
        dataStream >> text >> offset;

        DragLabel *newLabel = new DragLabel(text, this);
        newLabel->move(event->pos() - offset);
        newLabel->show();
        newLabel->setAttribute(Qt::WA_DeleteOnClose);

        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else if (event->mimeData()->hasText())
    {
        QStringList pieces = event->mimeData()->text().split(QRegExp("\\s+"),
                             QString::SkipEmptyParts);
        QPoint position = event->pos();

        foreach (QString piece, pieces) {
            DragLabel *newLabel = new DragLabel(piece, this);
            newLabel->move(position);
            newLabel->show();
            newLabel->setAttribute(Qt::WA_DeleteOnClose);

            position += QPoint(newLabel->width(), 0);
        }

        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}


void BuildKeypad::mousePressEvent(QMouseEvent *event)
{
    if((event->buttons()&Qt::RightButton))
    {
        BtnOptions(m_idx, event->x(), event->y());
        return;
    }

    //////////////////////////////////////////
    DragLabel *child = static_cast<DragLabel*>(childAt(event->pos()));
    if (!child)
        return;

    QPoint hotSpot = event->pos() - child->pos();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << child->labelText() << QPoint(hotSpot);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-fridgemagnet", itemData);
    mimeData->setText(child->labelText());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);

    child->hide();

    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        child->close();
    else
        child->show();
}
*/

#include "DlgBackground.h"
#include "DlgGrid.h"
#include "keypad_help_function.h"


void BuildKeypad::setKeypadType()
{
    QMenu  Menu( this );
    Menu.addAction(pDrwM[0])->setData(0);
    Menu.addAction(pDrwM[1])->setData(1);
    Menu.addAction(pDrwM[2])->setData(2);
    Menu.addAction(pDrwM[3])->setData(3);
    QAction *act= Menu.exec(QCursor::pos());

    if (act)
    {
        int sel = act->data().toInt();

        //////////////// warning to user
        if(kpd.type!=sel && pkey)
        {
            if(QMessageBox::question(this, "Κeypad Type", "Αλλαγή τύπου από αυτό που είναι δημιουργημένο.\n\nΘέλετε να προχωρήσω στην αλλαγή?", QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
                return;
        }
        ////////////////
        kpd.type=sel;

        if(kpd.type==2) // new grid keys
        {
            CreateEmptyKeyPad();
            kpd.type=1;
        }

        if(kpd.type==3) // new free keys
        {
            CreateEmptyKeyPad();
            kpd.type=0;
        }

        if(kpd.type==1) // grid keys + grid keys size*cell
        {
            if(pkey==NULL) return;

            ///////// create a grid of buttons
            DlgKeypad dlg(this);

            if(kpd.cols==0) kpd.cols=4;
            if(kpd.rows==0) kpd.rows=4;

            dlg.m_col =kpd.cols;
            dlg.m_row =kpd.rows;
            dlg.m_hor =kpd.space_h;
            dlg.m_ver =kpd.space_v;
            dlg.m_lft= kpd.margine_lf;
            dlg.m_up = kpd.margine_up;
            dlg.m_rgt= kpd.margine_rt;
            dlg.m_dn = kpd.margine_dn;
            dlg.m_scan=kpd.subtp;

            dlg.SetDlgData();

            if(dlg.exec())
            {
                dlg.GetDlgData();

                kpd.margine_lf= dlg.m_lft;
                kpd.margine_up= dlg.m_up;
                kpd.margine_rt= dlg.m_rgt;
                kpd.margine_dn= dlg.m_dn;
                kpd.space_h = dlg.m_hor;
                kpd.space_v = dlg.m_ver;

                setGreedKeys(dlg.m_col, dlg.m_row, dlg.m_scan);
            }
        }
    }
}


void BuildKeypad::setGreedKeys(int cols, int rows, int direction)
{
    kpd.nKeys = KeysAllocate(cols*rows);
    kpd.x = 0;
    kpd.y = 0;
    kpd.cols = cols;
    kpd.rows = rows;
    kpd.subtp = direction;

    int szCol = (kpd.cx-(kpd.margine_lf+kpd.margine_rt)) / cols;
    int szRow = (kpd.cy-(kpd.margine_up+kpd.margine_dn)) / rows;

    for(int i=0; i<kpd.nKeys; i++)
    {
        pkey[i].color[2]&=0x00ffffff;   // clear upper byte data of cell*size grid

        if(pkey[i].cx==0)               // default design for the new buttons
        {
            sprintf(pkey[i].text,"%d",i+1);
            pkey[i].btn = 1;            // DrawButtonFl
            pkey[i].oper = 1;           // message: event type + mouse button down
            pkey[i].color[0] = 0xffffff;
            pkey[i].color[1] = 0x005050;
            pkey[i].attr2 = 1;
        }

        int col, row;
        idx_to_cr(i, col, row);

        pkey[i].x = (szCol*col)+kpd.margine_lf;
        pkey[i].y = (szRow*row)+kpd.margine_up;
        pkey[i].cx= szCol-kpd.space_h; // -space_hor
        pkey[i].cy= szRow-kpd.space_v; // -space_ver
    }

    ///////////////////////// auto loading products
    m_idx = -1;
    emit kpd_update(2);
}


void BuildKeypad::CreateEmptyKeyPad()
{
    m_FilePath = "";
    EmptyKeyPad(500, 600);
    setWindowTitle("New");
    show();
    setGeometry(0, 25, 500, 600);
}


void BuildKeypad::setWallpaper()
{
    QString PathName = QFileDialog::getOpenFileName(this,
                                "Open Wallpaper",
                                kpd.Wallpaper,
                                "Images (*.png *.jpg *.bmp)",
                                0,	//&selectedFilter,
                                0);	//options);

    if (!PathName.isEmpty())
    {
        PathName = PathName.section('/', -1); // QDir::separator()
        strncpy(kpd.Wallpaper, PathName.toLatin1().constData(), sizeof(kpd.Wallpaper));
        ResizeKeyPad();
        emit kpd_update(0);
    }
}


void BuildKeypad::setBkColor()
{
    DlgSetBackground dlg(&kpd, this);
    if(dlg.exec())
    {
        emit kpd_update(1);
    }
}
