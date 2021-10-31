/*
    SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PARLEYPREFS_H
#define PARLEYPREFS_H

#include <KConfigWidgets/KConfigDialog>

/**
 *@author Peter Hedlund
 */

class GeneralOptions;
class ViewOptions;
class TranslationShellOptions;
class KEduVocDocument;
class KComboBox;
class KConfigSkeleton;

class ParleyPrefs : public KConfigDialog
{
    Q_OBJECT

public:
    /**
     * Creates a configuration dialog.
     */
    ParleyPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config);

protected slots:
    /**
     * Called when the user clicks Apply or OK.
     */
    void updateSettings() override;
    /**
     * Updates dialog widgets. Here only used after loading a profile.
     * Profiles only store the settings of the last three pages in the dialog.
     */
    void updateWidgets() override;
    /**
     * Called when the user clicks Default
     */
    void updateWidgetsDefault() override;

protected:
    /**
     * Returns true if the current state of the dialog is different from the saved settings
     */
    bool hasChanged() override;
    /**
     * Returns true if the current state of the dialog represents the default settings.
     */
    bool isDefault() override;

private:
    GeneralOptions *m_generalOptions{nullptr};
    ViewOptions *m_viewOptions{nullptr};
    TranslationShellOptions *m_translationShellOptions{nullptr};

    KConfigSkeleton *m_config{nullptr};
    KEduVocDocument *m_doc{nullptr};
    KComboBox *m_lessons{nullptr};
};

#endif
