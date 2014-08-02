/*#include "databasemanager.h"
#include "csv.h"
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{

}

DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::createDB()
{
    //Open the database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName ("test.dev.db");
    if(!db.open ()){
        exit(-1);
    }

    QSqlQuery que;
    que.exec ("CREATE TABLE langfrom_syn(
    dm_id integer,
    lang_id integer,
    syntrans_id integer,
    spelling text,
    IPA text DEFAULT NULL,
    hyphenation text DEFAULT NULL,
    example text DEFAULT NULL,
    usage_1 text DEFAULT NULL,
    POS text DEFAULT NULL,
    usage_2 text DEFAULT NULL,
    area text DEFAULT NULL,
    grammatical_property text DEFAULT NULL,
    number_n text DEFAULT NULL,
    pinyin text DEFAULT NULL,
    gender text DEFAULT NULL,
    primary key( dm_id, lang_id, syntrans_id )
    );");

    QFile f("owd_syn_eng.csv"); // TODO: change eng to lang1
    if(f.open(QIODevice::ReadOnly)){
        QTextStream ts (&f);

        //Travel through the csv file
        while(!ts.atEnd()){
            QString req = "INSERT INTO langfrom_syn VALUES(";
            // split every lines on comma
            QStringList line = ts.readLine().split(','); // TODO: Use CSV csv(&f); qDebug() << csv.parseLine();
            //for every values on a line,
            //append it to the INSERT request
            for(int i=0; i<line .length ();++i){
                req.append(line.at(i));
                req.append(",");
            }
            req.chop(1); // remove the trailing comma
            req.append(");"); // close the "VALUES([...]" with a ");"
            que.exec(req);
        }
        f.close ();
    }

    que.exec ("CREATE TABLE langto_syn(
    dm_id integer,
    lang_id integer,
    syntrans_id integer,
    spelling text,
    IPA text DEFAULT NULL,
    hyphenation text DEFAULT NULL,
    example text DEFAULT NULL,
    usage_1 text DEFAULT NULL,
    POS text DEFAULT NULL,
    usage_2 text DEFAULT NULL,
    area text DEFAULT NULL,
    grammatical_property text DEFAULT NULL,
    number_n text DEFAULT NULL,
    pinyin text DEFAULT NULL,
    gender text DEFAULT NULL,
    primary key( dm_id, lang_id, syntrans_id )
    );");

    QFile f2("owd_syn_fra.csv"); // TODO: change eng to lang2
    if(f2.open(QIODevice::ReadOnly)){
        QTextStream ts (&f2);

        //Travel through the csv file
        while(!ts.atEnd()){
            QString req = "INSERT INTO langto_syn VALUES(";
            // split every lines on comma
            QStringList line = ts.readLine().split(',');
            //for every values on a line,
            //append it to the INSERT request
            for(int i=0; i<line .length ();++i){
                req.append(line.at(i));
                req.append(",");
            }
            req.chop(1); // remove the trailing comma
            req.append(");"); // close the "VALUES([...]" with a ");"
            que.exec(req);
        }
        f2.close ();
    }
}

void DatabaseManager::getAllTranslations()
{
    QSqlQuery que;
    que.exec("SELECT f.spelling, t.spelling, f.dm_id FROM langfrom_syn AS f, langto_syn AS t WHERE f.dm_id = t.dm_id");
    while (query.next()) {
        QString fromWord = query.value(0).toString();
        QString toWord = query.value(1).toString();
        int dm_id = query.value(2).toInt();
        qDebug() << dm_id << fromWord << toWord; // Use the values.
    }
}
*/