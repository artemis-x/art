#ifndef KEYPAD_H
#define KEYPAD_H

#include "qpainter.h"
#include <QWidget>
#include <QFont>
//#include <QMap>
#include <QPixmap>

/* * * * * * * * * * * * * */
//#define KEYPAD_DELETE	4998468	// "DEL"=4998468
//#define KEYPAD_HIDE	4475208	// "HID"=4475208
#define KEYPAD_DO(op)       (op&0x07)
#define KEYPAD_STAY(op)     !(op&0xf0)
#define KEYPAD_CHANGE(op)   (op&0xf0)
#define KEYPAD_ALTER(op)    ((op&0xf0)==0x10)
#define KEYPAD_DELETE(op)   ((op&0xf0)==0x20)
#define KEYPAD_HIDE(op)     ((op&0xf0)==0x30)

#define KEYPAD_FREE 0		// SET BY x/y
#define KEYPAD_UP 0x01		// UPPER SIDE
#define KEYPAD_DN 0x02		// DOWN SIDE
#define KEYPAD_LT 0x04		// LEFT SIDE
#define KEYPAD_RT 0x08		// RIGHT SIDE
#define KPDKPD_CX 0x10		// Set kpd cx from window size
#define KPDKPD_CY 0x20		//
#define KPDWND_CX 0x40		// Set window cx from keypad size
#define KPDWND_CY 0x80		//

#define KEY_HIDE 0x80       // for KeyPadX::setVisible(idx, show)
/* * * * * * * * * * * * * */

struct	FONTSET2		// structure size = 40
{
  qint16  Size;         // Font Size
  qint16  Weight;       // Bold 25-87. Normal=50
  quint16 attr;         // Italic=0x01 Underline=0x02
  qint16  Stretch;      // Font Size Width. The stretch factor is 1 to 4000. Normal=100.
  qint16  LetterSpacing;// Letters spacing. The spacing between the letters can be made smaller or larger. Normal=100
  qint16  Orientation;
  char  Name[28];
};


/* * * * * * * * * * * * * *
----------------------------
KEYS STRUCT Names & Values
----------------------------
char text[80]       "Label"         = last_15_char:call-function-name
char mem[25]        "More"          = drawbmp_picture, keyboard_attr
char data[15]       "Data"          = emited_by_kpd, keypad_filename, product_id
qint32 cmd          "event id"
qint32 val

qint16 x            "place x"
qint16 y            "place y"
qint16 cx           "size cx"
qint16 cy           "size cy"

quint8 btn      "Design Type"	= "Empty Button","Fill Rect Button","3D Rect Button","Round Rect Button","Eliptic Button","Midle Button Degrade","Down Degrade Button"
quint8 oper     "Event Type"	= (bits) 00000111 message, 00001000 ev_updn, 11110000 k_state
                        message = {"no message","send event message","call function()","call function(cmd,val,data)"}
                        ev_updn = {"mouse down", "mouse up"}
                        k_state = {"stay","keypad change","delete","hide"}
quint8 onoff    "Button State"	= "push button","switch on/off","group 1 radio","group 2 radio","group 3 radio","group 4 radio","group 5 radio","group 6 radio","group A to all keypads","group B to all keypads","group C to all keypads","group D to all keypads","group E to all keypads","group F to all keypads"
quint8 nfnt     "Font Index"	= (bits) 00000011 Font, 00001100 Icon, 11110000 Text_Align

qint8 attr1         DrawButtonGr(), DrawButtonMd(), DrawButtonAr()
qint8 attr2         DrawButtonGr(), DrawButtonAr(), DrawButtonFl()
qint8 attr3
qint8 attr4
quint32 color[0]	"Color Text"
quint32 color[1]	"Color Fill"
quint32 color[2]	"Color Out", Upper-byte-0xff000000: Flags for free-zoom-keypad buttons position

----------------------------
onoff:	"Button State"
----------------------------
"push button"  "switch on/off"  "group 1 radio"  "group 2 radio"  "group 3 radio"  "group 4 radio"  "group 5 radio"  "group 6 radio"  "group A to all keypads"  "group B to all keypads"  "group C to all keypads"  "group D to all keypads"  "group E to all keypads"  "group F to all keypads"
----------------------------
    0000 = push button
    0001 = switch on/off
    0111 = local groups of radio buttons
    1111 = global groups of radio buttons for all keypads
----------------------------
* * * * * * * * * * * * * *
----------------------------
Header
---------------
position  = keypad position & size (x/y/cx/cy) on screen (flags)
space_h/v = for greed it is space_hor & space_ver between buttons

---------------
QLinearGradient
---------------
attr[0] = x1
attr[1] = y1
attr[2] = x2
attr[3] = y2
attr[5]10 = 0.0-1.0 (0-100) setColor position
attr[6]11 = 0.0-1.0 (0-100) setColor position
attr[7]12 = 0.0-1.0 (0-100) setColor position
---------------
QRadialGradient
---------------
attr[0] = x center
attr[1] = y center
attr[2] = x focus point
attr[3] = y focus point
attr[4] = radial
attr[5] = 0.0-1.0 (0-100) setColor position
attr[6] = 0.0-1.0 (0-100) setColor position
attr[7] = 0.0-1.0 (0-100) setColor position
* * * * * * * * * * * * * */

