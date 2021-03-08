/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMAGECHOOSERWIDGET_H
#define IMAGECHOOSERWIDGET_H


#include "ui_imagechooserwidget.h"

class KEduVocExpression;

namespace Editor
{

class ImageChooserWidget : public QWidget, public Ui::ImageChooserWidget
{
    Q_OBJECT
public:
    explicit ImageChooserWidget(QWidget* parent = 0);

public slots:
    /**
     * Update the word
     * @param entry
     * @param translation
     */
    void setTranslation(KEduVocExpression* entry, int translation);

// signals:
//     /**
//      * Important to let other widgets know that a certain word type is edited (might make conjugations/comparison/declension appear)
//      * @param
//      */
//     void imageChanged(const QString&);

private slots:
    void slotImageChanged(const QString & url);

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    KEduVocExpression* m_entry;

};
}
#endif
