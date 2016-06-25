/*
 Copyright 2014 Andreas Xavier <andxav at zoho dot com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
*/

/**
@file This file collects some utility classes that are used in multiple unit tests
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
    const QString generator = QString::fromLatin1( "File Locking Unit Tests" );
    const QString author = QString::fromLatin1( "File Locking Test" );
    const QString lang = QString::fromLatin1( "File Locking Language Name" );
    const QString locale = QString::fromLatin1( "en" ) ;

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
