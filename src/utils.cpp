/*
    SPDX-FileCopyrightText: 2015 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


// Own
#include "utils.h"

// Qt
#include <QPainter>
#include <QRect>
#include <QPainterPath>

// KEduVocDocument library
#include <KEduVocDocument>
#include <KEduVocExpression>

// Parley
#include "prefs.h"

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
    for (int i = 0; i < KV_MAX_GRADE + 1; ++i) {
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


void WordCount::fillFromContainer(KEduVocContainer &container, int translationIndex,
                                  KEduVocContainer::EnumEntriesRecursive recursive)
{
    clear();

    foreach (KEduVocExpression *entry, container.entries(recursive)) {
        KEduVocTranslation &translation(*entry->translation(translationIndex));
        evaluateWord(translation, translation.text());
    }
}


void WordCount::fillFromContainerForPracticeMode(KEduVocContainer &container, int translationIndex,
                                                 const QStringList &activeConjugationTenses,
                                                 KEduVocContainer::EnumEntriesRecursive recursive)
{
    KEduVocWordFlags wordTypeToProcess(KEduVocWordFlag::NoInformation);
    switch (Prefs::practiceMode()) {
    case Prefs::EnumPracticeMode::GenderPractice:
        wordTypeToProcess = KEduVocWordFlag::Noun;
        break;
    case Prefs::EnumPracticeMode::ConjugationPractice:
        wordTypeToProcess = KEduVocWordFlag::Verb;
        break;
    case Prefs::EnumPracticeMode::ComparisonPractice:
        wordTypeToProcess = KEduVocWordFlag::Adjective | KEduVocWordFlag::Adverb;
        break;
    default:
        fillFromContainer(container, translationIndex, recursive);
        return;
    }

    clear();

    foreach (KEduVocExpression *entry, container.entries(recursive)) {
        KEduVocTranslation &translation(*entry->translation(translationIndex));
        if (isValidForProcessing(translation, wordTypeToProcess)) {
            switch (wordTypeToProcess) {
            case KEduVocWordFlag::Noun:
                {
                    KEduVocText article = translation.article();
                    evaluateWord(article, translation.text());
                }
                break;
            case KEduVocWordFlag::Verb:
                {
                    QStringList conjugationTenses = translation.conjugationTenses();
                    foreach(const QString &activeTense, activeConjugationTenses)
                    {
                        if (conjugationTenses.contains(activeTense)) {
                            KEduVocConjugation conj = translation.getConjugation(activeTense);
                            foreach (KEduVocWordFlags key, conj.keys()) {
                                KEduVocText person = conj.conjugation(key);
                                evaluateWord(person, person.text());
                            }
                        }
                    }
                }
                break;
            case KEduVocWordFlag::Adjective | KEduVocWordFlag::Adverb:
                {
                    KEduVocText comparative = translation.comparativeForm();
                    evaluateWord(comparative, comparative.text());
                    KEduVocText superlative = translation.superlativeForm();
                    evaluateWord(superlative, superlative.text());
                }
                break;
            }
        }
    }
}

bool WordCount::isValidForProcessing(KEduVocTranslation &trans, KEduVocWordFlags wordType) const
{
    return !trans.isEmpty()
        && (trans.wordType() != nullptr)
        && ((trans.wordType()->wordType() & wordType) != 0);
}

void WordCount::evaluateWord(const KEduVocText &item, const QString &text)
{
    ++totalWords;
    if (text.isEmpty()) {
        ++invalid;
    } else if (item.preGrade() > 0) {
        // Initial phase (we assume correctness, i.e. if pregrade>0 then grade = 0)
        ++initialWords;
        ++pregrades[item.preGrade()];
    } else {
        // Long term or unpracticed
        ++grades[item.grade()];
    }
}


// ----------------------------------------------------------------
//                         class confidenceColors


ConfidenceColors::ConfidenceColors(ColorScheme colorScheme)
{
    initColors(colorScheme);
}


void ConfidenceColors::initColors(ColorScheme colorScheme)
{
    switch (colorScheme) {
    case MultiColorScheme:
    default: // Not default at the last line.  Hope this works...

        longTermColors[0] = QColor(25,38,41);
        //longTermColors[1] = QColor(Qt::yellow);
        longTermColors[1] = QColor(25,38,41,64);
        longTermColors[2] = QColor(237,21,21);
        longTermColors[3] = QColor(246,116,0);
        longTermColors[4] = QColor(201,206,59);
        longTermColors[5] = QColor(28,220,154);
        longTermColors[6] = QColor(17,209,22);
        longTermColors[7] = QColor(61,174,253);

        initialTermColor = QColor(25,38,41,64); // Find something else

        invalidColor = QColor(Qt::red);
        break;

    case ProgressiveColorScheme:
        {
            static const int AlphaMax = 255;
            static const int AlphaStep = ((AlphaMax - 10) / KV_MAX_GRADE);

            QColor color;

            // Confidence 1..max
            for (int grade = 1; grade <= KV_MAX_GRADE; ++grade) {
                color = Prefs::gradeColor();
                color.setAlpha(AlphaMax - (KV_MAX_GRADE - grade) * AlphaStep);

                longTermColors[grade] = color;
            }

            // Unpracticed (confidence 0)
            color = QColor("#FFFFFF");
            color.setAlpha(AlphaMax);
            longTermColors[0] = color;

            // Use one color for all initial phase values
            color = Prefs::preGradeColor();
            color.setAlpha(AlphaMax);
            initialTermColor = color;

            // Invalid
            invalidColor = Prefs::invalidUnitColor();

            break;
        }
    }

    // These two are placeholders for the wordcloud background color.
    frontEndColors[0] = QColor(255,221,217);
    frontEndColors[1] = QColor(238,232,213);
}


// ----------------------------------------------------------------
//                         Various utility functions


void paintColorBar(QPainter &painter, const QRect &rect,
                   const WordCount &wordCount, const ConfidenceColors &colors)
{
    // The outline of the total bar.
    QRectF roundedRect(rect);
    roundedRect.adjust(1.0, 1.0, -1.0, -1.0);

    // Set a rounded clipping region to paint the bar segments in
    QPainterPath clippingPath;
    clippingPath.addRoundedRect(roundedRect, 2.0, 2.0);
    painter.setClipPath(clippingPath);

    qreal xPosition = 0.0;

    // >0: grade, 0: initial, -1: untrained, -2: invalid
    for (int i = KV_MAX_GRADE; i >= -2; --i) {
        qreal fraction;
        QColor color;

        // Get the fraction and the color
        if (i > 0) {
            // long term
            fraction = qreal(wordCount.grades[i]) / qreal(wordCount.totalWords);
            color = colors.longTermColors[i];
        } else if (i == 0) {
            // initial term
            fraction = qreal(wordCount.initialWords) / qreal(wordCount.totalWords);
            color = colors.initialTermColor;
        } else if (i == -1) {
            // untrained (stored in longterm[0])
            fraction = qreal(wordCount.grades[0]) / qreal(wordCount.totalWords);
            color = colors.longTermColors[0];
        } else {
            fraction = qreal(wordCount.invalid) / qreal(wordCount.totalWords);
            color = colors.invalidColor;
        }

        // Create a rect from the current fraction
        qreal barElementWidth = fraction * roundedRect.width();
        QRectF barElement(roundedRect.x() + xPosition, roundedRect.y(), barElementWidth, roundedRect.height());
        xPosition += barElementWidth;

        // Paint!
        painter.setBrush(QBrush(color));
        painter.drawRect(barElement);
    }

    // Draw the outline
    painter.setClipping(false);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(roundedRect, 2.0, 2.0);
}
