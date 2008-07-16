/***************************************************************************

                      main part of parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "parley.h"

#include "../config-parley.h"

#include "practiceold/testentrymanager.h"
#include "practice/parleypracticemainwindow.h"

#include "vocabulary/vocabularymodel.h"
#include "vocabulary/vocabularyview.h"
#include "vocabulary/vocabularyfilter.h"
#include "vocabulary/containerview.h"
#include "vocabulary/lessonview.h"
#include "vocabulary/wordtypeview.h"
#include "vocabulary/containermodel.h"
#include "vocabulary/lessonmodel.h"
#include "vocabulary/wordtypemodel.h"
#include "vocabulary/leitnerview.h"
#include "vocabulary/leitnermodel.h"

#include "entry-dialogs/multiplechoicewidget.h"
#include "entry-dialogs/comparisonwidget.h"
#include "entry-dialogs/conjugationwidget.h"
#include "entry-dialogs/declensionwidget.h"
#include "entry-dialogs/imagechooserwidget.h"
#include "entry-dialogs/audiowidget.h"
#include "entry-dialogs/browserwidget.h"
#include "entry-dialogs/synonymwidget.h"

#include "statistics-dialogs/StatisticsDialog.h"
#include "settings/parleyprefs.h"
#include "language-dialogs/languagedialog.h"
#include "language-dialogs/grammardialog.h"
#include "docprop-dialogs/TitlePage.h"
#include "configure-practice/configurepracticedialog.h"
#include "prefs.h"

#include "scripts/scriptdialog.h"
#include "scripts/scriptobjectparley.h"
#include "scripts/scriptobjectentry.h"

#include <keduvoclesson.h>
#include <keduvocexpression.h>

#include <KLineEdit>
#include <KStatusBar>

#include <KActionCollection>
#include <KRecentFilesAction>
#include <KToggleAction>
#include <KActionMenu>
#include <KMenuBar>
#include <KMessageBox>
#include <KTipDialog>
#include <KCharSelect>
#include <knewstuff2/ui/knewstuffaction.h>

#include <QtCore/QTimer>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>

#include "modeltest/modeltest.h"


ParleyApp::ParleyApp(const QString& appName, const KUrl & filename) : KXmlGuiWindow(0)
{
    m_appName = appName;
    m_document = new ParleyDocument(this);
    m_searchLine = 0;
    m_searchWidget = 0;
    m_pronunciationStatusBarLabel = 0;
    m_remarkStatusBarLabel = 0;
    m_typeStatusBarLabel = 0;
    m_conjugationWidget = 0;

    m_entryDlg = 0;

    initStatusBar();

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    initView();
    initModel();

    initDockWidgets();

    initActions();

    // these connects need the model to exist
//     QAction * actionRestoreNativeOrder = actionCollection()->action("restore_native_order");
//     m_tableView->horizontalHeader()->addAction(actionRestoreNativeOrder);
//     connect(actionRestoreNativeOrder, SIGNAL(triggered()), m_sortFilterModel, SLOT(restoreNativeOrder()));

    if ( !filename.url().isEmpty() ) {
        kDebug() << "open doc" << filename.url();
        m_document->open(filename);
        kDebug() << "open done";
    } else {
        if (m_recentFilesAction->actions().count() > 0
            && m_recentFilesAction->action(
                                        m_recentFilesAction->actions().count()-1)->isEnabled() )
        {
            m_recentFilesAction->action(m_recentFilesAction->actions().count()-1)->trigger();
        } else {
        kDebug() << "new doc";
            // this is probably the first time we start.
            m_document->newDocument();
            updateDocument();
        }
    }


    if (!initialGeometrySet()) {
        resize(QSize(800, 600).expandedTo(minimumSizeHint()));
    }
    setupGUI(ToolBar | Keys | StatusBar | Create);

    // save position of dock windows etc
    setAutoSaveSettings();

    initScripts();

    // finally show tip-of-day ( if the user wants it :) )
    QTimer::singleShot( 0, this, SLOT( startupTipOfDay() ) );
}


ParleyApp::~ParleyApp()
{
}


void ParleyApp::saveOptions()
{
    m_recentFilesAction->saveEntries(KGlobal::config()->group("Recent Files"));
    m_downloadedFilesAction->saveEntries(KGlobal::config()->group("Downloaded Files"));

///@todo save selection per document
//     if (m_tableView) {
//         // map to original entry numbers:
//         QModelIndex sourceIndex = m_sortFilterModel->mapToSource(m_tableView->currentIndex());
//         Prefs::setCurrentRow(sourceIndex.row());
//         Prefs::setCurrentCol(sourceIndex.column());
//     }
    Prefs::self()->writeConfig();
}

void ParleyApp::slotGrammarDialog()
{
    // the dialog to set tenses, personal pronouns and articles
    GrammarDialog dialog(m_document->document(), this);

    if (dialog.exec() == KDialog::Accepted) {
        m_document->document()->setModified();
        // update the list of tenses
        m_conjugationWidget->setDocument(m_document->document());
        // FIXME: update articles/pronouns (?)
//         slotStatusMsg(IDS_DEFAULT);
    }
}

void ParleyApp::slotUpdateWindowCaption()
{
    setCaption(m_document->document()->title(), m_document->document()->isModified());
//     slotStatusMsg(IDS_DEFAULT);
}

void ParleyApp::slotShowStatistics()
{
    StatisticsDialog statisticsDialog(m_document->document(), this);
    statisticsDialog.exec();
}

void ParleyApp::slotGeneralOptions()
{
    ParleyPrefs* dialog = new ParleyPrefs(m_document->document(), this, "settings",  Prefs::self());
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotApplyPreferences()));
    dialog->show();
}


void ParleyApp::slotApplyPreferences()
{
    m_document->enableAutoBackup(Prefs::autoBackup());

    if (m_pronunciationStatusBarLabel) {
        m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
    }

    m_vocabularyView->setFont(Prefs::tableFont());
    m_vocabularyView->reset();
}


// void ParleyApp::slotStatusHelpMsg(const QString &text)
// {
//     ///////////////////////////////////////////////////////////////////
//     // change status message of whole statusbar temporary (text, msec)
//     if (pbar == 0 || !pbar->isVisible()) {
//         statusBar()->showMessage(text, 3000);
//     }
// }




void ParleyApp::slotConfigShowSearch()
{
    if (m_searchWidget) {
        m_searchWidget->setVisible(m_searchWidget->isHidden());
        Prefs::setShowSearch(m_searchWidget->isVisible());
    }
}

void ParleyApp::slotEditLanguages()
{
    LanguageDialog* languageDialog = new LanguageDialog(m_document->document(), this);
    ///@todo
    // if this becomes a KConfigDialog: connect(languageDialog, SIGNAL(settingsChanged(const QString&)), m_tableModel, SLOT(loadLanguageSettings()));
    if ( languageDialog->exec() == KDialog::Accepted ) {
         m_vocabularyModel->resetLanguages();
    }
}

void ParleyApp::slotDocumentProperties()
{
    TitlePage* titleAuthorWidget = new TitlePage(m_document->document(), this);
    KDialog* titleAuthorDialog;
    titleAuthorDialog = new KDialog(this);
    titleAuthorDialog->setMainWidget( titleAuthorWidget );
    titleAuthorDialog->setCaption(i18nc("@title:window document properties", "Properties for %1", m_document->document()->url().url()));
    if ( titleAuthorDialog->exec() == KDialog::Accepted ) {
        titleAuthorWidget->commitData();
    }
    slotUpdateWindowCaption();
    delete titleAuthorDialog;
}

void ParleyApp::configurePractice()
{
    ConfigurePracticeDialog* configurePracticeDialog;
    configurePracticeDialog = new ConfigurePracticeDialog(m_document->document(), this, "practice settings",  Prefs::self());

    if ( configurePracticeDialog->exec() == ConfigurePracticeDialog::StartPractice ) {
        startPractice();
    }
}

void ParleyApp::startPractice()
{
    if (Prefs::oldPractice()) {
        hide();
        TestEntryManager testManager(m_document->document(), this);
        testManager.startPractice();
        show();
    } else {
        hide();
        ParleyPracticeMainWindow* window = new ParleyPracticeMainWindow(m_document->document(), this);
        connect(window, SIGNAL(signalPracticeFinished()), this, SLOT(show()));
        window->show();
    }
}


void ParleyApp::slotConfigOldPractice(bool old)
{
kDebug() << "slot config old practice";
    Prefs::setOldPractice(old);
}

bool ParleyApp::queryClose()
{
    bool erg = queryExit();
    if (erg)
        m_document->document()->setModified(false);  // avoid double query on exit via system menu
    return erg;
}


bool ParleyApp::queryExit()
{
    saveOptions();
    if (!m_document->document()->isModified()) {
        return true;
    }

    bool save = Prefs::autoSave(); //save without asking

    if (!save) {
        int exit = KMessageBox::warningYesNoCancel(this, i18n("Vocabulary is modified.\n\nSave file before exit?\n"),
                "", KStandardGuiItem::save(), KStandardGuiItem::discard());
        if (exit == KMessageBox::Yes) {
            save = true;   // save and exit
        } else if (exit == KMessageBox::No) {
            save = false;  // don't save but exit
        } else {
            return false;  // continue work
        }
    }

    if (save) {
        m_document->save();       // save and exit
    }
    return true;
}


void ParleyApp::updateDocument()
{
///@todo we can use connect here
    m_vocabularyModel->setDocument(m_document->document());

    m_lessonModel->setDocument(m_document->document());
    m_wordTypeModel->setDocument(m_document->document());
    m_leitnerModel->setDocument(m_document->document());

    // expand the root items
    m_lessonView->expandToDepth(0);
    m_wordTypeView->expandToDepth(0);

    // the top level container of this model only holds the others
    m_leitnerView->setRootIndex(m_leitnerModel->index(0,0));

    connect(m_document->document(), SIGNAL(docModified(bool)), this, SLOT(slotUpdateWindowCaption()));
//     connect(m_lessonModel,SIGNAL(documentModified()),this,SLOT(slotUpdateWindowCaption()));
    connect(m_vocabularyModel, SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyView, SLOT(slotRestoreColumnVisibility(KEduVocDocument*)));

    setCaption(m_document->document()->url().fileName(), false);

///@todo remove this!
// at the moment creates a new test every time a model is created. this is good because we get the basic sanity check then.
    new ModelTest(m_vocabularyModel, this);
}


void ParleyApp::initDockWidgets()
{
// Lesson dock
    QDockWidget *lessonDockWidget = new QDockWidget(i18n("Lessons"), this);
    lessonDockWidget->setObjectName("LessonDock");
    m_lessonView = new LessonView(this);
    lessonDockWidget->setWidget(m_lessonView);
    addDockWidget(Qt::LeftDockWidgetArea, lessonDockWidget);
    actionCollection()->addAction("show_lesson_dock", lessonDockWidget->toggleViewAction());

    m_lessonModel = new LessonModel(this);
///@todo remove before release
    new ModelTest(m_lessonModel, this);

    m_lessonView->setModel(m_lessonModel);
    m_lessonView->setToolTip(i18n("Right click to add, delete, or rename lessons. \n"
            "With the checkboxes you can select which lessons you want to practice. \n"
            "Only checked lessons [x] will be asked in the tests!"));

    connect(m_lessonView, SIGNAL(selectedLessonChanged(KEduVocLesson*)),
        m_vocabularyModel, SLOT(setLesson(KEduVocLesson*)));

    connect(m_lessonView, SIGNAL(signalShowContainer(KEduVocContainer*)),
        m_vocabularyModel, SLOT(showContainer(KEduVocContainer*)));

    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_lessonView, SLOT(setTranslation(KEduVocExpression*, int)));


// Word types dock
    QDockWidget* wordTypeDockWidget = new QDockWidget(i18n("Word Types"), this);
    wordTypeDockWidget->setObjectName( "WordTypeDock" );
    m_wordTypeView = new WordTypeView(this);
    wordTypeDockWidget->setWidget(m_wordTypeView);
    addDockWidget( Qt::LeftDockWidgetArea, wordTypeDockWidget );

    m_wordTypeModel = new WordTypeModel(this);
    wordTypeDockWidget->setVisible(false);
    actionCollection()->addAction("show_wordtype_dock", wordTypeDockWidget->toggleViewAction());

///@todo test, should be fixed with the lesson one though
///@todo remove before release
//     new ModelTest(m_wordTypeModel, this);

    m_wordTypeView->setModel(m_wordTypeModel);

    connect(m_wordTypeView, SIGNAL(selectedWordTypeChanged(KEduVocWordType*)),
        m_vocabularyModel, SLOT(setWordType(KEduVocWordType*)));

    connect(m_wordTypeView, SIGNAL(signalShowContainer(KEduVocContainer*)),
        m_vocabularyModel, SLOT(showContainer(KEduVocContainer*)));

    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_wordTypeView, SLOT(setTranslation(KEduVocExpression*, int)));

// Leitner boxes dock
    QDockWidget* leitnerDockWidget = new QDockWidget(i18n("Grade Boxes"), this);
    leitnerDockWidget->setObjectName( "LeitnerDock" );
    m_leitnerView = new LeitnerView(this);
    leitnerDockWidget->setWidget(m_leitnerView);
    addDockWidget( Qt::LeftDockWidgetArea, leitnerDockWidget );

    m_leitnerModel = new LeitnerModel(this);
    leitnerDockWidget->setVisible(false);
    actionCollection()->addAction("show_leitner_dock", leitnerDockWidget->toggleViewAction());

    m_leitnerView->setModel(m_leitnerModel);

    connect(m_leitnerView, SIGNAL(selectedLeitnerBoxChanged(KEduVocLeitnerBox*)),
        m_vocabularyModel, SLOT(setLeitnerBox(KEduVocLeitnerBox*)));

    connect(m_leitnerView, SIGNAL(signalShowContainer(KEduVocContainer*)),
        m_vocabularyModel, SLOT(showContainer(KEduVocContainer*)));

    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_leitnerView, SLOT(setTranslation(KEduVocExpression*, int)));

// Conjugations
    QDockWidget *conjugationDock = new QDockWidget(i18n("Conjugation"), this);
    conjugationDock->setObjectName("ConjugationDock");
    m_conjugationWidget = new ConjugationWidget(this);
    conjugationDock->setWidget(m_conjugationWidget);
    addDockWidget(Qt::RightDockWidgetArea, conjugationDock);
    conjugationDock->setVisible(false);
    actionCollection()->addAction("show_conjugation_dock", conjugationDock->toggleViewAction());
    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)),
        m_conjugationWidget, SLOT(setDocument(KEduVocDocument*)));
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_conjugationWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Declensions
    QDockWidget *declensionDock = new QDockWidget(i18n("Declension"), this);
    declensionDock->setObjectName("DeclensionDock");
    DeclensionWidget *declensionWidget = new DeclensionWidget(this);
    declensionDock->setWidget(declensionWidget);
    addDockWidget(Qt::RightDockWidgetArea, declensionDock);
    actionCollection()->addAction("show_declension_dock", declensionDock->toggleViewAction());
    declensionDock->setVisible(false);
    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)),
            declensionWidget, SLOT(setDocument(KEduVocDocument*)));
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            declensionWidget, SLOT(setTranslation(KEduVocExpression*, int)));


// Comparison forms
    QDockWidget *comparisonDock = new QDockWidget(i18n("Comparison forms"), this);
    comparisonDock->setObjectName("ComparisonDock");
    ComparisonWidget *comparisonWidget = new ComparisonWidget(this);
    comparisonDock->setWidget(comparisonWidget);
    addDockWidget(Qt::RightDockWidgetArea, comparisonDock);
    actionCollection()->addAction("show_comparison_dock", comparisonDock->toggleViewAction());
    comparisonDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        comparisonWidget, SLOT(setTranslation(KEduVocExpression*, int)));
    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)), comparisonWidget, SLOT(setDocument(KEduVocDocument*)));


// Multiple choice
    QDockWidget *multipleChoiceDock = new QDockWidget(i18n("Multiple Choice"), this);
    multipleChoiceDock->setObjectName("MultipleChoiceDock");
    MultipleChoiceWidget *multipleChoiceWidget = new MultipleChoiceWidget(this);
    multipleChoiceDock->setWidget(multipleChoiceWidget);
    addDockWidget(Qt::RightDockWidgetArea, multipleChoiceDock);
    actionCollection()->addAction("show_multiplechoice_dock", multipleChoiceDock->toggleViewAction());
    multipleChoiceDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        multipleChoiceWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Synonym (and the same for antonym and false friends)
    QDockWidget *synonymDock = new QDockWidget(i18n("Synonyms"), this);
    synonymDock->setObjectName("SynonymDock");
    SynonymWidget *synonymWidget = new SynonymWidget(SynonymWidget::Synonym, this);
    synonymDock->setWidget(synonymWidget);
    addDockWidget(Qt::RightDockWidgetArea, synonymDock);
    actionCollection()->addAction("show_synonym_dock", synonymDock->toggleViewAction());
    synonymDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            synonymWidget, SLOT(setTranslation(KEduVocExpression*, int)));

    QDockWidget *antonymDock = new QDockWidget(i18n("Antonyms"), this);
    antonymDock->setObjectName("AntonymDock");
    SynonymWidget *antonymWidget = new SynonymWidget(SynonymWidget::Antonym, this);
    antonymDock->setWidget(antonymWidget);
    addDockWidget(Qt::RightDockWidgetArea, antonymDock);
    actionCollection()->addAction("show_antonym_dock", antonymDock->toggleViewAction());
    antonymDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            antonymWidget, SLOT(setTranslation(KEduVocExpression*, int)));

    QDockWidget *falseFriendDock = new QDockWidget(i18n("False Friends"), this);
    falseFriendDock->setObjectName("FalseFriendDock");
    SynonymWidget *falseFriendWidget = new SynonymWidget(SynonymWidget::FalseFriend, this);
    falseFriendDock->setWidget(falseFriendWidget);
    addDockWidget(Qt::RightDockWidgetArea, falseFriendDock);
    actionCollection()->addAction("show_falsefriend_dock", falseFriendDock->toggleViewAction());
    falseFriendDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            falseFriendWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Pronunciation symbols - Use KCharSelect
    QDockWidget *charSelectDock = new QDockWidget(i18n("Phonetic Symbols"), this);
    charSelectDock->setObjectName("IPADock");
    KCharSelect *charSelectWidget = new KCharSelect(this,  KCharSelect::SearchLine | KCharSelect::BlockCombos | KCharSelect::CharacterTable);
    charSelectWidget->setCurrentChar(0x0250);
    charSelectDock->setWidget(charSelectWidget);
    addDockWidget(Qt::BottomDockWidgetArea, charSelectDock);
    actionCollection()->addAction("show_pronunciation_dock", charSelectDock->toggleViewAction());
    charSelectDock->setVisible(false);
    connect(charSelectWidget, SIGNAL(charSelected(const QChar &)), m_vocabularyView, SLOT(appendChar(const QChar &)));

// Image
    QDockWidget *imageDock = new QDockWidget(i18n("Image"), this);
    imageDock->setObjectName("ImageDock");
    ImageChooserWidget *imageChooserWidget = new ImageChooserWidget(this);
    imageDock->setWidget(imageChooserWidget);
    addDockWidget(Qt::RightDockWidgetArea, imageDock);
    actionCollection()->addAction("show_image_dock", imageDock->toggleViewAction());
    imageDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        imageChooserWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Sound
    QDockWidget *audioDock = new QDockWidget(i18n("Sound"), this);
    audioDock->setObjectName("AudioDock");
    AudioWidget *audioWidget = new AudioWidget(this);
    audioDock->setWidget(audioWidget);
    addDockWidget(Qt::RightDockWidgetArea, audioDock);
    actionCollection()->addAction("show_audio_dock", audioDock->toggleViewAction());
    audioDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
        audioWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// browser
    QDockWidget *browserDock = new QDockWidget(i18n("Internet"), this);
    browserDock->setObjectName("BrowserDock");
    //TinyWebBrowser *browserWidget = new TinyWebBrowser(this);
    BrowserWidget *htmlPart = new BrowserWidget(browserDock);
    browserDock->setWidget(htmlPart);
    addDockWidget(Qt::RightDockWidgetArea, browserDock);
    actionCollection()->addAction("show_browser_dock", browserDock->toggleViewAction());
    browserDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            htmlPart, SLOT(setTranslation(KEduVocExpression*, int)));

// Marble
//     QDockWidget *marbleDock = new QDockWidget(i18n("Marble"), this);
//     marbleDock->setObjectName("MarbleDock");
//     MarbleWidget *marbleWidget = new MarbleWidget(this);
//     marbleDock->setWidget(marbleWidget);
//     addDockWidget(Qt::RightDockWidgetArea, marbleDock);


// Grades
//     QDockWidget *gradeDock = new QDockWidget(i18n("Grade"), this);
//     gradeDock->setObjectName("gradeDock");
//     QLabel *gradeWidget = new QLabel("grade placeholder", this);
//     gradeDock->setWidget(gradeWidget);
//     addDockWidget(Qt::RightDockWidgetArea, gradeDock);
//     connect(this, SIGNAL(signalSetData(KEduVocTranslation*)), m_declensionWidget, SLOT(setTranslation(KEduVocTranslation*)));

// actionCollection()->addAction("show_leitner_dock", ->toggleViewAction());
}

void ParleyApp::tipOfDay() {
  KTipDialog::showTip(this, "parley/tips", true);
}

void ParleyApp::startupTipOfDay() {
  KTipDialog::showTip(this, "parley/tips");
}


void ParleyApp::initActions()
{
// -- FILE --------------------------------------------------
    KAction* fileNew = KStandardAction::openNew(m_document, SLOT(slotFileNew()), actionCollection());
    fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
    fileNew->setToolTip(fileNew->whatsThis());
    fileNew->setStatusTip(fileNew->whatsThis());

    KAction* fileOpen = KStandardAction::open(m_document, SLOT(slotFileOpen()), actionCollection());
    fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
    fileOpen->setToolTip(fileOpen->whatsThis());
    fileOpen->setStatusTip(fileOpen->whatsThis());

    KAction* fileOpenExample = new KAction(this);
    actionCollection()->addAction("file_open_example", fileOpenExample);
    fileOpenExample->setIcon(KIcon("document-open"));
    fileOpenExample->setText(i18n("Open &Example..."));
    connect(fileOpenExample, SIGNAL(triggered(bool)), m_document, SLOT(openExample()));
    fileOpenExample->setWhatsThis(i18n("Open an example vocabulary document"));
    fileOpenExample->setToolTip(fileOpenExample->whatsThis());
    fileOpenExample->setStatusTip(fileOpenExample->whatsThis());

    KAction* fileGHNS = KNS::standardAction(i18n("Download New Vocabularies..."), m_document, SLOT(slotGHNS()), actionCollection(), "file_ghns");
    fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
    fileGHNS->setToolTip(fileGHNS->whatsThis());
    fileGHNS->setStatusTip(fileGHNS->whatsThis());

    m_recentFilesAction = KStandardAction::openRecent(m_document, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());
    m_recentFilesAction->loadEntries(KGlobal::config()->group("Recent Files"));

    m_downloadedFilesAction = new KRecentFilesAction(KIcon("get-hot-new-stuff"), "file_open_downloaded", this);
    actionCollection()->addAction("file_open_downloaded", m_downloadedFilesAction);
    m_downloadedFilesAction->setText(i18n("Open Downloaded Vocabularies"));
    m_downloadedFilesAction->loadEntries(KGlobal::config()->group("Downloaded Files"));
    connect(m_downloadedFilesAction, SIGNAL(urlSelected(const KUrl &)), m_document, SLOT(open(const KUrl&)));
    m_downloadedFilesAction->loadEntries(KGlobal::config()->group("Downloaded Files"));
    m_downloadedFilesAction->setMaxItems(30);

    /*
    KAction* fileMerge = new KAction(this);
    actionCollection()->addAction("file_merge", fileMerge);
    fileMerge->setText(i18n("&Merge..."));
    connect(fileMerge, SIGNAL(triggered(bool)), m_document, SLOT(slotFileMerge()));
    fileMerge->setWhatsThis(i18n("Merge an existing vocabulary document with the current one"));
    fileMerge->setToolTip(fileMerge->whatsThis());
    fileMerge->setStatusTip(fileMerge->whatsThis());
    fileMerge->setEnabled(false); ///@todo merging files is horribly broken
    */

    KAction* fileSave = KStandardAction::save(m_document, SLOT(save()), actionCollection());
    fileSave->setWhatsThis(i18n("Save the active vocabulary document"));
    fileSave->setToolTip(fileSave->whatsThis());
    fileSave->setStatusTip(fileSave->whatsThis());

    KAction* fileSaveAs = KStandardAction::saveAs(m_document, SLOT(saveAs()), actionCollection());
    fileSaveAs->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    fileSaveAs->setWhatsThis(i18n("Save the active vocabulary document with a different name"));
    fileSaveAs->setToolTip(fileSaveAs->whatsThis());
    fileSaveAs->setStatusTip(fileSaveAs->whatsThis());

