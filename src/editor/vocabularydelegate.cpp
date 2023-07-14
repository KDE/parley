/*
    SPDX-FileCopyrightText: 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "vocabularydelegate.h"
#include "languagesettings.h"
#include "prefs.h"
#include "vocabularyfilter.h"
#include "vocabularymodel.h"
#include <KComboBox>
#include <KEduVocExpression>
#include <KEduVocWordtype>
#include <KLocalizedString>
#include <QCompleter>
#include <QDBusInterface>
#include <QDebug>
#include <QFutureWatcher>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QToolTip>
#include <QTreeView>

using namespace Editor;

VocabularyDelegate::VocabularyDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *VocabularyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option) /// as long as it's unused

    if (!index.isValid()) {
        return nullptr;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case VocabularyModel::WordClass: {
        if (!m_doc) {
            return nullptr;
        }
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
        // view->setCurrentItem();

        return wordTypeCombo;
    }

    case VocabularyModel::Translation: {
        if (!m_doc) {
            return nullptr;
        }

        // always create combo box for translation selection, because translations are gained async
        QLineEdit *lineedit = new QLineEdit(parent);
        lineedit->setFrame(false);
        lineedit->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
        lineedit->setText(index.model()->data(index, Qt::DisplayRole).toString());

        if (m_translator.isTranslateShellAvailable() && Prefs::automaticTranslation()) {
            QString targetLanguage = m_doc->identifier(index.column() / VocabularyModel::EntryColumnsMAX).locale();
            QString fromLanguage;
            QString word;
            for (int i = 0; i < index.model()->columnCount(index.parent()); i++) {
                if (word.isEmpty() && VocabularyModel::columnType(i) == VocabularyModel::entryColumns::Translation) { // translation column
                    fromLanguage = m_doc->identifier(VocabularyModel::translation(i)).locale();
                    word = index.model()->index(index.row(), i, QModelIndex()).data().toString();
                }
            }

            auto result = m_translator.translateAsync(word, fromLanguage, targetLanguage);
            QFutureWatcher<TranslateShellAdapter::Translation> *watcher = new QFutureWatcher<TranslateShellAdapter::Translation>();
            watcher->setFuture(result);
            connect(watcher, &QFutureWatcher<TranslateShellAdapter::Translation>::finished, lineedit, [lineedit, watcher]() {
                if (!watcher->future().result().m_error && !watcher->future().result().m_suggestions.isEmpty()) {
                    QCompleter *completer = new QCompleter(watcher->future().result().m_suggestions, lineedit);
                    completer->setCaseSensitivity(Qt::CaseInsensitive);
                    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
                    lineedit->setCompleter(completer);
                }
                watcher->deleteLater();
            });
        }

        return lineedit;
    }
        // no break - we fall back to a line edit if there are not multiple translations fetched online
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

bool VocabularyDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(view)

    if (event->type() == QEvent::ToolTip) {
        QPainterPath audioPainterPath;
        QPainterPath imagePainterPath;
        audioPainterPath.addPolygon(audioPolygon(option));
        imagePainterPath.addPolygon(imagePolygon(option));

        int column = columnType(index.column());

        if (audioPainterPath.contains(event->pos()) && hasAudio(index) && (column == Translation || column == Pronunciation)) {
            QToolTip::showText(event->globalPos(), i18n("Sound file selected: %1", audioUrl(index)));
        } else if (imagePainterPath.contains(event->pos()) && hasImage(index) && (column == Translation || column == Pronunciation)) {
            QToolTip::showText(event->globalPos(), i18n("Image file selected: %1", imageUrl(index)));
        } else {
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

void VocabularyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case (VocabularyModel::Translation): {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        KComboBox *translationCombo = qobject_cast<KComboBox *>(editor);
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

        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        if (lineEdit) {
            lineEdit->setText(value);
        }
    }
    }
}

void VocabularyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case (VocabularyModel::WordClass): {
        qDebug() << "word type editor";
        KComboBox *combo = qobject_cast<KComboBox *>(editor);
        if (!combo) {
            return;
        }
        qDebug() << "combo" << combo->currentText();
        QModelIndex comboIndex = combo->view()->currentIndex();
        KEduVocWordType *wordType = static_cast<KEduVocWordType *>(comboIndex.internalPointer());

        // the root is the same as no word type
        if (wordType && wordType->parent() == nullptr) {
            wordType = nullptr;
        }

        VocabularyFilter *filter = qobject_cast<VocabularyFilter *>(model);
        VocabularyModel *vocModel = qobject_cast<VocabularyModel *>((filter)->sourceModel());
        Q_ASSERT(vocModel);
        QVariant data = vocModel->data(filter->mapToSource(index), VocabularyModel::EntryRole);

        KEduVocExpression *expression = data.value<KEduVocExpression *>();
        Q_ASSERT(expression);
        int translationId = VocabularyModel::translation(index.column());

        expression->translation(translationId)->setWordType(wordType);
        model->setData(index, combo->currentText());
        break;
    }
    case (VocabularyModel::Translation): {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        if (lineEdit) {
            model->setData(index, lineEdit->text());
        }
        break;
    }
    default: {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
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

VocabularyDelegate::WordTypeBasicModel::WordTypeBasicModel(QObject *parent)
    : ReadonlyContainerModel(KEduVocContainer::WordType, parent)
{
}

KEduVocContainer *VocabularyDelegate::WordTypeBasicModel::rootContainer() const
{
    if (!m_doc) {
        return nullptr;
    }
    return m_doc->wordTypeContainer();
}

#include "moc_vocabularydelegate.cpp"
