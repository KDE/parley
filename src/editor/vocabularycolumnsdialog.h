/*
    options to enable/disable columns for the languages
    SPDX-FileCopyrightText: 2008 Javier goday <jgoday@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VOCABULARYCOLUMNSDIALOG_H
#define VOCABULARYCOLUMNSDIALOG_H

#include <QMap>

#include <QDialog>

class DocumentSettings;
class QGridLayout;
class QStandardItem;
class QStandardItemModel;
class QVBoxLayout;
class KEduVocDocument;

namespace Editor
{

/**
* Show options to enable/disable the columns for each language of the current document
*/
class VocabularyColumnsDialog : public QDialog
{
    Q_OBJECT
public:
    enum Roles {
        ColumnRole = 0x12,
        LanguageRole = 0x18
    };

    VocabularyColumnsDialog(KEduVocDocument *doc, QWidget *parent);
    ~VocabularyColumnsDialog();

    void accept() Q_DECL_OVERRIDE;

private:
    /**
    * Initialize the gridlayout of the widget
    * with one treeview for each language showing the possible columns
    */
    void createLanguagesLayout();

    /**
    * Adds a language to the main gridlayout
    */
    void addLanguage(int language, QVBoxLayout *parent);

    /**
    * Save the visible columns for each language in the document settings
    */
    void saveVisibleColumns();

    DocumentSettings *m_settings;
    QGridLayout *m_box;
    KEduVocDocument *m_doc;
    QMap <int, QStandardItemModel *> m_models;
};
}

#endif
