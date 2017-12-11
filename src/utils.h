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
#include <QtGui>

// KEduVocDocument library
#include <keduvoccontainer.h>
#include <KEduVocWordtype>


class QPainter;
class QRect;
class KEduVocTranslation;

// The WordCount struct contains the number of words in each category.
// This could be used for number of words due, total number of words, etc.
struct WordCount {
    WordCount();
    void clear();
    int  percentageCompleted() const;

    // Fill the WordCount data from the container.
    void fillFromContainer(KEduVocContainer &container, int translationIndex,
                           KEduVocContainer::EnumEntriesRecursive recursive = KEduVocContainer::Recursive);

    // Fill the WordCount data from the container for the selected practice mode.
    void fillFromContainerForPracticeMode(KEduVocContainer &container, int translationIndex,
                    const QStringList &activeConjugationTenses,
                    KEduVocContainer::EnumEntriesRecursive recursive = KEduVocContainer::Recursive);

    int grades[KV_MAX_GRADE + 1]; // Number of entries in each grade including grade=0, pregrade=0
    int pregrades[KV_MAX_GRADE + 1]; // Number of entries in each grade including grade=0, pregrade=0
    int invalid;	          // Number of invalid entries (not always applicable);

    int initialWords;             // Number of entries in initial phase (grade=0, pregrade>0)
                                  // This is the sum of the numbers in pregrades[].
    int totalWords;		  // Total number of words
                                  // This is the sum of grades[], pregrades[] and invalid

private:
    bool isValidForProcessing(KEduVocTranslation &trans, KEduVocWordFlags wordType) const;
    void evaluateWord(const KEduVocText &item, const QString &text);
};


struct ConfidenceColors {
    enum ColorScheme {
        MultiColorScheme,       //< The color scheme from the dashboard
        ProgressiveColorScheme  //< The color scheme from the practice component
    };

    ConfidenceColors(ColorScheme colorScheme = MultiColorScheme);

    void initColors(ColorScheme colorScheme);

    QColor longTermColors[KV_MAX_GRADE + 1];
    QColor initialTermColor;
    QColor invalidColor;
    QColor frontEndColors[2]; // Placeholders for the wordcloud background colors
};


void paintColorBar(QPainter &painter, const QRect &rect,
                   const WordCount &wordCount, const ConfidenceColors &colors);


#endif // UTILS_H
