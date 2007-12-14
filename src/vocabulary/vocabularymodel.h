/***************************************************************************

    Copyright: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <keduvocexpression.h>
#include <keduvoctranslation.h>
#include <QAbstractTableModel>

class KEduVocDocument;
class KEduVocLesson;


/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class VocabularyModel : public QAbstractTableModel
{
Q_OBJECT
public:
    enum entryColumns {
        Translation,
        Pronunciation,
        WordType,
        Synonym,
        Antonym,
        Example,
        Comment,
        Paraphrase,
//         Audio,
//         Image,
        EntryColumnsMAX
    };

    enum roles {
        TranslationRole = Qt::UserRole,
        EntryRole
    };

    VocabularyModel(QObject *parent = 0);

    ~VocabularyModel();


    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex&, int) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    static int translation(int column);
    static int columnType(int column);

    QModelIndex appendEntry();

public slots:
    void setDocument(KEduVocDocument *doc);

    /**
     * Whatever the contents, the model will now display it.
     * @param container 
     */
    void showContainer(KEduVocContainer *container);
    
    /**
     * 
     * @param lessonContainer 
     */
    void setLesson(KEduVocLesson *lessonContainer);


    /**
     * 
     * @param wordTypeContainer 
     */
    void setWordType(KEduVocWordType *wordTypeContainer);

private:
    KEduVocContainer *m_container;
    KEduVocLesson *m_lesson;
    KEduVocWordType *m_wordType;

    KEduVocDocument *m_document;
};

Q_DECLARE_METATYPE(KEduVocExpression*)

#endif
