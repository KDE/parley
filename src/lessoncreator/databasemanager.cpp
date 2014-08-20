#include "databasemanager.h"
#include "csv.h"
#include <QDebug>

#include <QFile>
#include <QTest>
#include <kstandarddirs.h>

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{

}

DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::createDB()
{
    QString drv_name = "sqlite3";
    QString db_name = KStandardDirs::locateLocal("data", "parley/test.kexi", true /*create dir*/);

    KexiDB::DriverManager manager;
    //get driver
    const KexiDB::Driver::Info drv_info = manager.driverInfo(drv_name);
    KexiDB::Driver *driver = manager.driver(drv_name);

    //open connection
    KexiDB::ConnectionData cdata;
    cdata.setFileName(db_name);
    conn = driver->createConnection(cdata);
    if (driver->error() && conn) qDebug()<< "Failed to create connection";
     {

        if(!conn->connect()) qDebug()<<"Failed to connect";
        if (conn->databaseExists(db_name)) {
            if(!conn->dropDatabase(db_name)) qDebug()<<"Failed to drop database";
        }
        if(!conn->createDatabase(db_name)) qDebug()<< "Failed to create db";
        if(!conn->useDatabase(db_name)) qDebug()<<"Failed to use db";

        createTables();

        if(!conn->disconnect()) qDebug()<<"Failed to disconnect database";
    }
}

void DatabaseManager::createTables()
{
     conn->setAutoCommit(false);
    KexiDB::Transaction t = conn->beginTransaction();
    if (conn->error()) {
        conn->debugError();
        return;
    }

    //now: lets create tables:
    KexiDB::Field *f;
    KexiDB::TableSchema *t_langfrom = new KexiDB::TableSchema("langfrom_syn");
    t_langfrom->setCaption("Language From");
    t_langfrom->addField(f = new KexiDB::Field("id", KexiDB::Field::Integer, KexiDB::Field::PrimaryKey | KexiDB::Field::AutoInc, KexiDB::Field::Unsigned));
    f->setCaption("ID");
    t_langfrom->addField(f = new KexiDB::Field("dm_id", KexiDB::Field::Integer, 0, KexiDB::Field::Unsigned));
    f->setCaption("Dictionary Meaning ID");
    t_langfrom->addField(f = new KexiDB::Field("lang_id", KexiDB::Field::Integer, 0, KexiDB::Field::Unsigned));
    f->setCaption("Language ID");
    t_langfrom->addField(f = new KexiDB::Field("syntrans_id", KexiDB::Field::Integer, 0, KexiDB::Field::Unsigned));
    f->setCaption("Syntrans ID");
    t_langfrom->addField(f = new KexiDB::Field("spelling", KexiDB::Field::Text));
    f->setCaption("Spelling");
    if (!conn->createTable(t_langfrom)) {
        conn->debugError();
        return;
    }

    // Inserting data into table after parsing appropriate CSV file.
    QString db_name = KStandardDirs::locateLocal("data", "parley/owd_syn_eng.csv"); //TODO:Change eng to lang1
    CSV csv(db_name);
    csv.parseFile();
    QList<QStringList> bigList = csv.list();
    int x = 1;
    QListIterator<QStringList> i(bigList);
    while (i.hasNext()) {
        QStringList stringList= i.next();
        conn->insertRecord(*t_langfrom, QVariant(x++), QVariant(stringList.at(0)), QVariant(stringList.at(1)), QVariant(stringList.at(2)), QVariant(stringList.at(3)));
    }

    KexiDB::TableSchema *t_langto = new KexiDB::TableSchema("langto_syn");
    t_langto->setCaption("Language To");
    t_langto->addField(f = new KexiDB::Field("id", KexiDB::Field::Integer, KexiDB::Field::PrimaryKey | KexiDB::Field::AutoInc, KexiDB::Field::Unsigned));
    f->setCaption("ID");
    t_langto->addField(f = new KexiDB::Field("dm_id", KexiDB::Field::Integer, 0, KexiDB::Field::Unsigned));
    f->setCaption("Dictionary Meaning ID");
    t_langto->addField(f = new KexiDB::Field("lang_id", KexiDB::Field::Integer, 0, KexiDB::Field::Unsigned));
    f->setCaption("Language ID");
    t_langto->addField(f = new KexiDB::Field("syntrans_id", KexiDB::Field::Integer, 0, KexiDB::Field::Unsigned));
    f->setCaption("Syntrans ID");
    t_langto->addField(f = new KexiDB::Field("spelling", KexiDB::Field::Text));
    f->setCaption("Spelling");
    if (!conn->createTable(t_langto)) {
        conn->debugError();
        return;
    }

    // Inserting data into table after parsing appropriate CSV file.
    db_name = KStandardDirs::locateLocal("data", "parley/owd_syn_fra.csv"); //TODO:Change eng to lang2
    CSV csv2(db_name);
    csv2.parseFile();
    bigList = csv2.list();
    x = 1;
    QListIterator<QStringList> i2(bigList);
    while (i2.hasNext()) {
        QStringList stringList= i2.next();
        conn->insertRecord(*t_langfrom, QVariant(x++), QVariant(stringList.at(0)), QVariant(stringList.at(1)), QVariant(stringList.at(2)), QVariant(stringList.at(3)));
    }

    if (!conn->commitTransaction(t)) {
        conn->debugError();
        return;
    }

    qDebug() << "NOW, TABLE LIST: ";
    QStringList tnames = conn->tableNames();
    for (QStringList::iterator it = tnames.begin(); it != tnames.end(); ++it) {
        qDebug() << " - " << (*it);
    }

    if (!conn->closeDatabase()) {
        conn->debugError();
        return;
    }
}

void DatabaseManager::getAllTranslations()
{
    KexiDB::Cursor *c = conn->executeQuery("SELECT f.spelling, t.spelling, f.dm_id FROM langfrom_syn AS f, langto_syn AS t WHERE f.dm_id = t.dm_id");
    if (!c) qDebug() << conn->errorMsg();
    if (c) {
        while (c->moveNext()) {
            QString fromWord = c->value(0).toString();
            QString toWord = c->value(1).toString();
            int dm_id = c->value(2).toInt();
            qDebug() << dm_id << fromWord << toWord; // Use the values.
        }
        //qDebug() << "Cursor error:" << c->errorMsg();
    }

}