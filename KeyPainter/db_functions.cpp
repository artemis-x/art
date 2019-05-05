#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>

///////////////////////////////////////////////
#define SETTINGSFILENAME    "Data/settings.cnf"

struct APPL_CNF
{
    qint32	UserID;
    char	UserName[10];
    char	key[15];
    char	DataPath[82];
    char	dbname[30];
    char	dbdriver[8];
    long	ServerPort;
    char	ServerIP[20];
//    char	VfdPort[5];
//    long	VfdSpeed;
//    quint16	Permission;     // user permissions
//    quint8	Operation;
//    quint8	NetOperation;   // handle messages
//    //float   map_lat;      // map center
//    //float   map_lon;
//    char	psw_s[10];      // settings
//    char	psw_o[10];      // office
//    char	psw_a[10];      // administrator
//    char	psw_r[10];      // remove / delete / change
//    char	psw_d[10];      // deliver
//    char	psw_w[10];      // waiter
} cnf;

char *TXT(const char *format, ...);
void LoadConfig();
/* * * * * * * * * * * * *   GENERAL DB FUNCTIONS   * * * * * * * * * * * * * * * */
QSqlDatabase db;


//#ifdef DB_MDB
bool db_open()
{
    db = QSqlDatabase::addDatabase(cnf.dbdriver, "db");
    db.setDatabaseName(TXT("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s/%s.mdb", cnf.DataPath, cnf.dbname));
    db.setHostName(cnf.ServerIP); // "127.0.0.1", "localhost"
    //db.setUserName("root");
    db.setPassword("CashArt"); // txt_decode(psw) // psw[]={26,133,230,104,160,156,142,0} // (psw = ROR_5 + shift_each_next_char_1)

    if(db.open()==false)
    {
        //QMessageBox::warning(0, cnf.dbdriver, db.lastError().text());
        return false;
    }

    return true;
}
//#endif // DB_MDB


#ifdef DB_MYSQL
    //----------------------// MySQL - MariaDB
    //       - SET -
    // path=C:\Program Files\MariaDB 10.0\lib

