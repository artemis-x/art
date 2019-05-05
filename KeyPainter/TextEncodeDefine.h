#ifndef TEXTENCODEDEFINE_H
#define TEXTENCODEDEFINE_H

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
              Classes Text Encoding
-----------------------------------------------------
1) The files *.cpp and *.h are UTF-8 encoded.

2) ArtButton::drawText(,,,,, m_Title);  >>  in = UTF-8

3) ArtMenu::Add(ItemTxt);               >>  in = UTF-8
   ArtMenu::m_Title;                    >>  in = UTF-8

5) TransactionData::SetText(Name)       >>  in = Local8bit

6) LoadDBArrayS(out=GEN_STRUCT)         >>  out = UTF-8
   LoadDBArrayV                         >>  out = UTF-8 (εαν χρειαστεί και local μπορεί να προστεθεί εκτώς του 's' και το 't')
   LoadArray(out=GEN_STRUCT)            >>  out = UTF-8  in = file: UTF-8 or Local8bit (reads both formats)
   GetDBFieldsV                         >>  out = Local8bit
   GetDBFieldS                          >>  out = Local8bit
   MenuDBLoad(in=Title,out=SelName)     >>  in  = UTF-8  out=Local8bit

 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//Transaction: encoding to *char pointers
//---------------------------------------
//convert from QString to ... *char pointer
//#define tr_enc(qs)    qs.toUtf8().constData()
#define tr_enc(qs)    qs.toLocal8Bit().constData()
//convert from *char pointer to ... *char pointer
//#define tr_enc_p(pn)  QString::fromLocal8Bit(pn).toUtf8().constData()
#define tr_enc_p(pn)  QString::fromUtf8(pn).toLocal8Bit().constData()
//#define tr_enc_p(pn)  pn   // copy whatever it is char*

#define tr_dec_p(pn)  QString::fromLocal8Bit(pn).toUtf8().constData()
// Notice: The ListLines have to convert the *char properly to UTF-8
// according with what we have choosen here for the *char pointer
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//ListLines: decoding from *char pointers to utf-8
//------------------------------------------------
//convert from *char pointer to ... utf-8 for drawtext
#define lst_utf(pn)  QString::fromLocal8Bit(pn)
//#define lst_utf(pn)  pn   // char* is already acceptable utf-8

//BtnMenu
//#define menu_utf(pn)  QString::fromLocal8Bit(pn)
#define menu_utf(pn)  pn   // char* is already acceptable utf-8

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


#define tutf(ss) ss.toUtf8().constData()
#define tlcl(ss) ss.toLocal8Bit().constData()
#define flcl(ss) QString::fromLocal8Bit(ss)
#define flcl_tutf(ss) QString::fromLocal8Bit(ss).toUtf8().constData()
#define futf_tlcl(ss) QString::fromUtf8(ss).toLocal8Bit().constData()


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


#endif // TEXTENCODEDEFINE_H
