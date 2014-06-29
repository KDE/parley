#include "translationsfetcher.h"
#include <QDebug>
#include <QFile>

TranslationsFetcher::TranslationsFetcher(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"In TranslationFetcher()";
}

TranslationsFetcher::~TranslationsFetcher()
{

}

void TranslationsFetcher::fetchTranslationFromUrl(QString urlString)
{
    QUrl url(urlString);
    m_fileDownloader = new FileDownloader(url, this);
    connect(m_fileDownloader, SIGNAL(downloaded()), this, SLOT(writeFile()));
}

void TranslationsFetcher::writeFile()
{
    qDebug()<<"In TranslationFetcher::writeFile()";
    QFile file("owd_fra_csv.zip");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << m_fileDownloader->downloadedData();
}