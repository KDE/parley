///\file hint.h
// C++ Interface: hint
//
// Description: Displays hints about the prompt and/or answer
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

#ifndef EDU_HINT_H
#define EDU_HINT_H

/**
 * @class Hint
 * @author David Capel <wot.narg@gmail.com>
 * @brief This class provides the mechanism for showing hints about a question.
 * Showing a hint will taint the answer, which makes it count as incorrect, even if correct.
 * This will likely be connected to a "Show Hint" button.
 * Depending on the practice mode, this may or may not be used.
 */
class Hint : public EduWidget
{
    Q_OBJECT

    signals:
        /// Emitted when a hint is show; tainting makes the answer count
        /// as incorrect (even if it is correct); tainting frequency is also tracked
        /// in the statistics module. In the this class, TaintReason will almost always
        /// be HintShown.
        signalAnswerTainted(TaintReason reason);

    public slots:
        /// Implements the hint logic. The logic itself is dictated by the practice mode.
        virtual void slotShowHint();
};





#endif