#ifdef HAVE_LIBXSLT

// Printing would be nice, but for now html export has to suffice
//     KAction* filePrint = KStandardAction::print(m_document, SLOT(print()), actionCollection());
//     filePrint->setWhatsThis(i18n("Print the active vocabulary document"));
//     filePrint->setToolTip(filePrint->whatsThis());
//     filePrint->setStatusTip(filePrint->whatsThis());

    KAction* fileExport = new KAction(this);
    actionCollection()->addAction("file_export", fileExport);
    fileExport->setText(i18n("&Export..."));
    connect(fileExport, SIGNAL(triggered(bool)), m_document, SLOT(exportHtmlDialog()));
    fileExport->setIcon(KIcon("document-export"));
    fileExport->setWhatsThis(i18n("Export to HTML"));
    fileExport->setToolTip(fileExport->whatsThis());
    fileExport->setStatusTip(fileExport->whatsThis());
#endif

    KAction* fileProperties = new KAction(this);
    actionCollection()->addAction("file_properties", fileProperties);
    fileProperties->setText(i18n("&Properties..."));
    connect(fileProperties, SIGNAL(triggered(bool)), SLOT(slotDocumentProperties()));
    fileProperties->setIcon(KIcon("document-properties"));
    fileProperties->setWhatsThis(i18n("Edit document properties"));
    fileProperties->setToolTip(fileProperties->whatsThis());
    fileProperties->setStatusTip(fileProperties->whatsThis());


    KAction* fileQuit = KStandardAction::quit(this, SLOT(close()), actionCollection());
    fileQuit->setWhatsThis(i18n("Quit Parley"));
    fileQuit->setToolTip(fileQuit->whatsThis());
    fileQuit->setStatusTip(fileQuit->whatsThis());


    KAction* editLanguages =new KAction(this);
    actionCollection()->addAction("edit_languages", editLanguages);
    editLanguages->setIcon(KIcon("set-language"));
    editLanguages->setText(i18n("&Languages..."));
    connect(editLanguages, SIGNAL(triggered()),  this, SLOT(slotEditLanguages()));
    ///@todo tooltip

    KAction* editGramar = new KAction(this);
    actionCollection()->addAction("edit_grammar", editGramar);
    editGramar->setText(i18n("&Grammar..."));
    editGramar->setWhatsThis(i18n("Edit language properties (types, tenses and usages)."));
    editGramar->setToolTip(editGramar->whatsThis());
    editGramar->setStatusTip(editGramar->whatsThis());
    connect(editGramar, SIGNAL(triggered(bool)), SLOT(slotGrammarDialog()));

    KAction *checkSpelling = KStandardAction::spelling(m_vocabularyView, SLOT(checkSpelling()), actionCollection());


    KAction *showSublessonentries = actionCollection()->add<KToggleAction>("lesson_showsublessonentries");
    showSublessonentries->setText(i18n("Show Entries from Child Lessons"));
    connect(showSublessonentries, SIGNAL(triggered(bool)), m_vocabularyModel, SLOT(showEntriesOfSubcontainers(bool)));
    showSublessonentries->setWhatsThis(i18n("Enable to also see the entries of child lessons in each lesson."));
    showSublessonentries->setToolTip(showSublessonentries->whatsThis());
    showSublessonentries->setStatusTip(showSublessonentries->whatsThis());
    showSublessonentries->setChecked(Prefs::showSublessonentries());

    KAction *automaticTranslation = actionCollection()->add<KToggleAction>("lesson_automatictranslation");
    automaticTranslation->setText(i18n("Automatic Translation"));
    connect(automaticTranslation, SIGNAL(triggered(bool)), m_vocabularyModel, SLOT(automaticTranslation(bool)));
    automaticTranslation->setWhatsThis(i18n("Enable for automatic translation of the lesson entries."));
    automaticTranslation->setToolTip(automaticTranslation->whatsThis());
    automaticTranslation->setStatusTip(automaticTranslation->whatsThis());
    automaticTranslation->setChecked(Prefs::automaticTranslation());

    KAction* removeGrades = new KAction(this);
    actionCollection()->addAction("vocab_remove_grades", removeGrades);
    removeGrades->setIcon(KIcon("edit-clear"));
    removeGrades->setText(i18n("Remove Grades"));
    connect(removeGrades, SIGNAL(triggered(bool)), this, SLOT(removeGrades()));
    removeGrades->setWhatsThis(i18n("Remove all grades from the current document"));
    removeGrades->setToolTip(removeGrades->whatsThis());
    removeGrades->setStatusTip(removeGrades->whatsThis());

