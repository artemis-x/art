#ifndef KEYPAD_HELP_FUNCTION_H
#define KEYPAD_HELP_FUNCTION_H

extern const char *pDrwM[];
extern const char *pWllM[];
extern const char *pMsgK[];
extern const char *pMsg2[];
extern const char *pDgnK[];
extern const char *pBtIc[];
extern const char *pStBt[];

#include "KeyPad.h"

int sizeLebel(KEY &ky);
void copy_field(KEY *kfr, KEY *kto, int column);
void copy_fields(KEY *kfr, KEY *kto, quint32 flags);
void insert_key(int idxfr, int idxto, KEYHDR *hdr, KEY *keys); /// moves 'idx1' above 'idx2'
bool setColor(quint32 &clr);
bool setInt32(int &val);

bool setOperation(KEY &ky, QWidget *parent);
bool setDesignType(KEY &ky, QWidget *parent);
bool setOnOffGroup(KEY &ky, QWidget *parent);
bool setFontK(KEY &ky, KEYHDR &hdr, QWidget *parent);
bool setTxtAlign(KEY &ky, QWidget *parent);
bool setIcon(KEY &ky, QWidget *parent);
bool setAttr1(KEY &ky, QWidget *parent);
bool setAttr2(KEY &ky, QWidget *parent);
bool setBtPos(KEY &ky, KEYHDR &hdr, QWidget *parent);

#endif // KEYPAD_HELP_FUNCTION_H
