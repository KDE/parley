/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statisticsmodel.h"
#include "statisticslegendwidget.h"
#include "utils.h"

#include <KEduVocExpression>
#include <KEduVocTranslation>
#include <KEduVocWordtype>
#include <KLocalizedString>
#include <QGradient>
#include <QDebug>

StatisticsModel::StatisticsModel(QObject *parent)
    : ContainerModel(KEduVocContainer::Lesson, parent)
{
}

QVariant StatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= FirstDataColumn) {
        if (role == Qt::DisplayRole
            && m_doc->identifierCount() > (section - FirstDataColumn) ) {
            return i18nc("Confidence level in language, table header", "Confidence (%1)"
                         , m_doc->identifier(section - FirstDataColumn).name());
        }
    }
    return ContainerModel::headerData(section, orientation, role);
}

QVariant StatisticsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(!m_documentSettings.isEmpty());

    KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());

    // Entrie count
    if (index.column() == TotalCountColumn) {
        if (role == Qt::DisplayRole) {
            switch (Prefs::practiceDirection()) {
            case Prefs::EnumPracticeDirection::KnownToLearning:
                return entryCountForPracticeMode(container, Prefs::learningLanguage());
            case Prefs::EnumPracticeDirection::LearningToKnown:
                return entryCountForPracticeMode(container, Prefs::knownLanguage());
            case Prefs::EnumPracticeDirection::MixedDirectionsWordsOnly:
            case Prefs::EnumPracticeDirection::MixedDirectionsWithSound:
                return entryCountForPracticeMode(container, Prefs::knownLanguage())
                    + entryCountForPracticeMode(container, Prefs::learningLanguage());
            default:
                return entryCountForPracticeMode(container, Prefs::learningLanguage());
            }
        }
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignRight;
        }
    }

    // Colorbars
    if (index.column() >= FirstDataColumn) {
        int translation = index.column() - FirstDataColumn;
        switch (role) {
        case Container:
            {
                // Return a pointer to the container we are working on.
                QVariant var;
                var.setValue(container);
                return var;
            }
        case TotalPercent: // Average grade
            return averageGradeForPracticeMode(container, translation);
        case TotalCount:
            return entryCountForPracticeMode(container, translation);
        case ActiveConjugationTenses:
            return m_documentSettings.at(translation)->conjugationTenses();
        default:
            if ((role >= Grade0) && (role <= Grade7)) {
                return expressionsOfGradeForPracticeMode(container, translation, role - Grade0);
            }
        }
    }

    // Checkboxes
    if (index.column() == 0 && role == Qt::CheckStateRole) {
        if (container->inPractice()) {
            return Qt::Checked;
        } else {
            return Qt::Unchecked;
        }
    }

    return ContainerModel::data(index, role);
}

int StatisticsModel::averageGradeForPracticeMode(KEduVocContainer *container, int translation) const
{
    WordCount wordCount;
    wordCount.fillFromContainerForPracticeMode(
        *container,
        translation,
        m_documentSettings.at(translation)->conjugationTenses()
    );
    return wordCount.percentageCompleted();
}

int StatisticsModel::entryCountForPracticeMode(KEduVocContainer *container, int translation) const
{
    WordCount wordCount;
    wordCount.fillFromContainerForPracticeMode(
        *container,
        translation,
        m_documentSettings.at(translation)->conjugationTenses()
    );
    return wordCount.totalWords - wordCount.invalid;
}

int StatisticsModel::expressionsOfGradeForPracticeMode(KEduVocContainer *container,
                                                      int translation, grade_t grade) const
{
    WordCount wordCount;
    wordCount.fillFromContainerForPracticeMode(
        *container,
        translation,
        m_documentSettings.at(translation)->conjugationTenses()
    );
    return wordCount.grades[grade];
}

Qt::ItemFlags StatisticsModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (index.column() == 0) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        }
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    return Qt::NoItemFlags;
}

int StatisticsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_doc->identifierCount() + FirstDataColumn;
}

Qt::DropActions StatisticsModel::supportedDragActions() const
{
    return Qt::IgnoreAction;
}

KEduVocContainer *StatisticsModel::rootContainer() const
{
    if (!m_doc) {
        return 0;
    }
    return m_doc->lesson();
}

void StatisticsModel::loadDocumentsSettings()
{
    m_documentSettings.clear();
    if (!m_doc) {
        return;
    }
    for (int i = 0 ; i < m_doc->identifierCount(); ++i) {
        m_documentSettings << QSharedPointer<DocumentSettings>(
            new DocumentSettings(m_doc->url().url() + QString::number(i))
        );
        m_documentSettings.last()->load();

    }
}

void StatisticsModel::setDocument(const std::shared_ptr<KEduVocDocument> &doc)
{
    beginResetModel();
    m_doc = doc;
    loadDocumentsSettings();
    endResetModel();
}


void StatisticsModel::updateDocumentSettings()
{
    beginResetModel();
    loadDocumentsSettings();
    endResetModel();
}


std::shared_ptr<KEduVocDocument> StatisticsModel::document() const
{
    return m_doc;
}

