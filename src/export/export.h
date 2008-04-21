/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXPORT_H
#define EXPORT_H

class ParleyDocument;
class QWidget;

class ParleyExport
{
public:
    static void exportDocument(ParleyDocument* document, QWidget *parent = 0);

// private:
//     ParleyExport(KEduVocDocument* file, QWidget *parent = 0);

};

#endif
