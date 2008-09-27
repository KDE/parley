//
// C++ Interface: multipletextualinput
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef MULTIPLETEXTUALINPUT_H
#define MULTIPLETEXTUALINPUT_H

#include <QMap>
#include <QList>
#include <QStringList>
#include <QObject>
class ActiveArea;
class QString;
class KSvgRenderer;
class QLineEdit;


/**
* @class MultipleTextualInput
* @author David Capel <wot.narg@gmail.com>
* @brief Manages a set of input widgets.
* This class receives a set of elementIds and creates a QLineEdit in each one. Later, it is passed a set of solutions
* with slotSetChoices(QStringList); it picks one of them and displays it, but leaves all other input widgets blank.
* The intended use of this class is to give show one element of a set of related solutions (eg the conjugation forms
* of a single verb) and have the user supply the rest.
* This is used in conjunction with @c MultipleTextualPrompt.
*
* All QStringLists passed to a given instance of this class *must* be the same length and *must* be in the same order.
* For example, the first QString passed to slotSetChoices(QStringList) will be put in the input widget for the first
* elementId passed to the constructor.
*/
class MultipleTextualInput : public QObject
{
    Q_OBJECT

    public:
        /// Creates a QLineEdit for each elementId that is passed to it.
        MultipleTextualInput(KSvgRenderer * renderer, ActiveArea * area, QStringList& elementIds, QObject * parent = 0);
        ~MultipleTextualInput();
    public slots:
        /// Given a set of solutions, it randomly choses one and displays it. This acts as the question.
        /// These will be applied in the same order as the list passed othe constructor.
        void slotSetChoices(const QStringList& texts);
        /// Clears all input widgets.
        void slotClear();
        /// Emits signalAnswer() with the list of answers.
        void slotEmitAnswer();
        /// Shows the solution to the first elementid passed.
        void slotShowHint();
        /// Shows the solutions in all input widgets.
        void slotShowSolution(const QStringList& solutions);
    signals:
        /// Gives the list of user answers.
        void signalAnswer(const QStringList&);
    protected:
        KSvgRenderer * m_renderer;
        QStringList m_elementIds;
        QList<QLineEdit*> m_kids;
        QMap<QString, QLineEdit*> m_map;
        QString m_hint;
        ActiveArea* m_area;
};

#endif
