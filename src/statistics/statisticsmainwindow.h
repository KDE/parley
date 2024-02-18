/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2008-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STATISTICSMAINWINDOW_H
#define STATISTICSMAINWINDOW_H

#include <KXMLGUIClient>
#include <QWidget>
#include <memory>

class ConjugationOptions;
class StatisticsModel;
class KEduVocDocument;
class ParleyMainWindow;
namespace Ui
{
class StatisticsMainWindow;
}

class StatisticsMainWindow : public QWidget, public KXMLGUIClient
{
    Q_OBJECT
public:
    StatisticsMainWindow(const std::shared_ptr<KEduVocDocument> &doc, ParleyMainWindow *parent);
    ~StatisticsMainWindow() override;

    void setDocument(const std::shared_ptr<KEduVocDocument> &doc);

    void syncConfig();

Q_SIGNALS:
    void languagesChanged(int, int);

private Q_SLOTS:
    void configurePractice();
    void languagesChanged();
    void practiceModeSelected(int mode);
    void practiceDirectionChanged(int mode);
    void rememberPracticeDirectionChanged(bool checked);
    void updateVisibleColumns();
    void updateModelSettings();

private:
    void initActions();
    void initPracticeModeSelection();
    void initLanguages();
    void initPracticeMode();
    void showConjugationOptions(bool visible = true);
    int practiceDirectionForPracticeMode(int mode) const;
    void setPracticeDirectionForPracticeMode(int direction, int mode);

    ParleyMainWindow *m_mainWindow{nullptr};
    std::shared_ptr<KEduVocDocument> m_doc;
    StatisticsModel *m_statisticsModel{nullptr};
    Ui::StatisticsMainWindow *m_ui{nullptr};
    ConjugationOptions *m_conjugationOptions{nullptr};
};

#endif
