/***************************************************************************

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

#ifndef CONFIGUREPRACTICEWIDGET_H
#define CONFIGUREPRACTICEWIDGET_H

#include "ui_configurepracticewidget.h"

class KEduVocDocument;
class KConfigSkeleton;
class QStackedLayout;

/**
 *
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class ConfigurePracticeWidget
    : public QWidget, public Ui::ConfigurePracticeWidget
{
Q_OBJECT
public:
    ConfigurePracticeWidget(KEduVocDocument* doc, QWidget *parent);

    /**
     * Called when the user clicks Apply or OK.
     */
    void updateSettings();
    /**
     * Updates dialog widgets.
     */
    void updateWidgets();

    bool hasChanged();
    bool isDefault();

private slots:
    void fromLanguageSelected(int identifierFromIndex);

    void comparisonRadioToggled(bool checked);
    void conjugationRadioToggled(bool checked);
    void otherRadioToggled(bool checked);

private:
    enum StackedWidgets {
        Empty,
        Conjugation,
        Comparison
    };
    QStackedLayout* m_optionsStackedLayout;
    KEduVocDocument* m_doc;
};

#endif
