/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONFIGUREPRACTICEDIALOG_H
#define CONFIGUREPRACTICEDIALOG_H

#include <KEduVocDocument>
#include <KConfigWidgets/KConfigDialog>
#include <KConfigSkeleton>

class GeneralPracticeOptions;
class SpecificPracticeOptions;
class ThresholdOptions;
class BlockOptions;

/**
  *
  */
class ConfigurePracticeDialog : public KConfigDialog
{
    Q_OBJECT

public:
    ConfigurePracticeDialog(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config);

    ~ConfigurePracticeDialog();

    void commitData();

private slots:
    /**
     * Called when the user clicks Apply or OK.
     */
    void updateSettings() Q_DECL_OVERRIDE;
    /**
     * Updates dialog widgets. Here only used after loading a profile.
     * Profiles only store the settings of the last three pages in the dialog.
     */
    void updateWidgets() Q_DECL_OVERRIDE;
    /**
     * Called when the user clicks Default
     */
    void updateWidgetsDefault() Q_DECL_OVERRIDE;

    /**
     * Returns true if the current state of the dialog is different from the saved settings
     */
    bool hasChanged() Q_DECL_OVERRIDE;

private:

    /**
     * Returns true if the current state of the dialog represents the default settings.
     */
    bool isDefault() Q_DECL_OVERRIDE;

    GeneralPracticeOptions  *m_generalPracticeOptions;
    BlockOptions            *m_blockOptions;
    ThresholdOptions        *m_thresholdOptions;
    SpecificPracticeOptions *m_specificPracticeOptions;

    KConfigSkeleton *m_config;
};

#endif // ConfigurePracticeDialog_included