//V6 (156btes) (biger is better for utf8 text and pic file names size)
struct KEY{char text[80]; char mem[25]; char data[15]; qint32 cmd; qint32 val; qint16 x; qint16 y; qint16 cx; qint16 cy; quint8 btn; quint8 oper; quint8 onoff; quint8 nfnt; qint8 attr1; qint8 attr2; qint8 attr3; qint8 attr4; quint32 color[3];};
struct KEYHDR{quint8 type; quint8 subtp; quint8 cols; quint8 rows; qint8 margine_lf; qint8 margine_up; qint8 margine_rt; qint8 margine_dn; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint32 free32; FONTSET2 font[4]; quint8 bckgrType; quint8 nKeys; qint16 attr[8]; quint8 position; quint8 free8; qint8 space_h; qint8 space_v; qint32 data; char Wallpaper[40];};

//V5 (free key map) (has a shorter header:  25 keys 5x5 file size V5=2560  v4=3120)
//struct KEY{char text[30]; char mem[15]; char data[15]; qint16 lp; qint8 attr1; qint8 attr2; qint8 attr3; qint8 attr4; qint16 cmd; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint8 btn; quint8 nfnt; quint8 onoff; quint8 oper;};
//struct KEYHDR{quint8 type; quint8 subtp; quint8 cols; quint8 rows; qint8 margine_lf; qint8 margine_up; qint8 margine_rt; qint8 margine_dn; qint16 orX; qint16 orY; qint16 cx; qint16 cy; quint32 color[4]; FONTSET2 font[4]; quint8 bckgrType; quint8 nKeys; qint16 attr[10]; qint32 data; char Wallpaper[40];};

//V4 (new font structure from 60 to 44 bytes) (key cmd from qint32 to qint16, making 2 new qint8 attr3&attr4) (key fonts from 2 to 4 optionts) (2 new pad backgrounds)
//struct KEY{char text[30]; char mem[15]; char data[15]; qint16 lp; qint8 attr1; qint8 attr2; qint8 attr3; qint8 attr4; qint16 cmd; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint8 btn; quint8 nfnt; quint8 onoff; quint8 oper;};
//struct KEYHDR{quint8 type; quint8 subtp; quint8 row; quint8 col; qint8 offlf; qint8 offrt; qint8 offup; qint8 offdn; qint16 cx; qint16 cy; quint8 spx; quint8 spy; quint32 color[4]; FONTSET font[4]; quint8 backpaint; quint8 attr1; qint16 attr[30]; qint32 msg; qint32 data; char text[40]; char BmpKeypad[40];};

//V3 (come from AirArt Navigation)
//struct KEY{char text[30]; char mem[15]; char data[15]; qint16 lp; qint8 attr1; qint8 attr2; qint32 cmd; qint16 x; qint16 y; qint16 cx; qint16 cy; quint32 color[3]; quint8 btn; quint8 nfnt; quint8 onoff; quint8 oper;};
//struct KEYPAD{quint8 type; quint8 subtp; quint8 row; quint8 col; quint8 offlf; quint8 offrt; quint8 offup; quint8 offdn; qint16 cx; qint16 cy; quint8 spx; quint8 spy; quint32 color[4]; lgf fnt[4]; qint32 msg; qint32 data; char text[40]; char BmpKeypad[40]; KEY Key[MAX_BUTTONS];};

//V2
//struct KEY{char text[30]; char mem[15]; char code[15]; LPVOID lp; long data; short x; short y; short cx; short cy; long color[3]; BYTE btn; BYTE attr; BYTE dummy; BYTE flag;};
//struct KEYPAD{BYTE type; BYTE subtp; BYTE row; BYTE col; short x1; short y1; short cx; short cy; BYTE spx; BYTE spy; long color[4]; LOGFONT lgf[4]; long msg; long data; char text[40]; char BmpKeypad[40]; KEY Key[50];};

