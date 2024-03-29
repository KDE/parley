/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <KEduVocExpression>
#include <KEduVocTranslation>
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
        Translation = 0,
        Pronunciation,
        WordClass,
        Synonym,
        Antonym,
        Example,
        Comment,
        Paraphrase,
        //         Audio,
        //         Image,
        EntryColumnsMAX

    };

    enum roles { TranslationRole = Qt::UserRole, EntryRole, LocaleRole, AudioRole, ImageRole };

    explicit VocabularyModel(QObject *parent = nullptr);

    ~VocabularyModel() override;

    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &, int) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * Returns the name of the entryColumns column
     * @param document KEduVocDocument document
     * @param translation the number of translation
     * @param column the column index
     * @param addLocaleSuffix controls if locale name should be added to column title
     */
    static QString columnTitle(KEduVocDocument *document, int translation, int column, bool addLocaleSuffix);

    /**
     * Returns which translation this column matches. It starts from 0 and increases every
     * EntryColumnMax columns
     */
    static int translation(int column);

    /**
     * Returns the type of the column specified. Translation types are multiples of
     * EntryColumnsMAX
     */
    static int columnType(int column);

    QModelIndex appendEntry(KEduVocExpression *expression = nullptr);

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    //     bool dropMimeData(const QMimeData *data, Qt::DropAction action,
    //         int row, int column, const QModelIndex &parent);

    void resetLanguages();

public Q_SLOTS:
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

    KEduVocLesson *lesson();

    /**
     * Show the entries of child lessons in selected lessons
     * @param show
     */
    void showEntriesOfSubcontainers(bool show);

    /**
     * Set automatic translation to enabled/disabled
     * @param enabled
     */
    void automaticTranslation(bool enabled);

private:
    KEduVocContainer *m_container;
    KEduVocLesson *m_lesson;

    KEduVocDocument *m_document;
    KEduVocContainer::EnumEntriesRecursive m_recursive;
};

Q_DECLARE_METATYPE(KEduVocExpression *)

#endif
