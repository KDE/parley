/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PARLEYPRACTICEMAINWINDOW_H
#define PARLEYPRACTICEMAINWINDOW_H

#include <KXmlGuiWindow>

#include <prefs.h>

class QString;
class ActiveArea;
class KUrl;

/**
* @class ParleyPracticeMainWindow
* @author David Capel <wot.narg@gmail.com>
* @brief The central class of Parley's Practice mode. It keeps state and coordinates the actions between various other classs.
*/
class ParleyPracticeMainWindow : public KXmlGuiWindow
{
        Q_OBJECT

    public:
        ParleyPracticeMainWindow(class KEduVocDocument * doc = 0, QWidget * parent = 0);
        ~ParleyPracticeMainWindow();

    /// States that the actions can be in -- Check Answer means the next navigation command should check the answer.
    static const int CheckAnswer = 1;
    /// States that the actions can be in -- Continue means the next navigation command should be to move to the next question.
    static const int Continue = 2;

    protected:
        class KSvgRenderer* m_renderer;

        class PracticeView* m_view;

        /// The active area for questions without images (or if no image theme exists)
        class ActiveArea* m_normalArea;

        /// The active area for questions with images (only if an image theme exists)
        class ActiveArea * m_imageArea;

        class PracticeEntryManager * m_manager;
        class Statistics* m_stats;
        class AnswerValidator* m_validator;
        class Timer * m_timer;

        /// Keeps track of the global action state -- CheckAnswer or Continue.
        int m_state;

        /// Sets up the canvas, view, and essetial logic items.
        void setupBase(const QString& desktopFileFilename, class KEduVocDocument * doc = 0);
        /// Sets up the Active Area for the current mode.
        void setupActiveArea();
        /// Sets up the various KActions
        void setupActions();
        /// Sets up widgets specific to each mode.
        void setupModeSpecifics();
        /// Setups up widgets that are not mode-specific.
        void setupModeIndependent(ActiveArea * area);

        /// Sets up widgets for modes with the written template.
        void setupWrittenTemplate(ActiveArea * area);

        /// Sets up widgets for modes with the multiple choice template.
        void setupMultipleChoiceTemplate(ActiveArea * area);

        /// Sets up widgets for modes with the mixed letters template.
        void setupMixedLettersTemplate(ActiveArea * area);

        /// Sets up widgets for modes with the flash cards template.
        void setupFlashCardTemplate(ActiveArea * area);

        /// Sets up widgets for modes with the comparison template.
        void setupComparisonTemplate(ActiveArea * area);

        /// Sets up widgets for modes with the conjugation template.
        void setupConjugationTemplate(ActiveArea * area);
    public slots:
        /// Takes the given input and uses @c AnswerValidator to check its correctness. (Single QString version)
        void slotCheckAnswer(const QString& input);

        /// Takes the given inputs and uses @c AnswerValidator to check their correctness. (Multiple QString version)
        void slotCheckAnswer(const QStringList& input);
        /// Obtains the solution from the @c PracticeEntryManager and sends it to the Input widget to be displayed.
        /// Emits signalShowSolution(solution)
        /// Triggered by the "show solution" action.
        void slotShowSolution();
        /// Emits signalShowHint(solution) to allow the Input widget to show a hint.
        /// Triggered by the "show hint" action.
        void slotShowHint();
        /// Toggles the internal state between CheckAnswer and Continue.
        void slotToggleCheckAnswerContinueActions();
        /// Creates a preferences dialog that allows the user to change the theme.
        void slotCreatePreferencesDialog();
        /// Forces the entry to be marked as correctly answered. Used for when correction isn't necessary. (eg  Flashcard mode)
        void slotForceCorrect();
        /// Forces the entry to be marked as incorrectly answered. Used for when correction isn't necessary. (eg Flashcard mode)
        void slotForceIncorrect();

        /// Checks if an image is available and should be shown, and if so, shows the image version of the theme.
        void slotShowImageView(const KUrl&, bool backsideOfFlashcard);

        /// Called queryClose()
        void slotClose() { queryClose(); };
        /// Closes down Practice
        bool queryClose();
    signals:
        /// Signals that a solution should be shown.
        void signalShowSolution(const QString& solution);
        /// Signals that the solutions should be shown (for multiple data modes)
        void signalShowSolution(const QStringList& solutions);
        /// Signals that a hint should be shown.
        void signalShowHint(const QString& solution);
        /// Signals that the CheckAnswer/Continue state has toggled.
        void signalCheckAnswerContinueActionsToggled(int mode);
        /// Signals that the Practice has finished (by running out of entries to practice).
        void signalPracticeFinished();
};


#endif