bool db_open()
{
    db = QSqlDatabase::addDatabase(cnf.dbdriver, "db");
    db.setDatabaseName(cnf.dbname);
    db.setHostName(cnf.ServerIP);
    db.setUserName("root");
    //db.setPassword("CashArt");

    if(db.open()==false)
    {
        QMessageBox::warning(0, cnf.dbdriver, db.lastError().text());
        return false;
    }

    return true;
}
#endif // DB_MYSQL


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
void UpdateSQL(const char*sql)
{
    QSqlQuery query(sql,db);
}
*/
/*
void UpdFrmSQL(const char *format, ...)
{
    char sql[400];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(sql, format, arguments);
    //va_end(arguments);	// restart the pointer in case you reread the arguments
    QSqlQuery query(sql,db_bkup);
}
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  QSqlQuery::setForwardOnly(true) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *  --------------------------------------------------------------
 *  If you iterate through a result set only using next() and seek()
 *  with positive values, you can call this, before calling exec().
 *  This is an easy optimization that will speed up
 *  the query significantly when operating on large result sets.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
To determine whether a database driver supports a given feature, use QSqlDriver::hasFeature().
In the following example, we call QSqlQuery::size() to determine the size of a result set of the underlying database supports that feature;
otherwise, we navigate to the last record and use the query's position to tell us how many records there are.

    QSqlQuery query(db);
    query.exec("SELECT ...");

    if(db.driver()->hasFeature(QSqlDriver::QuerySize))
    {
         numRows = query.size();
    }
    else
    {
         query.last();
         numRows = query.at() + 1;
    }

Ούτε το query.last() εργλαζεται στο παραπάνω παράδειγμα η MsAccess.
Μόνο με query.setForwardOnly(true), αλλά τότε είναι άχρηστο γιατί δεν επιστρέφει στην αρχή.

    qr.setForwardOnly(true)
    query.exec("SELECT ...");
    qr.last()
    numRows = query.at()+1;     ! yes
    qr.first();                 ! no
    qr.previous();              ! no
    ------------------------

    MsAccess
    -------------------------------------
    Δεν Διαθέτει: QuerySize, LastInsertId
    -------------------------------------
    if(db.driver()->hasFeature(QSqlDriver::QuerySize))      qDebug("QuerySize");
    if(db.driver()->hasFeature(QSqlDriver::LastInsertId))   qDebug("LastInsertId");

    Returns the object ID of the most recent inserted row if the database supports it.
    key = qr.lastInsertId().toInt();

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


QVariant GetDBField(const char*sql)
{
    QSqlQuery query(sql, db);
    if(query.next()) return query.value(0); // else > QSqlQuery::value: not positioned on a valid record
    return QVariant(0);
}

/*
QVariant GetDBFieldF(const char *format, ...)
{
    char sql[400];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(sql, format, arguments);
    //va_end(arguments);	// restart the pointer in case you reread the arguments

    QSqlQuery query(sql, db);
    query.next();
    return query.value(0);
    //return QVariant(0);
}


bool GetDBQuery(QSqlQuery *query, const char *format, ...)
{
    char sql[400];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(sql, format, arguments);
    //va_end(arguments);	// restart the pointer in case you reread the arguments

    query->exec(sql);
    if(query->next()) return true;
    return false;
}
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *    Variable number of predefined type arguments.
 *
 *    Input:    1)SQL SELECT ...
 *              2)Type of requested field "s,t,c,i,l,f" for the next returning columns.
 *              3)Addresses for returning the columns values
 *
 *    Output:   Variable values of types and sizes.
 *              Not what the db is, but what we want to be each value
 *
 *    Warning:  SELECT columns + fieldtype size characters + arguments = should be equal number !!
 *	  Example:  GetDBFieldsV("SELECT TaxID, Name FROM TaxList", "is", &iVal, sVal);
 *
 */
/*
bool GetDBFieldsV(const char*sql, const char*fieldtype, ...)
{
    QSqlQuery qr(db);
    qr.exec(sql);

    va_list	arg;
    int nCol = qr.record().count();

    if( qr.next() )
    {
        va_start(arg, fieldtype);

        for(int c=0; c<nCol; c++)
        {
            char *s = va_arg( arg, char* );	// get each argument

            //memcpy(s, &a, sizeof(a)); // int to str
            //memcpy(&a, s, sizeof(a)); // str to int

            switch(fieldtype[c])
            {
            case 's': strcpy(s, qr.value(c).toString().toUtf8().constData()); break;
            case 't': strcpy(s, qr.value(c).toString().toLocal8Bit().constData()); break;
            case 'c': *((qint8*)s) = (qint8)qr.value(c).toInt(); break;
            case 'i': *((qint32*)s)= qr.value(c).toInt(); break;
            case 'l': *((qint64*)s)= qr.value(c).toLongLong(); break;
            case 'f': *((float*)s) = qr.value(c).toFloat();
            }
        }

        //va_end(arg);	// restart the pointer in case you reread the arguments
        return true;
    }

    return false;
}
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *    Loads an Array with variable values and number of predefined type arguments.
 *
 *    Input:    1)SQL SELECT ...
 *              2)Max size of loading array
 *              3)Size of cell/structure in array
 *              2)Type of requested field "s,t,c,i,l,f" for the next columns inside the structure.
 *              3)Addresses of each column inside the structure
 *
 *    Output:   Variable values of types and sizes.
 *              Not what the db is, but what we want to be each value
 *
 *    Advances: Επιστρέφει όπως εμείς θέλουμε το κάθε πεδίο και όχι το πως αυτό είναι στην βάση δεδομένων.
 *              Εχει την δυνατότητα να ενημερώσει συγκεκριμένες μόνο τιμές σέ μια δομή
 *              εφήνοντας ανέπαφες τις υπόλοιπες τιμές του structure.
 *
 *    Warning:  SELECT columns + fieldtype size characters + arguments = should be equal number !!
 *	  Example:  GetDBFieldsV("SELECT TaxID, Name FROM TaxList", "is", &iVal, sVal);
 *
 */
