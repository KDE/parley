/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

/**
@file
This file collects some utility classes that are used in multiple unit tests
**/

#include "parleyunittestutilities.h"

using namespace UnitTestUtilities;

TemporaryVocDoc::TemporaryVocDoc()
    :QTemporaryFile( QStringLiteral("XXXXXX.kvtml") )
{
    this->open();
    this->close();
}

TemporaryVocDoc::~TemporaryVocDoc()
{
}

MinimalTempVocDoc::MinimalTempVocDoc()
{
    const QString generator = QStringLiteral( "File Locking Unit Tests" );
    const QString author = QStringLiteral( "File Locking Test" );
    const QString lang = QStringLiteral( "File Locking Language Name" );
    const QString locale = QStringLiteral( "en" ) ;

    KEduVocIdentifier lang0;
    lang0.setName( lang );
    lang0.setLocale( locale);

    KEduVocDocument *doc = new KEduVocDocument();
    doc->setAuthor( author );
    doc->appendIdentifier( lang0 );
    doc->setGenerator(generator);
    doc->saveAs(QUrl::fromLocalFile(fileName()), KEduVocDocument::Kvtml);
    delete doc;
}

/** Destructor*/
MinimalTempVocDoc::~MinimalTempVocDoc()
{
}
