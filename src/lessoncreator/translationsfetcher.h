/***************************************************************************
    Copyright 2014 Amarvir Singh <amarvir.ammu.93@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef TRANSLATIONSFETCHER_H
#define TRANSLATIONSFETCHER_H

#include <QObject>
#include "filedownloader.h"

class TranslationsFetcher : public QObject
{
    Q_OBJECT
public:
    explicit TranslationsFetcher(QObject *parent = 0);

    virtual ~TranslationsFetcher();

    void fetchTranslationFromUrl(QString urlString);

private slots:

    void writeFile();

private:

   FileDownloader *m_fileDownloader;

};

#endif // TRANSLATIONSFETCHER_H