/*
int LoadDBArrayV(const char*sql, int maxRec, int recsz, const char*fieldtype, ...)
{
    QSqlQuery qr(db);
    qr.exec(sql);

    va_list	arg;
    int nCol = qr.record().count();
    int nRec = 0;

    while( qr.next() )
    {
        va_start(arg, fieldtype);

        for(int c=0; c<nCol; c++)
        {
            char *s = va_arg( arg, char* )+(nRec*recsz);	// get the array structure pointer

            //memcpy(s, &a, sizeof(a)); // int to str
            //memcpy(&a, s, sizeof(a)); // str to int

            switch(fieldtype[c])
            {
            case 's': strcpy(s, qr.value(c).toString().toUtf8().constData()); break;
            case 't': strcpy(s, qr.value(c).toString().toLocal8Bit().constData()); break;
            case 'c': *((qint8*)s) = (qint8)qr.value(c).toInt(); break;
            case 'i': *((qint32*)s)= qr.value(c).toInt(); break;
            case 'l': *((qint64*)s)= qr.value(c).toLongLong(); break;
            case 'f': *((float*)s) = qr.value(c).toFloat();
            }
        }

        va_end(arg);	// restart the pointer in case you reread the arguments
        if(++nRec>=maxRec) break;
    }

    return nRec;
}
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Input:     1)SQL SELECT ...
 *              2)Columns size in bytes to return in each field.
 *              3)Fields addresses for returning values
 *
 *   Output:    Variable number of values according the input addresses.
 *
 *   Advances:  Ταχύτητα καθώς διαβάζει απευθείας την μνήμη buffer
 *              για όλους του τύπους πεδίων της βάσης δεδομένων.
 *              Get the values and sizes you want. (max size of each field = 250)
 *              Exception is the string that is not exchangable, like
 *              int to str & str to int.
 *
 *	 Example:   GetDBFieldsS("SELECT TaxID, Name FROM TaxList", "4,80", &int, &str);
 *
 */
/*
bool GetDBFieldsS(const char*sql, const char*colsize, ...)
{
    QSqlQuery qr(db);
    qr.exec(sql);

    va_list	arg;
    int nCol = qr.record().count();

    if( qr.next() )
    {
        va_start(arg, fieldtype);

        for(int c=0; c<nCol; c++)
        {
            char *s = va_arg( arg, char* );	// get here each argument

            //storing int to string
            //memcpy(s, &a, sizeof(a)); //to
            //memcpy(&a, s, sizeof(a)); //from

            //if(qr.value(c).type()==QVariant::String)
            //    memcpy(s, qr.value(c).toString().toUtf8().constData(), colsize[c]);
            //else
            //    memcpy(s, qr.value(c).constData(), colsize[c]);

            memcpy(s, (qr.value(c).type()==QVariant::String)? qr.value(c).toString().toUtf8().constData() : qr.value(c).constData(), colsize[c]);
        }

        //va_end(arg);	// restart the pointer in case you reread the arguments
        return true;
    }

    return false;
}
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
int LoadDBArrayS(const char *sql, GEN_STRUCT *pGS, int nMaxRec)
{
    memset(pGS, 0, sizeof(GEN_STRUCT)*nMaxRec);
    int	nRec=0;

    QSqlQuery qr(db);
    qr.exec(sql);

    while( qr.next() )
    {
        (pGS+nRec)->id = qr.value(0).toUInt();

        strncpy((pGS+nRec)->Name, qr.value(1).toString().toUtf8(), 40);

        if(++nRec>=nMaxRec) break;
    }

    return nRec;
}
*/