// -- PRACTICE --------------------------------------------------

    KAction* configurePractice = new KAction(this);
    configurePractice->setText(i18n("Configure Practice..."));
    configurePractice->setIcon(KIcon("practice-setup"));
    configurePractice->setWhatsThis(i18n("Set up and start a test"));
    configurePractice->setToolTip(configurePractice->whatsThis());
    configurePractice->setStatusTip(configurePractice->whatsThis());
    actionCollection()->addAction("practice_configure", configurePractice);
    connect(configurePractice, SIGNAL(triggered(bool)), SLOT(configurePractice()));

    KAction* startPractice = new KAction(this);
    startPractice->setText(i18n("Start Practice..."));
    startPractice->setIcon(KIcon("practice-start"));
    startPractice->setWhatsThis(i18n("Start a test with the last settings"));
    startPractice->setToolTip(startPractice->whatsThis());
    startPractice->setStatusTip(startPractice->whatsThis());
    actionCollection()->addAction("practice_start", startPractice);
    connect(startPractice, SIGNAL(triggered(bool)), SLOT(startPractice()));

    KAction* showStatistics = new KAction(this);
    actionCollection()->addAction("show_statistics", showStatistics);
    showStatistics->setIcon(KIcon("statistics"));
    showStatistics->setText(i18n("&Statistics..."));
    connect(showStatistics, SIGNAL(triggered(bool)), this, SLOT(slotShowStatistics()));
    showStatistics->setWhatsThis(i18n("Show and reset statistics for the current vocabulary"));
    showStatistics->setToolTip(showStatistics->whatsThis());
    showStatistics->setStatusTip(showStatistics->whatsThis());