//V1
//struct KEYS{char name[20]; char str[15]; long data; long color[3]; short cx; short cy; BYTE btn; BYTE attr; BYTE dummy1; BYTE dummy2; BYTE flag;};
//struct KEYPAD{BYTE type; BYTE row; BYTE col; int x1; int y1; int cx; int cy; int spx; int spy; long color[4]; LOGFONT lgf[4]; char BmpKeypad[40]; KEYS Key[50];};

/* * * * * * * * * * * * * */

class KeyPad : public QWidget
{
    Q_OBJECT
	
public:
    explicit KeyPad(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::Widget|Qt::FramelessWindowHint);
    ~KeyPad();

    //QObject *sendToObj;
    QPixmap *m_xmp = nullptr;
    QPainter m_pnt;
    QFont    m_fnt[4];
    QString m_FilePath;
    QByteArray m_FileName;//, m_FileNameFirst;
    //QMap<QByteArray,quint32> m_map; // < keyname, value >

    KEYHDR   kpd;
    KEY     *pkey = nullptr;
    KEY     *pkeySel;

    int     m_wnd_x = 0;	// current window position
    int     m_wnd_y = 0;	// current window position
    int     m_wnd_cx;		// current window size (from kpd->cx or window set)
    int     m_wnd_cy;		// current window size (from kpd->cx or window set)
    int     m_dsp_cx;		// display size (usualy  DISPLAY_CX)
    int     m_dsp_cy;		// display size (usualy  DISPLAY_CY)
    float   m_newratio_cx = 1;
    float   m_newratio_cy = 1;
    quint8  m_Position = KPDWND_CX | KPDWND_CY;
    int     m_idx = -1;             // selected key
    bool    m_push;
    //bool    is_switch_kpd = false;
    //quint32 m_32switchesstate = 0;  // 32 bit register for 32 keys state

    //bool isSwitchKeypad();
    //void SwitchSet(bool on, int idx);
    //void SwitchOnOff(quint8 idx);	// sets the new state and color for on/off type switch
    //void SwitchGroup(quint8 group_id, int set_on_idx);
    //void SetKeysState(quint32 switches32state);
    void buttonPush();
    void buttonRelease();
    void mouseOperation(quint8 oper);

    bool setKeypad(const QByteArray &FileName, const QString &FilePath);
    bool ReplaceKeypad(const QByteArray &FileName);
    bool LoadKeyPad(const QByteArray &FileName);
    void NewSizes(int wnd_cx, int wnd_cy);
    void setKpdGeometry(int wnd_x, int wnd_y, int wnd_cx, int wnd_cy, quint8 position);
    void setPositionOnDisplay(quint8 position);
    int KeysAllocate(int maxkeys);
    void ResizeKeyGrid(qint16 tocx, qint16 tocy);
    void ResizeKeyFree(qint32 tocx, qint32 tocy);
    void ButtonPosition(int idx, qint16 &x, qint16 &y, qint16 &cx, qint16 &cy);
    int idx_from_pos(int x, int y);
    int idx_from_id(int id);
    void idx_to_cr(int idx, int &col, int &row);

    void Redraw();
    //void RedrawKey(int idx);
    void DrawKey(KEY *drkey);
    void DrawButtonFl(int x, int y, int cx, int cy, KEY *drkey);
    void DrawButtonRc(int x, int y, int cx, int cy, KEY *drkey);
    void DrawButtonRd(int x, int y, int cx, int cy, KEY *drkey);
    void DrawButtonEl(int x, int y, int cx, int cy, KEY *drkey);
    void DrawButtonAv(int x, int y, int cx, int cy, KEY *drkey);
    void DrawButtonAr(int x, int y, int cx, int cy, KEY *drkey);
    //void DrawRainbowV(int x1, int cx, int y1, int y2, quint32 icolor, float db, float dg, float dr, int stepy);
    //void DrawRectsDgr(int x1, int cx, int y1, int cy, quint32 icolor, float db, float dg, float dr, quint8 nthick, bool stoph, float stepx, float stepy, float stepcx, float stepcy);
    void DrawButtonGr(int x, int y, int cx, int cy, KEY *drkey);
    void DrawButtonGc(int x, int y, int cx, int cy, KEY *drkey);
    void DrawBoundaries(int x, int y, int cx, int cy);
    void DrawBmp(int x, int y, int cx, int cy, quint8 scale, const char* FileName);
    void DrawTxt(int x, int y, int cx, int cy, KEY *drkey, quint32 clr);
    quint16 TxtAlign(quint8 flags);

    void paintEvent(QPaintEvent *);
    //void mousePressEvent(QMouseEvent *ev);
    //void mouseReleaseEvent(QMouseEvent *);

signals:
    void KeyPressed(qint32 idx, KEY *ikey);
};

#endif // KEYPAD_H
