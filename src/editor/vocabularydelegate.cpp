/*
    SPDX-FileCopyrightText: 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "vocabularydelegate.h"
#include "vocabularymodel.h"
#include "vocabularyfilter.h"

#include "prefs.h"
#include "languagesettings.h"

#include <KEduVocExpression>
#include <KEduVocWordtype>
#include <KLocalizedString>
#include <KComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QTreeView>
#include <QHeaderView>
#include <QDBusInterface>
#include <QKeyEvent>
#include <QPainter>
#include <QToolTip>
#include <QPainterPath>

using namespace Editor;

VocabularyDelegate::VocabularyDelegate(QObject *parent)
    : QItemDelegate(parent), m_translator(0)
{
}

QSet<QString> VocabularyDelegate::getTranslations(const QModelIndex & index) const
{
    if (Prefs::automaticTranslation() == false) return QSet<QString>();

    QSet<QString> translations; //translations of this column from all the other languages

    int language = index.column() / VocabularyModel::EntryColumnsMAX;
    QString toLanguage = m_doc->identifier(language).locale();

    //iterate through all the Translation columns
    for (int i = 0; i < index.model()->columnCount(index.parent()); i ++) {
        if (VocabularyModel::columnType(i) == VocabularyModel::Translation) {    //translation column
            QString fromLanguage = m_doc->identifier(VocabularyModel::translation(i)).locale();
            QString word = index.model()->index(index.row(), i, QModelIndex()).data().toString();

            if (fromLanguage != toLanguage) {
//                 qDebug() << fromLanguage << toLanguage << word;
                //get the word translations and add them to the translations set
                QSet<QString> * tr = m_translator->getTranslation(word, fromLanguage, toLanguage);
                if (tr)
                    translations.unite(* (tr));
            }
        }
    }

    return translations;
}

QWidget * VocabularyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);    /// as long as it's unused

    if (!index.isValid()) {
        return 0;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case VocabularyModel::WordClass: {
        if (!m_doc) return 0;
        KComboBox *wordTypeCombo = new KComboBox(parent);

        WordTypeBasicModel *basicWordTypeModel = new WordTypeBasicModel(parent);
        wordTypeCombo->setModel(basicWordTypeModel);
        QTreeView *view = new QTreeView(parent);

        view->setModel(basicWordTypeModel);
        wordTypeCombo->setView(view);

        view->header()->setVisible(false);
        view->setRootIsDecorated(true);

        basicWordTypeModel->setDocument(m_doc);
        view->expandAll();

        qDebug() << "index data" << index.data().toString();
        //view->setCurrentItem();

        return wordTypeCombo;
    }

    case VocabularyModel::Translation:
        if (!m_doc || !m_translator) return 0;

        if (VocabularyModel::columnType(index.column()) == VocabularyModel::Translation) {
            //get the translations of this word (fetch only with the help of scripts, if enabled)
            QSet<QString> translations = getTranslations(index);

            //create combo box
            //if there is only one word and that is the suggestion word (in translations) then don't create the combobox
            if (!translations.isEmpty() && !(translations.size() == 1 && (*translations.begin()) == index.model()->data(index, Qt::DisplayRole).toString())) {
                KComboBox *translationCombo = new KComboBox(parent);
                translationCombo->setFrame(false);
                translationCombo->addItems(translations.values());
                translationCombo->setEditable(true);
                translationCombo->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
                translationCombo->setEditText(index.model()->data(index, Qt::DisplayRole).toString());
                translationCombo->completionObject()->setItems(translations.values());
                return translationCombo;
            }
        }
        // no break - we fall back to a line edit if there are not multiple translations fetched onlin
        // fallthrough
    default: {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setFrame(false);
        editor->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
        editor->setText(index.model()->data(index, Qt::DisplayRole).toString());

        QString locale = index.model()->data(index, VocabularyModel::LocaleRole).toString();
        if (!locale.isEmpty()) {
            LanguageSettings settings(locale);
            settings.load();
            QString layout = settings.keyboardLayout();
            if (!layout.isEmpty()) {
                QDBusInterface kxkb(QStringLiteral("org.kde.keyboard"), QStringLiteral("/Layouts"), QStringLiteral("org.kde.KeyboardLayouts"));
                if (kxkb.isValid()) {
                    kxkb.call(QStringLiteral("setLayout"), layout);
                }
            }
        }
        return editor;
    }
    }
}

bool VocabularyDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view,
                                   const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(view);

    if (event->type() == QEvent::ToolTip) {
        QPainterPath audioPainterPath;
        QPainterPath imagePainterPath;
        audioPainterPath.addPolygon(audioPolygon(option));
        imagePainterPath.addPolygon(imagePolygon(option));

        int column = columnType(index.column());

        if (audioPainterPath.contains(event->pos()) && hasAudio(index) && (column == Translation || column == Pronunciation)) {
            QToolTip::showText(event->globalPos(), i18n("Sound file selected: %1", audioUrl(index)));
        }
        else if (imagePainterPath.contains(event->pos()) && hasImage(index) && (column == Translation || column == Pronunciation)) {
            QToolTip::showText(event->globalPos(), i18n("Image file selected: %1", imageUrl(index)));
        }
        else {
            QToolTip::hideText();
            event->ignore();
        }
        return true;
    }
    return false;
}

QPolygon VocabularyDelegate::audioPolygon(const QStyleOptionViewItem &option) const
{
    QRect rect = option.rect;
    QPolygon polygon;
    polygon << QPoint(rect.x() + rect.width() - 10, rect.y());
    polygon << QPoint(rect.x() + rect.width(), rect.y());
    polygon << QPoint(rect.x() + rect.width(), rect.y() + 10);
    return polygon;
}

QPolygon VocabularyDelegate::imagePolygon(const QStyleOptionViewItem &option) const
{
    QRect rect = option.rect;
    QPolygon polygon;
    polygon << QPoint(rect.x() + rect.width() - 10, rect.y() + rect.height());
    polygon << QPoint(rect.x() + rect.width(), rect.y() + rect.height());
    polygon << QPoint(rect.x() + rect.width(), rect.y() + rect.height() - 10);
    return polygon;
}

bool VocabularyDelegate::hasAudio(const QModelIndex &index) const
{
    return !audioUrl(index).isEmpty();
}

bool VocabularyDelegate::hasImage(const QModelIndex &index) const
{
    return !imageUrl(index).isEmpty();
}

QString VocabularyDelegate::audioUrl(const QModelIndex &index) const
{
    QVariant audioVar = index.data(VocabularyModel::AudioRole);
    QString audioUrl = audioVar.toString();
    return audioUrl;
}

QString VocabularyDelegate::imageUrl(const QModelIndex &index) const
{
    QVariant imageVar = index.data(VocabularyModel::ImageRole);
    QString imageUrl = imageVar.toString();
    return imageUrl;
}

int VocabularyDelegate::columnType(int column)
{
    return column % EntryColumnsMAX;
}

void VocabularyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
    painter->save();

    int column = columnType(index.column());

    if (hasAudio(index) == true && (column == Translation || column == Pronunciation)) {
        painter->setPen(QPen(Qt::red));
        painter->setBrush(QBrush(Qt::red));
        painter->drawPolygon(audioPolygon(option));
    }
    if (hasImage(index) == true && (column == Translation || column == Pronunciation)) {
        painter->setPen(QPen(Qt::blue));
        painter->setBrush(QBrush(Qt::blue));
        painter->drawPolygon(imagePolygon(option));
    }
    painter->restore();
}

void VocabularyDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    if (!index.isValid()) {
        return;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case (VocabularyModel::Translation) : {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        KComboBox * translationCombo = qobject_cast<KComboBox*> (editor);
        if (translationCombo) {
            translationCombo->setEditText(value);
            if (value.isEmpty()) {
                // show the translations that were fetched as popup
                translationCombo->showPopup();
            }
            break;
        }
    }
    // fallthrough
    default: {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();

        QLineEdit *lineEdit = qobject_cast<QLineEdit*> (editor);
        if (lineEdit) {
            lineEdit->setText(value);
        }
    }
    }
}

void VocabularyDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    if (!index.isValid()) {
        return;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case (VocabularyModel::WordClass) : {
        qDebug() << "word type editor";
        KComboBox *combo = qobject_cast<KComboBox*> (editor);
        if (!combo) {
            return;
        }
        qDebug() << "combo" << combo->currentText();
        QModelIndex comboIndex = combo->view()->currentIndex();
        KEduVocWordType* wordType = static_cast<KEduVocWordType*>(comboIndex.internalPointer());

        // the root is the same as no word type
        if (wordType && wordType->parent() == 0) {
            wordType = 0;
        }

        VocabularyFilter *filter = qobject_cast<VocabularyFilter*> (model);
        VocabularyModel *vocModel = qobject_cast<VocabularyModel*> ((filter)->sourceModel());
        Q_ASSERT(vocModel);
        QVariant data = vocModel->data(filter->mapToSource(index), VocabularyModel::EntryRole);

        KEduVocExpression *expression = data.value<KEduVocExpression*>();
        Q_ASSERT(expression);
        int translationId = VocabularyModel::translation(index.column());

        expression->translation(translationId)->setWordType(wordType);
        model->setData(index, combo->currentText());
        break;
    }
    case (VocabularyModel::Translation) : {
        QLineEdit *lineEdit = qobject_cast<QLineEdit*> (editor);
        if (lineEdit) {
            model->setData(index, lineEdit->text());
        }
        break;
    }
    default: {
        QLineEdit *lineEdit = qobject_cast<QLineEdit*> (editor);
        if (lineEdit) {
            model->setData(index, lineEdit->text());
        }
    }
    }
}

void VocabularyDelegate::setDocument(const std::shared_ptr<KEduVocDocument> &doc)
{
    m_doc = doc;
}

/*
QPair< QString, QString > VocabularyDelegate::guessWordType(const QString & entry, int language) const
{
    qDebug() << "guessing word type for: " << entry;

    QString article = entry.section(" ", 0, 0);
    if ( article.length() < entry.length() ) {
        if ( article == ->identifier(language).articles().article(KEduVocWordFlag::Singular| KEduVocWordFlag::Definite| KEduVocWordFlag::Masculine) ) {
            qDebug() << "Noun masculine";
            return qMakePair(m_doc->wordTypes().specialTypeNoun(), m_doc->wordTypes().specialTypeNounMale());
        }

    }
    return qMakePair(QString(), QString());
}
*/


VocabularyDelegate::WordTypeBasicModel::WordTypeBasicModel(QObject * parent)
    : ReadonlyContainerModel(KEduVocContainer::WordType, parent)
{
}

KEduVocContainer * VocabularyDelegate::WordTypeBasicModel::rootContainer() const
{
    if (!m_doc) {
        return 0;
    }
    return m_doc->wordTypeContainer();
}

/**
 * Sets the member variable m_translator to a Translator object
 * @param translator Translator Object to be used for retrieving word translations
 */
void VocabularyDelegate::setTranslator(Translator* translator)
{
    m_translator = translator;
}
