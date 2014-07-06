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


#ifndef PARLEYUNITTESTUTILITIES_H
#define PARLEYUNITTESTUTILITIES_H

#include "parleydocument.h"

#include <KTemporaryFile>

#include <KUrl>

#include <qtest_kde.h>
#include <KDebug>

#include <QtTest>

namespace UnitTestUtilities
{

/** Class to manage creation/destruction of a kvtml temp doc*/
class TemporaryVocDoc : public KTemporaryFile
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
