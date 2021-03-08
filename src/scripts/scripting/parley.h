/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTOBJECTPARLEY_H
#define SCRIPTOBJECTPARLEY_H

#include "document.h"

#include <KLocalizedString>

#include <QStandardPaths>
#include <QObject>
#include <QAction>
#include <QDebug>

#include <KEduVocDeclension>

namespace Editor
{
class EditorWindow;
}
class Translator;

/**
 * @namespace Scripting
 * @brief Contains all the classes exposed to Kross scripts.
 *
 * Main entry point is the Scripting::Parley class which allows us to access all the other Scripting classes (see code below).
 *
 * Class Hierarchy:
 *
 * - Parley
 *   - Document
 *     - Lesson (inherits Container)
 *       - Expression (known as Entry in functions)
 *         - Translation (inherits Text)
 *   - Identifier
 *
 *<h3>How to create a new %Parley script (full example)</h3>
 *
 * Each %Parley script must be accompanied by a .desktop file, both put in the plugins folder in parley data directory (usually in  /usr/share/apps/parley/plugins/). In this example the desktop file is "example.desktop" and the script file is "example.py".
 *
 * The desktop file provides information about the script's functionality, author and specifies the script file that implements it. This information will appear in the Scripts Manager (Scripts->Script Manager in main menu) and the user will be able to enable/disable the script. If the script is enabled then it'll be loaded every time Parley starts up.
 *
 * Content of example.desktop (/usr/share/apps/parley/plugins/example.desktop)
 *
@code
[Desktop Entry]
Encoding=UTF-8
Icon=mypluginicon
Type=Service
ServiceTypes=KPluginInfo

Name=Example Parley Script
Comment=This Script offers two actions: 1) Move to new lesson 2) Mark as known for the Scripts menu
Script=example.py

X-KDE-PluginInfo-Author=Avgoustinos Kadis
X-KDE-PluginInfo-Email=avgoustinos.kadis@kdemail.net
X-KDE-PluginInfo-Name=parley_example_script
X-KDE-PluginInfo-Version=1.1
X-KDE-PluginInfo-Website=https://edu.kde.org/parley
X-KDE-PluginInfo-Category=examples
X-KDE-PluginInfo-License=GPL
X-KDE-PluginInfo-EnabledByDefault=true
@endcode
 *
 * The script file will contain the functions (script functionality) and some code for creating an action for the Scripts menu. In the example below, the functions are moveSelectedToNewLesson() and markAsKnown() and they are called by the two actions that are added to the scripts menu (action1 and action2).
 *
 * When the script file is loaded all the global code (outside of any function) is executed. This way we can register the Scripts menu actions and connect them with script functions (see example below for how to do this).
 *
 * Another way to have a function called is by connecting it to a %Parley signal (see Parley::translateWord() and the example in Detailed Description of the Parley class).
 *
 * Content of example.py (/usr/share/apps/parley/plugins/example.py)
@code
#!/usr/bin/env kross
import Parley

#FUNCTIONS

#moves selected entries to a new lesson
def moveSelectedToNewLesson():
  selected_entries = Parley.selectedEntries()
  if len(selected_entries) > 0:
    newlesson = Parley.doc.appendNewLesson("New Lesson")
    for entry in selected_entries:
      newlesson.appendEntry(entry)

#marks the selected translations as known (grade 7)
def markAsKnown():
  for tr in Parley.selectedTranslations():
    tr.grade = 7

#SCRIPT MENU

#create a new action for the Scripts menu (action1)
action1 = Parley.newAction("example_action1","Move to new lesson")
action1.statusTip="Moves the selected rows to a new lesson"
Parley.connect(action1,"triggered()",moveSelectedToNewLesson)


#create a new action for the Scripts menu (action2)
action2 = Parley.newAction("example_action2","Mark as known (highest level)")
action2.statusTip="Sets the confidence level selected translations to 7 (highest level)"
Parley.connect(action2,"triggered()",markAsKnown)
@endcode
 *
 * After creating the Script action and being able to call a function you can start adding your own code, using the documentation provided for the scripting classes and by seeing other examples.
 *
 * For debugging use the standard output and observed it in the terminal.
 *
 */
