/***************************************************************************

    C++ Interface: keduvocwordtype

    -----------------------------------------------------------------------

    begin         : Mi Aug 22 2007

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
#ifndef KEDUVOCWORDTYPE_H
#define KEDUVOCWORDTYPE_H

#include <keduvocdocument.h>

/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEduVocWordType{
public:
    KEduVocWordType();

    ~KEduVocWordType();

    void setDocument(KEduVocDocument *doc);

    QString getMainTypeFromOldFormat(const QString& typeSubtypeString) const;
    QString getSubTypeFromOldFormat(const QString& typeSubtypeString) const;
    QString getOldType(const QString& mainType, const QString& subType) const;

    QStringList getMainTypeList() const;
    QStringList getSubTypeList(const QString& mainType) const;

private:
    static const QString KVTML_1_TYPE_USER;
    static const QString KVTML_1_TYPE_DIV;

    void initOldTypeLists();

    KEduVocDocument *m_doc;

    /// user defined types of old documents
    QStringList m_userTypeDescriptions;

    QMap<QString, QString> m_oldMainTypeNames;
    QMap<QString, QString> m_oldSubTypeNames;

    QMap<QString, QStringList> m_wordTypeList;
};

#endif
