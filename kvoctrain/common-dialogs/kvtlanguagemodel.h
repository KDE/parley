//
// C++ Interface: kvtlanguagemodel
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTLANGUAGEMODEL_H
#define KVTLANGUAGEMODEL_H

#include <QStandardItemModel>

/**
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
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
