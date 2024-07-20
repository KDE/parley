/*
    SPDX-FileCopyrightText: 2021 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "translateshelltest.h"
#include "translateshelladapter.h"
#include <QProcess>
#include <QSignalSpy>

// LC_ALL=C trans -l en -s en -t de homework -show-alternatives=n -show-original=n -show-languages=n -show-original-dictionary=n -no-warn
static const char *s_homework_en_de = R"""(Hausaufgaben

Definitions of homework

noun
    (die) Hausaufgaben
        homework
    (die) Heimarbeit
        homework, outwork
    (die) Hausaufgabe
        homework, homework assignment, assignment
    (die) Hausarbeit
        housework, homework, homework assignment, assignment
    (die) Aufgabe
        task, duty, mission, job, function, homework
)""";

// translation request of untranslatable string
// LC_ALL=C trans -l en -s en -t de FOOBAA -show-alternatives=n -show-original=n -show-languages=n -show-original-dictionary=n -no-warn
static const char *s_foobaa_en_de = R"""(FOOBAA

Translations of FOOBAA
)""";

// translation request of string with with special char, where warning "Did you mean: homework" is skipped with argument
// LC_ALL=C trans -l en -s en -t de FOOBAA -show-alternatives=n -show-original=n -show-languages=n -show-original-dictionary=n -no-warn
static const char *s_homework_special_char_en_de = R"""(_Hausaufgaben

Definitions of _homework

noun
    (die) Hausaufgaben
        homework
    (die) Heimarbeit
        homework, outwork
    (die) Hausaufgabe
        homework, homework assignment, assignment
    (die) Hausarbeit
        housework, homework, homework assignment, assignment
    (die) Aufgabe
        task, duty, mission, job, function, homework
)""";

// LC_ALL=C trans -l en -s en -t de run -show-alternatives=n -show-original=n -show-languages=n -show-original-dictionary=n -no-warn
static const char *s_run_en_de = R"""(Lauf

Definitions of run

noun
    (der) Run
        run
    (der) Lauf
        running, run, course, barrel, race, operation
    (die) Auflage
        edition, circulation, rest, run, overlay, condition
    (die) Fahrt
        ride, trip, journey, tour, run, voyage
    (die) Laufzeit
        term, period of validity, operational time, running time, run-time
    (die) Strecke
        route, distance, track, line, stretch, run
    (die) Serie
        series, serial, set, run, string, succession
    (der) Ansturm
        rush, stampede, onslaught, run, onrush, crowd
    (die) Folge
        episode, result, sequence, consequence, order, succession
    (die) Reihe
        series, row, number, range, set, line
    (die) Sequenz
        sequence, progression, run, flush
    (das) Gehege
        enclosure, reserve, pen, preserve, compound, run
    (der) Hühnerhof
        run
    (die) Masche
        mesh, stitch, ploy, trick, pitch, run
    (die) Laufmasche
        run, ladder
    (die) Spielzeit
        season, playing time, period, run, inning
    (der) Ausflug
        tour, trip, excursion, outing, flight, hike
    (die) Tendenz
        trend, tendency, bias, direction, drift, run
    (der) Flug
        flight

verb
    laufen
        run, walk, go, operate, work, race
    verlaufen
        run, proceed, go off
    starten
        start, launch, take off, set off, power up, blast off
    führen
        lead, run, carry, guide, conduct, keep
    ablaufen
        run, expire, drain off, run out, pass, go off
    fahren
        drive, ride, run, move, pass, motor
    rennen
        race, run, sprint
    durchführen
        lead through, take through, run through, carry out, implement, run
    leiten
        guide, conduct, lead, direct, manage, run
    verkehren
        operate, run, associate, have intercourse, consort, haunt
    auslaufen
        leak, run out, run, drain, leak out, sail
    fließen
        flow, pass, run, move, stream, circulate
    unterhalten
        support, maintain, keep, operate, entertain, amuse
    verwalten
        manage, administer, maintain, govern, hold, conduct
    ausgehen
        go out, go, start, assume, emanate, run out
    rinnen
        run, stream, pour
    halten
        keep, hold, maintain, stop, stick, stay
    steuern
        control, manage, steer, drive, navigate, run
    abwickeln
        unwind, complete, carry out, transact, unroll, uncoil
    schmeißen
        throw, chuck, fling, sling, slam, bung
    zerfließen
        melt away, run
    färben
        color, dye, colour
    verfließen
        pass, run, go by, become blurred
    tropfen
        drop, drip, leak
    einlassen
        admit, let in, embed, set, run, bed in
    tröpfeln
        dribble, trickle, drip, run
    triefen
        drip, ooze, run, water, be dripping wet
    plagen
        plague, afflict, torment, infest, harass, run
)""";

// LC_ALL=C trans -l en -s en -t de nice -show-alternatives=n -show-original=n -show-languages=n -show-original-dictionary=n -no-warn
static const char *s_nice_en_de = R"""(nett

Definitions of nice

adjective
    schön
        beautiful, nice, lovely, good, pretty, fine
    nett
        nice, cute, kind, lovely, pleasant, neat
    gut
        good, fine, nice, solid, beneficial, sharp
    hübsch
        pretty, nice, lovely, fine, neat, comely
    sauber
        clean, cleanly, neat, tidy, nice, fresh
    fein
        fine, delicate, nice, subtle, sensitive, elegant
    lieb
        dear, sweet, kind, nice, good, lovely
    sympathisch
        nice, pleasant, likable, simpatico
    lecker
        delicious, yummy, nice, scrumptious, lovely, savory
    genau
        exact, accurate, precise, close, specific, detailed
    heikel
        delicate, tricky, difficult, awkward, fussy, ticklish
    pingelig
        picky, fussy, finicky, niggling, over-particular, nit-picking
    anspruchsvoll
        demanding, exacting, fastidious, ambitious, discriminating, challenging
)""";

// C_ALL=C trans -l en -s en -t de "this is a dog" -show-alternatives=y -show-original=n -show-languages=n -show-original-dictionary=n -no-warn
static const char *s_sentence_dog_en_de = R"""(das ist ein Hund

Translations of this is a dog

this is a dog
    das ist ein Hund, dies ist ein Hund
)""";

void TranslateShellTest::translationShellOutputParserTest()
{
    {
        TranslateShellAdapter::Translation result = TranslateShellAdapter::parseTranslateShellResult(QString(s_homework_en_de));
        QCOMPARE(result.m_suggestions, QStringList{"Hausaufgaben"});
    }
    {
        TranslateShellAdapter::Translation result = TranslateShellAdapter::parseTranslateShellResult(QString(s_foobaa_en_de));
        QCOMPARE(result.m_suggestions, QStringList{"FOOBAA"});
    }
    {
        TranslateShellAdapter::Translation result = TranslateShellAdapter::parseTranslateShellResult(QString(s_homework_special_char_en_de));
        QCOMPARE(result.m_suggestions, QStringList{"_Hausaufgaben"});
    }
    {
        TranslateShellAdapter::Translation result = TranslateShellAdapter::parseTranslateShellResult(QString(s_run_en_de));
        QCOMPARE(result.m_suggestions, QStringList{"Lauf"});
    }
    {
        TranslateShellAdapter::Translation result = TranslateShellAdapter::parseTranslateShellResult(QString(s_nice_en_de));
        QCOMPARE(result.m_suggestions, QStringList{"nett"});
    }
    {
        TranslateShellAdapter::Translation result = TranslateShellAdapter::parseTranslateShellResult(QString(s_sentence_dog_en_de));
        QCOMPARE(result.m_suggestions, QStringList{"das ist ein Hund"});
    }
}

void TranslateShellTest::translateShellProcessInteractionTest()
{
    // important: we cannot rely on correct answers from the webservice, which may stop answering our requests
    // after a surprisingly small number of API interactions
    QFuture<TranslateShellAdapter::Translation> translation = TranslateShellAdapter::translateAsync("haus", "de", "en");
    translation.waitForFinished();
    if (translation.result().m_suggestions.size() == 0) {
        qWarning("did not receive any translation results");
        return;
    }
    if (translation.result().m_suggestions.first() != "House") { // translation "haus" -> "House" is expected to be stable
        qWarning("translation result differes from expectation");
    }
}

QTEST_GUILESS_MAIN(TranslateShellTest);

#include "moc_translateshelltest.cpp"