// -- SETTINGS --------------------------------------------------
    KAction* configApp = KStandardAction::preferences(this, SLOT(slotGeneralOptions()), actionCollection());
    configApp->setWhatsThis(i18n("Show the configuration dialog"));
    configApp->setToolTip(configApp->whatsThis());
    configApp->setStatusTip(configApp->whatsThis());

    m_vocabShowSearchBarAction = actionCollection()->add<KToggleAction>("config_show_search");
    m_vocabShowSearchBarAction->setText(i18n("Show Se&arch"));
    connect(m_vocabShowSearchBarAction, SIGNAL(triggered(bool)), this, SLOT(slotConfigShowSearch()));
    m_vocabShowSearchBarAction->setWhatsThis(i18n("Toggle display of the search bar"));
    m_vocabShowSearchBarAction->setToolTip(m_vocabShowSearchBarAction->whatsThis());
    m_vocabShowSearchBarAction->setStatusTip(m_vocabShowSearchBarAction->whatsThis());
    m_vocabShowSearchBarAction->setChecked(Prefs::showSearch());

    actionCollection()->addAction("show_vocabulary_columns_menu", m_vocabularyColumnsActionMenu);
    m_vocabularyColumnsActionMenu->setText(i18n("Vocabulary Columns"));
    m_vocabularyColumnsActionMenu->setWhatsThis(i18n("Toggle display of individual vocabulary columns"));
    m_vocabularyColumnsActionMenu->setToolTip(m_vocabularyColumnsActionMenu->whatsThis());
    m_vocabularyColumnsActionMenu->setStatusTip(m_vocabularyColumnsActionMenu->whatsThis());
    m_vocabularyView->horizontalHeader()->addAction(m_vocabularyColumnsActionMenu);

    actionCollection()->addAction(KStandardAction::TipofDay,  "help_tipofday", this, SLOT( tipOfDay() ));

