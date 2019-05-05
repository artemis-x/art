#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

extern int  DISPLAY_CX;
extern int  DISPLAY_CY;

#include <QPainter>
#include <QtGlobal>

namespace draw
{
//void Frame(QPainter &p, int x, int y, int cx, int cy);
void RainbowV(QPainter &p, int x1, int cx, int y1, int y2, quint32 icolor, float db, float dg, float dr, int stepy);
void RectsDgr(QPainter &p, int x1, int cx, int y1, int cy, quint32 icolor, float db, float dg, float dr, quint8 nthick, bool stoph, float stepx, float stepy, float stepcx, float stepcy);
//void PeripheralDgr(QPainter &p, int x1, int cx, int y1, int cy, quint32 icolor, float db, float dg, float dr, quint8 nthick, quint8 flags, quint8 rndrect);
}

#endif // DRAW_FUNCTIONS_H
