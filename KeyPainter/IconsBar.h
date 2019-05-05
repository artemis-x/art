#ifndef ICONSBAR_H
#define ICONSBAR_H

#include <QWidget>
#include <QtWidgets>
#include <QRubberBand>
#include <QPoint>
#include "KeyPad.h"


class IconsBar : public KeyPad
{
    Q_OBJECT

public:
    explicit IconsBar(QWidget *parent = 0);
    //~IconsBar();

    int m_nKeys;
    int m_maxPic;
    int m_fromPic;
    int m_toPic;
    QStringList m_files;

    int	m_drag_x;
    int	m_drag_y;
    //QRubberBand *m_rubber;

    void IniIcons(const QString &DataPath);
    void LoadNextIcons();
    bool SetIconBar(const QByteArray &FileName, const QString &IconsPath);

    void FocusOnKey(int idx);

private:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

protected:
    //void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    //void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    //void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
};


#endif // ICONSBAR_H
