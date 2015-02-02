/***************************************************************************
    Copyright 2015 Inge Wallin
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// Own
#include "utils.h"

// Qt
//#include <QPaintEvent>
//#include <QWidget>

// KEduVocDocument library
#include <keduvocdocument.h>


// ----------------------------------------------------------------
//                         class WordCount


WordCount::WordCount()
{
    clear();
}


void WordCount::clear()
{
    for (int i = 0; i <= KV_MAX_GRADE; ++i) {
        grades[i] = 0;
        pregrades[i] = 0;
    }
    invalid = 0;

    initialWords = 0;
    totalWords = 0;
}

int WordCount::percentageCompleted() const
{
    // To calculate the percentage done we add:
    //  * 1..KV_MAX_GRADE points for words in the initial phase (grade = 0, pregrade > 0)
    //  * KV_MAX_GRADE * (1..KV_MAX_GRADE) points for words in the long-term phase (grade>0)
    // So the maximum number of points is KV_MAX_GRADE^2 per word.
    // 
    // In the final calculation, we exclude all invalid words from the percentage.
    int points = 0;
    for (int i = 0; i <= KV_MAX_GRADE + 1; ++i) {
        points += pregrades[i] * i;
        points += grades[i] * KV_MAX_GRADE * i;
    }

    if (totalWords - invalid == 0) {
        // Don't divide by 0.
        return 0;
    }
    else {
        return 100 * points / ((totalWords - invalid) * KV_MAX_GRADE * KV_MAX_GRADE);
    }
}


// ----------------------------------------------------------------
//                         class confidenceColors


// FIXME: NYI
//extern QColor gradeColor[11];