// -- ONLY ON RIGHT CLICK - HEADER SO FAR -------------------------------------
    ///@todo what about this one...?
    KAction *actionRestoreNativeOrder = new KAction(this);
    actionCollection()->addAction("restore_native_order", actionRestoreNativeOrder);
    actionRestoreNativeOrder->setText(i18n("Restore Native Order"));


    KAction* findVocabulary = KStandardAction::find(m_searchLine, SLOT(setFocus()), actionCollection());

// SCRIPTS MENU
//     QMenu * scriptsMenu = menuBar()->addMenu(i18n("Scriptss"));
//     scriptsMenu->addAction(i18n("Test"));
    
    //Script Manager Menu Action
    KAction* menu_scriptManager =new KAction(this);
    actionCollection()->addAction("show_script_manager", menu_scriptManager);
    menu_scriptManager->setIcon(KIcon("set-language"));
    menu_scriptManager->setText(i18n("&Script Manager"));
    connect(menu_scriptManager, SIGNAL(triggered()),  this, SLOT(slotShowScriptManager()));

//     KAction* menu_translateLesson =new KAction(this);
//     actionCollection()->addAction("translate_lesson", menu_translateLesson);
//     menu_translateLesson->setIcon(KIcon("set-language"));
//     menu_translateLesson->setText(i18n("&Translate Lesson"));
//     connect(menu_translateLesson, SIGNAL(triggered()),  this, SLOT(slotTranslateLesson()));
// 
//     KAction* menu_testscript =new KAction(this);
//     actionCollection()->addAction("test_script", menu_testscript);
//     menu_testscript->setIcon(KIcon("set-language"));
//     menu_testscript->setText(i18n("&Test Script"));
//     connect(menu_testscript, SIGNAL(triggered()),  this, SLOT(slotTranslateLesson()));

    KToggleAction *oldPractice = actionCollection()->add<KToggleAction>("config_oldPractice");
    oldPractice->setText(i18n("Old Practice Dialogs"));
    connect(oldPractice, SIGNAL(triggered(bool)), this, SLOT(slotConfigOldPractice(bool)));
    m_vocabShowSearchBarAction->setChecked(Prefs::oldPractice());
}


