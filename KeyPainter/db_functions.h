#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QComboBox>

extern QSqlDatabase db;

bool db_open();

QVariant GetDBField(const char*sql);
//QVariant GetDBFieldF(const char *format, ...);
//bool GetDBQuery(QSqlQuery *query, const char *format, ...);
//bool GetDBFieldsV(const char*sql, const char*fieldtype, ...);

//void UpdateSQL(const char*sql);
//void UpdFrmSQL(const char *format, ...);

int MenuDBLoad(const char* sql, int x, int y, int cx, int cy, const char* Title, char* SelName);
void LoadDBComboBox(const char*sql, QComboBox *pCombo);
//int LoadDBArrayS(const char*sql, GEN_STRUCT *pGS, int nMaxRec);
//int LoadDBArrayV(const char*sql, int maxRec, int recsz, const char*fieldtype, ...);

#include "KeyPad.h"
int KeypadDBLoad(const char* sql, KEY *keys, int max);
quint32 ProductsMenu(char *SelectedName, int Sel, int treelevel);
char *TXT(const char *format, ...);

#define recs(n)  rq.value(n).toString()
#define recb(n)  rq.value(n).toChar()
#define reci(n)  rq.value(n).toInt()
#define recf(n)  rq.value(n).toFloat()

#define getdbs(sql)  GetDBField(sql).toString()
#define getdbsu(sql) GetDBField(sql).toString().toUtf8().constData()
#define getdbsl(sql) GetDBField(sql).toString().toLocal8Bit().constData()
#define getdbd(sql)  GetDBField(sql).toChar()
#define getdbi(sql)  GetDBField(sql).toInt()
#define getdbf(sql)  GetDBField(sql).toFloat()

/*
select * from A,B where A.id = B.id where A.id = 1; instead of
select * from A left outer join B on A.id = B.id where A.id = 1)

The SQLite translates first statement to second before compiling and
you could save on the small amount of CPU time by directly using the second statement
*/


/*
SQL INSERT INTO / SELECT INTO Statements
----------------------------------------
The SELECT INTO statement selects data from one table and inserts it into a new table.
If we want to copy all columns of a table into a new table, then we use ‘*’ after the SELECT statement.
----------------------------------------
INSERT INTO
----------
INSERT INTO Customers (CustomerName, Country)
 SELECT SupplierName, Country FROM Suppliers;

INSERT INTO Customers (CustomerName, Country)
 SELECT SupplierName, Country FROM Suppliers
WHERE Country='Germany';

SELECT INTO
-----------
SELECT INTO * Table 1 FROM Table 2
•The query ‘SELECT * INTO Table 1 FROM Table 2’ creates table 1 with the values from table 2.
But if Table 1 already exists then the statement will give error.

*/

/*
Copy SQLite database information from file to :memory:
------------------------------------------------------
QString myPrivateDirectory (QDesktopServices::storageLocation (QDesktopServices::DataLocation));
    QSqlQuery query(QString("ATTACH '%1' AS disk").arg(QDir::toNativeSeparators(myPrivateDirectory + "database.db")));
    query.exec();
    // create the table in the file based DB as a copy
    // of the in memory DB
    if (query.exec("CREATE TABLE table1 AS SELECT * from disk.table1") &&
        query.exec("CREATE TABLE table2 AS SELECT * from disk.table2"))
        return true;
    return false;


Saving and restoring an in-memory SQLite database
This will make your file-based database (nearly) as fast as a memory based one (of course depending on the use case).



1.QSqlQuery("PRAGMA page_size = 4096");
2.QSqlQuery("PRAGMA cache_size = 16384");
3.QSqlQuery("PRAGMA temp_store = MEMORY");

There are other pragmas that influence performance as well.

1.QSqlQuery("PRAGMA journal_mode = OFF");
2.QSqlQuery("PRAGMA locking_mode = EXCLUSIVE");
3.QSqlQuery("PRAGMA synchronous = OFF");
*/


/*
----------------------------------
INSERT INTO target [(field1[, field2[, …]])] [IN externaldatabase] SELECT [source.]field1[, field2[, …]  FROM tableexpression

http://msdn.microsoft.com/en-us/library/bb208861(v=office.12).aspx
http://msdn.microsoft.com/en-us/library/bb177907(v=office.12).aspx

----------------------------------
INSERT INTO [YourNewTable] SELECT * FROM [MS Access;DATABASE=D:\Path\To\OldDatabase.mdb].[YourOldTable]

If the structure is not the same:

INSERT INTO [YourNewTable] (field1, field2, field3) SELECT field1, field2, field3 FROM [MS Access;DATABASE=D:\Path\To\OldDatabase.mdb].[YourOldTable]
----------------------------------


-----------------------------
       cross databases
 (test in access invironment)
-----------------------------
INSERT INTO Pay2 SELECT * FROM Pay
INSERT INTO Pay2 SELECT * FROM [C:\ArtCash\backup.mdb].Pay
INSERT INTO Pay2 SELECT * FROM [MS Access;PWD=CashArt;DATABASE=C:\ArtCash\backup.mdb].Pay

INSERT INTO Pay IN 'c:\artcash\backup.mdb' SELECT * FROM Pay
INSERT INTO Pay IN 'c:\artcash\backup.mdb'[;PWD=CashArt] SELECT * FROM Pay
INSERT INTO Pay IN ''[MS Access;PWD=CashArt;DATABASE=c:\artcash\backup.mdb] SELECT * FROM Pay

SELECT INTO Pay FROM [MS Access;PWD=CashArt;DATABASE=C:\ArtCash\backup.mdb].Pay
SELECT P.Name FROM [c:\artcash\backup.mdb].Pay Y LEFT JOIN Persons P ON P.PersonID=Y.PersonID
-----------------------------


-----------------------------
  cross database with join
-----------------------------
SELECT Customers1.Name, Customers1.Address, Customers2.Address FROM Customers As Customers1
  INNER JOIN Customers As Customers2 ON (Customers1.Name = Customers2.Name)
  IN "c:\db2.mdb"
  WHERE Customers1.Name="TEST"
-----------------------------

*/