namespace Scripting
{


/**
 * @class Parley
 * @brief Parley scripting class (main entry point of a %Parley Kross script)
 *
 * Parley class is the main entry point of Parley scripting classes. Through it you can access the Document class (Parley.doc or Parley.document) which provides functionality for viewing/modifying a %Parley document (%KEduVocDocument), that means access lessons, entries, document languages etc.
 *
 * The Parley class has to do more with the active %Parley application. Here it follows a list of possible usages of Parley class:
 *
 * - Add a new Action to the script menu (see Parley::newAction() function)
 * - Add a new translation script (see Parley::translateWord() signal)
 * - Have direct access to the active lesson (see Parley::activeLesson property)
 * - Have access to various enumerations (see Parley::Number, Parley::Case, Parley::Person, Parley::Gender and Parley::Definiteness enumerations)
 * - Create a new %Parley %Document (see Parley::newDocument() function)
 *
 * Signals and Slots: To connect a script function (slot) to a signal you <b> just define a function with the same name as the signal </b> or use the Parley.connect function:
 * @code
 * #how to connect a function to a signal (example with Parley::translateWord() signal)
 * def translateFromInternet(word,fromLang,toLang):
 *     print "Translating from Internet!!.."
 *
 * Parley.connect("translateWord(const QString &,const QString &,const QString &)",translateFromInternet)
 * @endcode
 *
 *   @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class Parley : public QObject
{
    Q_OBJECT
    /// Read-only property of the active document
    Q_PROPERTY(QObject * document READ getDocument)
    /// Abreviation of document property (same as Parley.document)
    Q_PROPERTY(QObject * doc READ getDocument)
    /// Currently active lesson
    Q_PROPERTY(QObject * activeLesson READ activeLesson)

//             Q_ENUMS ( Number Case Person Gender Definiteness )
    Q_ENUMS(Flags)

public:

    enum Flags {
        // This is used for both empty flags and to denote no flags of the correct type were set.
        NoInformation = 0x0,

        // Gender
        Masculine = 0x1,
        Feminine = 0x2,
        Neuter = 0x4,

        // Plurality
        Singular = 0x10,
        Dual = 0x20,
        Plural = 0x40,

        // Part of Speech
        Verb = 0x100,
        Noun = 0x200,
        Pronoun = 0x400,
        Adjective = 0x800,
        Adverb = 0x1000,
        Article = 0x2000,
        Conjunction = 0x4000,

        // Person
        First = 0x10000,
        Second = 0x20000,
        Third = 0x40000,

        // Declension Case
        Nominative = 0x80000,
        Genitive = 0x100000,
        Dative = 0x200000,
        Accusative = 0x400000,
        Ablative = 0x800000,
        Locative = 0x1000000,
        Vocative = 0x2000000,

        // Other assorted flags
        Definite = 0x4000000, // The article is definite
        Indefinite = 0x8000000, // The article is indefinite
        Regular = 0x10000000,
        Irregular = 0x20000000
    };

    explicit Parley(Editor::EditorWindow * editor);

    ~Parley();

    /*
     * Emits translateWord signal so the script function connected to it translate the @p word
     * @param word Word to translate
     * @param fromLanguage The language of @p word to translate from
     * @param toLanguage The language you want to translate to
     */
    void callTranslateWord(const QString & word, const QString& fromLanguage, const QString& toLanguage);

    //sets the translator object
//             void setTranslator ( Translator* translator ) { m_translator = translator; }
    Translator * translator() {
        return m_translator;
    }

    //Returns the active document
    QObject* getDocument() {
        return m_doc;
    }

    //Returns the active lesson
    QObject* activeLesson();

public Q_SLOTS:

    QStringList dataDirs() {
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    }

    QStringList pluginDirs() {
        QStringList basedirs(QStandardPaths::standardLocations(QStandardPaths::DataLocation));
        QStringList ret;
        foreach ( const QString dir , basedirs){
            ret << (dir + "/plugins");
        }

        return ret;
    }

    /** Returns a list of Expression objects (the selected entries of the active lesson) */
    QVariantList selectedEntries();

    /** Returns a list of Translation objects (the selected translations of the active lesson) */
    QVariantList selectedTranslations();

    /**
     * Adds the found @p translation of the @p word from language @p fromLanguage to language @p toLanguage to %Parley translations to be used for translating lesson entries (or anything else). This function is ment to be used by scripts to add a translation of a word by parsing either online or offline dictionaries.
     *
     * @code
     * #example usage of addTranslation function
     * import Parley
     * #function called by Parley whenever a translation of a word is needed
     * def translateWord(word,fromLang,toLang):
     *     <<look for the word translation>>
     *     Parley.addTranslation(word,fromLang,toLang,foundWord)
     *     <<look for more translations>>
     * @endcode
     *
     * @param word Translated word
     * @param fromLanguage From language
     * @param toLanguage To language
     * @param translation %Translation of word
     */
    void addTranslation(const QString &word, const QString &fromLanguage, const QString &toLanguage, const QString &translation);
    /**
     * Returns a list of all available locales (to be used by the scripts)
     * @return
     */
    QStringList locales();