void ParleyApp::initStatusBar()
{
    m_typeStatusBarLabel = new QLabel(statusBar());
    m_typeStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    statusBar()->addWidget(m_typeStatusBarLabel, 150);

    m_pronunciationStatusBarLabel = new QLabel(statusBar());
    m_pronunciationStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
    statusBar()->addWidget(m_pronunciationStatusBarLabel, 200);

    m_remarkStatusBarLabel = new QLabel(statusBar());
    m_remarkStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    statusBar()->addWidget(m_remarkStatusBarLabel, 150);
}

void ParleyApp::initModel()
{
    m_vocabularyModel = new VocabularyModel(this);

    m_vocabularyFilter = new VocabularyFilter(this);
    m_vocabularyFilter->setSourceModel(m_vocabularyModel);
    m_vocabularyView->setModel(m_vocabularyFilter);

    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyModel, SLOT(setDocument(KEduVocDocument*)));
    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyView, SLOT(setDocument(KEduVocDocument*)));

    connect(m_searchLine, SIGNAL(textChanged(const QString&)), m_vocabularyFilter, SLOT(setSearchString(const QString&)));
}

/**
 * This initializes the main widgets and table.
 */
void ParleyApp::initView()
{
    // Parent of all
    QWidget * mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout *topLayout = new QVBoxLayout(mainWidget);
    topLayout->setMargin(KDialog::marginHint());
    topLayout->setSpacing(KDialog::spacingHint());

    m_searchLine = new KLineEdit(this);
    m_searchLine->show();
    m_searchLine->setFocusPolicy(Qt::ClickFocus);
    m_searchLine->setClearButtonShown(true);
    m_searchLine->setClickMessage(i18n("Enter search terms here"));

//     m_searchLine->setToolTip(i18n("Search your vocabuary"));

    QLabel *label = new QLabel(i18n("S&earch:"), this);
    label->setBuddy(m_searchLine);
    label->show();

    m_searchWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(m_searchWidget);
    layout->setSpacing(KDialog::spacingHint());
    layout->setMargin(0);
    layout->addWidget(label);
    layout->addWidget(m_searchLine);

///@todo     centralWidget()-> delete layout
    QVBoxLayout * rightLayout = new QVBoxLayout(centralWidget());
    rightLayout->setSpacing(KDialog::spacingHint());
    rightLayout->setMargin(0);
    rightLayout->addWidget(m_searchWidget);
    m_searchWidget->setVisible(Prefs::showSearch());

    m_vocabularyView = new VocabularyView(this);
    m_vocabularyColumnsActionMenu = m_vocabularyView->columnsActionMenu();
    rightLayout->addWidget(m_vocabularyView, 1, 0);

    topLayout->addLayout(rightLayout);
}

