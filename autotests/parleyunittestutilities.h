/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

/**
@file
This file collects some utility classes that are used in multiple unit tests
**/


#ifndef PARLEYUNITTESTUTILITIES_H
#define PARLEYUNITTESTUTILITIES_H

#include "parleydocument.h"

#include <QTemporaryFile>

#include <QUrl>

#include <QTest>
#include <QDebug>

namespace UnitTestUtilities
{

/** Class to manage creation/destruction of a kvtml temp doc*/
class TemporaryVocDoc : public QTemporaryFile
{
public :
    /** Create the file, fix the suffix and instantiate it.*/
    TemporaryVocDoc() ;

    /** Destructor*/
    ~TemporaryVocDoc();
};

/** Creates a minimal doc that will save and load error free.*/
class MinimalTempVocDoc : public TemporaryVocDoc
{
public :
    /** The minimal doc has generator, author, lang and local */
    MinimalTempVocDoc();

    /** Destructor*/
    ~MinimalTempVocDoc();
};

}
#endif //PARLEYUNITTESTUTILITIES_H
