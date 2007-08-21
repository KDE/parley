/***************************************************************************

                             kvtlanguagemodel

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KVTLANGUAGEMODEL_H
#define KVTLANGUAGEMODEL_H

#include <QStandardItemModel>

/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KVTLanguageModel : public QStandardItemModel
{
Q_OBJECT
public:
    enum roles{
        LanguageTwoLetterRole = Qt::UserRole+2
    };

    KVTLanguageModel(QObject *parent = 0);

private:

};

#endif
