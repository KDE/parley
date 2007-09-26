/***************************************************************************

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STARTPRACTICEDIALOG_H
#define STARTPRACTICEDIALOG_H

#include <keduvocdocument.h>
#include <KConfigDialog>

class StartPracticeWidget;
class QueryOptions;
class ThresholdOptions;
class BlockOptions;

/**
  *
  */
class StartPracticeDialog : public KConfigDialog
{
    Q_OBJECT
public:
    StartPracticeDialog(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config);

    ~StartPracticeDialog();

    void commitData();

private slots:
    /**
     * Called when the user clicks Apply or OK.
     */
    void updateSettings();
    /**
     * Updates dialog widgets. Here only used after loading a profile.
     * Profiles only store the settings of the last three pages in the dialog.
     */
    void updateWidgets();
    /**
     * Called when the user clicks Default
     */
    void updateWidgetsDefault();

    /**
     * Returns true if the current state of the dialog is different from the saved settings
     */
    bool hasChanged();

private:

    /**
     * Returns true if the current state of the dialog represents the default settings.
     */
    bool isDefault();

    StartPracticeWidget* m_startPracticeWidget;
    QueryOptions* m_queryOptions;
    ThresholdOptions* m_thresholdOptions;
    BlockOptions* m_blockOptions;

    KConfigSkeleton* m_config;
};

#endif // StartPracticeDialog_included

