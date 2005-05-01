INCLUDES = -I$(srcdir)/.. -I$(srcdir)/../kvt-core $(all_includes)

noinst_LTLIBRARIES = libcommondlg.la

libcommondlg_la_SOURCES = ProgressDlg.cpp ProgressDlgForm.ui \
  languagesettings.kcfgc prefs.kcfgc presettings.kcfgc kvoctrainprefs.cpp kvoctrainprefs.h \
  generaloptionsbase.ui generaloptions.cpp viewoptionsbase.ui viewoptions.cpp \
  pasteoptionsbase.ui pasteoptions.cpp blockoptionsbase.ui groupoptionsbase.ui \
  languageoptionsbase.ui queryoptionsbase.ui thresholdoptionsbase.ui blockoptions.cpp \
  groupoptions.cpp languageoptions.cpp queryoptions.cpp thresholdoptions.cpp \
  profilesdialog.cpp

METASOURCES = AUTO

kde_kcfg_DATA = kvoctrain.kcfg languagesettings.kcfg presettings.kcfg

noinst_HEADERS = generaloptions.h viewoptions.h pasteoptions.h blockoptions.h \
  groupoptions.h languageoptions.h queryoptions.h thresholdoptions.h profilesdialog.h
