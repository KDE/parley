#include "csv.h"
#include <QTextDecoder>

CSV::CSV(QIODevice *device)
{
    m_device = device;
    m_codec = QTextCodec::codecForLocale();
    m_pos = 0;
    m_rx = QRegExp("((?:(?:[^;\\n]*;?)|(?:\"[^\"]*\";?))*)\\n");
}

CSV::CSV(QString &string)
{
    m_device = NULL;
    m_codec = QTextCodec::codecForLocale();
    m_string = string;
    m_pos = 0;
    m_rx = QRegExp("((?:(?:[^;\\n]*;?)|(?:\"[^\"]*\";?))*)\\n");
}

CSV::~CSV()
{
    //delete m_codec;
}

void CSV::setCodec(const char *codecName)
{
    //delete m_codec;
    m_codec = QTextCodec::codecForName(codecName);
}

QString CSV::readLine()
{
    QString line;

    if (m_string.isNull()) {
        //READ DATA FROM DEVICE
        if (m_device && m_device->isReadable()) {
            QTextDecoder dec(m_codec);
            m_string = dec.toUnicode(m_device->readAll());
        }
        else {
            return QString();
        }
    }

    //PARSE
    if ((m_pos = m_rx.indexIn(m_string,m_pos)) != -1) {
        line = m_rx.cap(1);
        m_pos += m_rx.matchedLength();
    }
    return line;
}

QStringList CSV::parseLine()
{
    return parseLine(readLine());
}

void CSV::parseFile()
{
    QFile file (m_string);
    if (file.open(QIODevice::ReadOnly)) {
        QString data = file.readAll();
        data.remove( QRegExp("\r") ); //remove all ocurrences of CR (Carriage Return)
        QString temp;
        QChar character;
        QTextStream textStream(&data);
        while (!textStream.atEnd()) {
            textStream >> character;
            if (character == ',') {
                checkString(temp, character);
            } else if (character == '\n') {
                checkString(temp, character);
            } else if (textStream.atEnd()) {
                temp.append(character);
                checkString(temp);
            } else {
                temp.append(character);
            }
        }
    }
}

void CSV::checkString(QString &temp, QChar character)
{

    if(temp.count("\"")%2 == 0) {
        //if (temp.size() == 0 && character != ',') //problem with line endings
        //    return;
        if (temp.startsWith( QChar('\"')) && temp.endsWith( QChar('\"') ) ) {
             temp.remove( QRegExp("^\"") );
             temp.remove( QRegExp("\"$") );
        }
        //FIXME: will possibly fail if there are 4 or more reapeating double quotes
        temp.replace("\"\"", "\"");
        tempList << temp;
        if (character != QChar(',')) {
            bigList << tempList;
            tempList.clear();
        }
        temp.clear();
    } else {
        temp.append(character);
    }
}

QList<QStringList> CSV::list()
{
    return bigList;
}

QStringList CSV::parseLine(QString line)
{
    QStringList list;
    int pos2 = 0;
    QRegExp rx2("(?:\"([^\"]*)\";?)|(?:([^;]*);?)");
    if (line.size()<1) {
        list << "";
    }
    else {
        while (line.size()>pos2 && (pos2 = rx2.indexIn(line, pos2)) != -1) {
            QString col;
            if(rx2.cap(1).size()>0)
                col = rx2.cap(1);
            else if(rx2.cap(2).size()>0)
                col = rx2.cap(2);
            list << col;
            if(col.size())
                pos2 += rx2.matchedLength();
            else
                pos2++;
        }
    }
    return list;
}