/* * *
 *  Loads variable structure arguments.
 *  Get the actual database values. (not what you want)
 *	The advantage is I have less arguments for calling the function but the structure values have to be the same with the database field types.
 *	Example : LoadArray("SELECT TaxID, Name FROM TaxList", MAX_PRODTAX, sizeof(GEN_STRUCT), &ProdTax[0].id, ProdTax[0].Name);
 */
/*
int LoadDBArrayV(const char*sql, int nMaxRec, int recsz, ...)
{
    QSqlQuery qr(db);
    qr.exec(sql);

    va_list	arg;
    int nCol = qr.record().count();
    int nRec = 0;

    while( qr.next() )
    {
        va_start(arg, recsz);

        for(int c=0; c<nCol; c++)
        {
            char *s = va_arg( arg, char* )+(nRec*recsz);	// get the array structure pointer

            //storing int to string
            //memcpy(s, &a, sizeof(a)); //to
            //memcpy(&a, s, sizeof(a)); //from

            switch(qr.value(c).type())
            {
            case QVariant::String:	strcpy(s, qr.value(c).toString().toUtf8()); break;
            //case QVariant::Char:  *((QChar*)s) = qr.value(c).toChar(); break;
            //case QVariant::Char:  *((quint8*)s)= (quint8)qr.value(c).toChar().toLatin1();	break;
            case QVariant::Char:    *((qint8*)s) = (qint8)qr.value(c).toInt(); break;
            case QVariant::Int:     *((qint32*)s)= qr.value(c).toInt(); break;
            case QVariant::LongLong:*((qint64*)s)= qr.value(c).toLongLong(); break;
            case QVariant::Double:  *((float*)s) = qr.value(c).toFloat();
            }
        }

        va_end(arg);	// restart the pointer in case you reread the arguments

        if(++nRec>=nMaxRec) break;
    }

    return nRec;
}
*/

////////////////////////////////////
//  SQL TYPE: "SELECT name+';'+age+';'+address+';'+phone FROM Main"
//	ExportDataBase("SELECT STREET+';'+CITY FROM TEL WHERE PHONE LIKE '2821%' GROUP BY STREET, CITY");
/*
void ExportDataBase(const char *sql)
{
    QString FilePath = QFileDialog::getSaveFileName(this, "Save File", "","Export (*.csv *.txt)");
    if(FilePath.isEmpty()) return;

    QFile file(FilePath);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        int	nCols = tw->columnCount();
        int	nRows = tw->rowCount();

        QString	wrStr;

        for(int r=0; r<nRows; r++)
        {
            wrStr = "";

            for(int c=0; c<nCols; c++)
            {
                wrStr += tw->item(r, c)->text();
                wrStr += ";";
            }

            wrStr+='\n';

            file.write(wrStr.toUtf8());
        }

        file.close();

        wrStr.sprintf("Επιτυχής εξαγωγή  - %d -  εγγραφών.", nRows);
        BOXINFO(DISPLAY_CX,DISPLAY_CY,wrStr.toUtf8().data());
    }
}
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <QApplication>
#include <QMenu>
/*
 *  SQL TYPE: "SELECT Text, Data FROM Table"
 *
 */
long MenuDBLoad(const char* sql, int x, int y, int cx, int cy=50, const char* Title="", char* SelName=NULL)
{
    QMenu menu(QApplication::activeWindow());
    //menu.setFixedSize(cx,cy);
    //menu.setStyleSheet("menu-scrollable: 1"); // place this in stylesheet file
    QSqlQuery qr(sql, db);
    int	row=0;

    while( qr.next() )
    {
        menu.addAction(qr.value(0).toString())->setData(qr.value(1).toUInt());
        row++;
    }

    if(row==0) return 0;
    QAction *act = menu.exec(QPoint(x, y));
    if(act==0) return 0;

    if(SelName)	strcpy(SelName, act->text().toUtf8().constData());
    return act->data().toUInt();
}


/*
 *  SQL TYPE: "SELECT Text, Data FROM Table"
 *
 */
