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


#ifndef UTILS_H
#define UTILS_H

// Qt
//#include <QPaintEvent>
//#include <QWidget>

// KEduVocDocument library
#include <keduvocdocument.h>


// The WordCount struct contains the number of words in each category.
// This could be used for number of words due, total number of words, etc.
struct WordCount {
    WordCount();
    void clear();
    int  percentageCompleted() const;

    int grades[KV_MAX_GRADE + 1]; // Number of entries in each grade including grade=0, pregrade=0
    int pregrades[KV_MAX_GRADE + 1]; // Number of entries in each grade including grade=0, pregrade=0
    int invalid;	          // Number of invalid entries (not always applicable);

    int initialWords;             // Number of entries in initial phase (grade=0, pregrade>0)
                                  // This is the sum of the numbers in pregrades[].
    int totalWords;		  // Total number of words
                                  // This is the sum of grades[], pregrades[] and invalid
};




//extern QColor gradeColor[11];




#endif // UTILS_H