    /**
     * Gives the language name of the given @p locale
     * @param locale Language locale
     * @return Language name
     */
    QString localeName(const QString &locale);

    /// @todo Make this function working (not very important function)
    /*
     * Open the Parley Document @p file
     * Usage:
     * @code
     * import Parley
     * Parley.open("Vocab/MyVocab.kvtml")
     * @endcode
     * @param file Parley Document file path (ex. /home/kde-user/MyVocab.kvtml)
     */
    void open(const QString &filename);

    /**
     * Creates a new document and returns a reference to it
     *
     * @code
     * #how to create a new document, add lessons, add entries and save it to a kvtml file
     * import Parley
     *
     * #create new document
     * doc = Parley.newDocument()
     * doc.title = "New document"
     *
     * #set identifiers
     * doc.appendNewIdentifier("English","en_US")
     * doc.appendNewIdentifier("French","fr")
     *
     * #lessons
     * l1 = doc.newLesson("Lesson1")
     * doc.rootLesson.appendChildLesson(l1)
     *
     * #add a new entry (first way)
     * e = l1.newEntry()
     * e.setTranslation(0,"dog")
     * e.setTranslation(1,"chien")
     * l1.appendEntry(e)
     *
     * #add a new entry (second way)
     * ee = l1.newEntry(["glass","verre"])
     * l1.appendEntry(ee)
     * #third way
     * ee = l1.appendNewEntry(["book","livre"])
     *
     * #add a new lesson (fast way)
     * l2 = doc.appendNewLesson("Lesson 2")
     *
     * #add a new child lesson under "Lesson 2"
     * l3 = doc.appendNewLesson("Lesson 3",l2)
     *
     * #add a new entry (third way)
     * l2.appendNewEntry(["I","je"]);
     * l3.appendNewEntry(["good morning","bonjour"])
     *
     * #save document
     * doc.saveAs("/home/kde-devel/test_new_document.kvtml")
     * @endcode
     *
     * @return A Document object, the newly created lesson
     */
    QObject * newDocument() {
        return new Document();
    }

    /**
     * Creates and adds to the Scripts menu a new QAction (see QAction documentation)
     *
     * @code
     * #how to add two new Scripts menu entries
     * import Parley
     *
     * def convertLessonToPDF():
     *     print "Converting lesson to PDF.."
     *
     * def convertLessonToHTML():
     *     print "Converting lesson to HTML.."
     *
     * #one way of creating a new action
     * newaction_pdf = Parley.newAction("convert_lesson_pdf")
     * newaction_pdf.text="Convert Lesson to PDF"
     * Parley.connect(newaction_pdf,"triggered()",convertLessonToPDF)
     *
     * #second way of creating a new action (short)
     * newaction_html = Parley.newAction("convert_lesson_html","Convert Lesson to HTML")
     * Parley.connect(newaction_html,"triggered()",convertLessonToHTML)
     * @endcode
     *
     * @param name Unique action name
     * @param text Action's text (what text appears in the scripts menu)
     * @return A reference to a QAction object (accessible by scripts)
     */
    QObject * newAction(const QString & name, const QString & text = QString());

Q_SIGNALS:
    /**
     * Slots (script functions) connected to this signal are called when a translation of @p word is requested. Note that a script function with the same name as a signal will be automatically connected to that signal when the script is activated.
     *
     * @code
     * #example usage of translateWord signal
     * import Parley
     * #function called by Parley whenever a translation of a word is needed
     * def translateWord(word,fromLang,toLang):
     *     <<look for the word translation>>
     *     Parley.addTranslation(word,fromLang,toLang,foundWord)
     *     <<look for more translations>>
     * @endcode
     *
     * @param word Word to translate
     * @param fromLanguage The language of @p word to translate from
     * @param toLanguage The language you want to translate to
     */
    void translateWord(const QString & word, const QString& fromLanguage, const QString& toLanguage);
    /* Emits when the translation of a word is finished (from all the scripts) [not to be used by scripts] */
    void translationFinished(const QString & word, const QString& fromLanguage, const QString& toLanguage);
    /* Emits when the translation of a word starts [not to be used by scripts] */
    void translationStarted(const QString & word, const QString& fromLanguage, const QString& toLanguage);

private:
    Translator* m_translator;
    Document* m_doc;
    Editor::EditorWindow * m_editor;
};

}
#endif
