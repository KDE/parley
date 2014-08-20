#ifndef CSV_H
#define CSV_H

#include <QObject>
#include <QStringList>
#include <QIODevice>
#include <QTextCodec>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QStringList>
#include <QDebug>

class CSV
{

public:
    CSV(QIODevice *device);
    CSV(QString &string);
    ~CSV();

    QString readLine();
    QStringList parseLine();
    static QStringList parseLine(QString line);

    void setCodec(const char *codecName);
    void parseFile();
    void checkString(QString &temp, QChar character = 0);
    QList<QStringList> list();

private:
    QIODevice *m_device;
    QTextCodec *m_codec;
    QString m_string;
    int m_pos;
    QRegExp m_rx;
    QList<QStringList> bigList;
    QStringList tempList;
	
};

#endif // CSV_H