void LoadDBComboBox(const char*sql, QComboBox *pCombo)
{
    if(!db.isOpen())
    {
        LoadConfig();
        if(!db.isOpen()) return;
    }
    /////////////////

    pCombo->clear();
    int i = 0;

    QSqlQuery qr(db);
    qr.exec(sql);

    while( qr.next() )
    {
        pCombo->addItem(qr.value(0).toString(), QVariant( qr.value(1).toUInt() ));
        i++;
    }

    pCombo->setMaxVisibleItems(i);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
char s_str[600];


char *TXT(const char *format, ...)	// Formated Text - passing variable arguments to sprintf()
{
    va_list arguments;
    va_start(arguments, format);
    vsprintf(s_str, format, arguments);
    //va_end(arguments);	// it is only for restarting the pointer for reading again the arguments
    return s_str;
}


void LoadConfig()
{
    QFile f_cnf(SETTINGSFILENAME);
    if (!f_cnf.open(QIODevice::ReadOnly))
    {
        //QMessageBox::warning(0, "error", SETTINGSFILENAME);
        return;
    }

    APPL_CNF cnf;
    f_cnf.read((char*)&cnf, sizeof(APPL_CNF));
    f_cnf.close();

    db_open();
}


quint32 ProductsMenu(char *SelectedName, int Sel, int treelevel)
{
    if(!db.isOpen())
    {
        LoadConfig();
        if(!db.isOpen()) return 0;
    }
    /////////////////

    switch(treelevel)
    {
    case 0:  if((Sel=MenuDBLoad("SELECT Name, CategoryID FROM ProductCategory ORDER BY idx", 700, 0, 450, 50, "επιλογή...", NULL))==0) break;
    case 1:  if((Sel=MenuDBLoad(TXT("SELECT Name, SubCatID FROM ProductSubCat WHERE CategoryID=%d ORDER BY idx", Sel), 700, 0, 450, 50, "επιλογή...", NULL))==0) break;
    case 2:  return MenuDBLoad(TXT("SELECT Name, ProductID FROM Products WHERE SubCateg=%d ORDER BY Name", Sel), 700, 0, 450, 50, "επιλογή...", SelectedName);
    }

    /*
    if(Sel=MenuDBLoad("SELECT Name, CategoryID FROM ProductCategory ORDER BY idx", 700, 0, 450, 50, "επιλογή...", NULL))
    {
        if(Sel=MenuDBLoad(TXT("SELECT Name, SubCatID FROM ProductSubCat WHERE CategoryID=%d ORDER BY idx", Sel), 700, 0, 450, 50, "επιλογή...", NULL))
        {
            return MenuDBLoad(TXT("SELECT Name, ProductID FROM Products WHERE SubCateg=%d ORDER BY Name", Sel), 700, 0, 450, 50, "επιλογή...", SelectedName);
        }
    }
    */

    return 0;
}


#include "KeyPad.h"


int KeypadDBLoad(const char* sql, KEY *keys, int max)
{
    if(!db.isOpen())
    {
        LoadConfig();
        if(!db.isOpen()) return 0;
    }
    /////////////////

    QSqlQuery qr(db);
    qr.exec(sql);

    int idx=0;

    while( qr.next() )
    {
        keys[idx].cmd= -21;	//	KEY_PID = -21	(LONG WITH PRODUCT_ID in DB)
        //pkey[idx].lp = ProductID;
        strncpy(keys[idx].data, qr.value(1).toString().toLatin1().constData(), sizeof(KEY::data));
        strncpy(keys[idx].text, qr.value(0).toString().toLocal8Bit().constData(), sizeof(KEY::text));

        idx++;
        if(idx>=max) return idx;
    }

    // clear the rest keys
    for(int i=idx; i<max; i++)
    {
        keys[i].cmd = 0;
        keys[i].text[0]=0;
        keys[i].data[0]=0;
    }

    return idx;
}