void ParleyApp::slotShowScriptManager() {
//      kDebug() << QString("here!!");
    ScriptDialog * dialog = new ScriptDialog(m_scriptManager);
    dialog->show();
}


void ParleyApp::removeGrades()
{
    m_document->document()->lesson()->resetGrades(-1, KEduVocContainer::Recursive);
}

void ParleyApp::initScripts()
{
    m_scriptManager = new ScriptManager(this);

    m_vocabularyView->setTranslator(&m_translator);

    //add Scripting::Parley
    m_scriptObjectParley = new Scripting::Parley(this);
    m_scriptObjectParley->setTranslator(&m_translator);
    m_scriptManager->addObject ( m_scriptObjectParley,"Parley" );

    //add Scripting::Expression
    /// @note not a good idea to be added cause it can be used without instanciating it
    //     Scripting::Expression * expression = new Scripting::Expression();
    //     m_scriptManager.addObject( expression, "Entry" );

    //Load scripts
    m_scriptManager->loadScripts();

    connect(m_vocabularyModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotTranslateWords(const QModelIndex&, const QModelIndex&)),
            Qt::QueuedConnection
            );
    connect(m_scriptObjectParley, SIGNAL(translationFinished(const QString&,const QString&,const QString &)),
            this, SLOT(slotTranslationFinished(const QString&, const QString&, const QString&)),
            Qt::QueuedConnection
            );
}

void ParleyApp::slotTranslateWords(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    if (Prefs::automaticTranslation() == false) return;

    kDebug() << "Translate Words" << topLeft << bottomRight;

    const int N = VocabularyModel::EntryColumnsMAX;
    QString fromLanguage;
    QString word;

    //find the first translation column that has a word in it (not empty)
    for (int i = 0; i < topLeft.model()->columnCount(topLeft.parent()); i += N) {
        word = topLeft.model()->index(topLeft.row(),i,QModelIndex()).data().toString();
        if (!word.isEmpty()) {
            fromLanguage = m_document->document()->identifier(i / N).locale();
            break;
        }
    }

    //translate to the rest translation columns
    for (int i = 0; i < topLeft.model()->columnCount(topLeft.parent()); i += N) {
        const QString & toLanguage = m_document->document()->identifier(i / N).locale();
        if (toLanguage != fromLanguage) {
            m_scriptObjectParley->callTranslateWord(word,fromLanguage,toLanguage);
        }
    }

    return;
//     //old function
//     if (topLeft.column() == VocabularyModel::Translation) {
//         QString word = topLeft.data().toString();
//         QString fromLanguage = m_document->document()->identifier(0).locale();
//         
//         //iterate through all the translation columns
//         for (int i = topLeft.column()+N; i < topLeft.model()->columnCount(topLeft.parent()); i += N) {
//             QString toLanguage = m_document->document()->identifier(i / N).locale();
//             kDebug() << word << fromLanguage << toLanguage;
//             //the scripts will receive a signal to translate this word
//             m_scriptObjectParley->callTranslateWord(word,fromLanguage,toLanguage);
//         }
//     }
}

int indexOfIdentifier(KEduVocDocument* document, const QString& locale) {
    for (int i = 0; i < document->identifierCount(); i++)
        if (document->identifier(i).locale() == locale)
            return i;
    return -1;
}

void ParleyApp::slotTranslationFinished(const QString & word,const QString& fromLanguage,const QString& toLanguage)
{
    if (!m_translator.getTranslation(word,fromLanguage,toLanguage))
        return;

//     kDebug() << "Translation Finised";

    //get identifiers
    int fromIdentifier = indexOfIdentifier(m_document->document(),fromLanguage);
    int toIdentifier = indexOfIdentifier(m_document->document(),toLanguage);

    if (fromIdentifier == -1 || toIdentifier == -1) return;
//     kDebug() << fromIdentifier << toIdentifier;
    int N = VocabularyModel::EntryColumnsMAX;

    //iterate through all the lesson rows (entries) and fill up the empty cells, if a translation is available
    for (int r = 0; r < m_vocabularyModel->rowCount(QModelIndex()); r++) {
        const QModelIndex& fromIndex = m_vocabularyModel->index(r,fromIdentifier * N,QModelIndex());
        const QModelIndex& toIndex = m_vocabularyModel->index(r,toIdentifier * N, QModelIndex());

//         kDebug() << fromIndex.data().toString() << toIndex.data().toString();

        if (fromIndex.data().toString() == word && toIndex.data().toString().isEmpty()) {
            QString firstTranslation = *(m_translator.getTranslation(word,fromLanguage,toLanguage)->begin());
//             kDebug() << "First translation: " << firstTranslation;
//             kDebug() << fromIndex;
//             kDebug() << toIndex;
            m_vocabularyModel->setData(toIndex,firstTranslation,Qt::EditRole);
        }
    }
}

ParleyDocument* ParleyApp::parleyDocument()
{
    return m_document;
}


#include "parley.moc"


