# please provide kvoctrain/* AND kvoctrain/kvoctrain/*
# this enables me to create a personal tar ball

INCLUDES = -I. \
           -I$(top_srcdir)/kvoctrain/kvoctrain/common-dialogs \
           -I$(top_srcdir)/kvoctrain/common-dialogs \
           -I$(top_builddir)/kvoctrain/kvoctrain/common-dialogs \
           -I$(top_srcdir)/kvoctrain/kvoctrain \
           -I$(top_srcdir)/kvoctrain \
           -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core \
           -I$(top_srcdir)/kvoctrain/kvt-core \
           $(all_includes)

noinst_LTLIBRARIES = libdocpropdlg.la

libdocpropdlg_la_SOURCES = \
  TitlePage.cpp    TypeOptPage.cpp      TenseOptPage.cpp     UsageOptPage.cpp \
  TitlePageForm.ui TypeOptPageForm.ui   TenseOptPageForm.ui  UsageOptPageForm.ui \
  LessOptPage.cpp    DocOptionsPage.cpp    LangPropPage.cpp \
  LessOptPageForm.ui DocOptionsPageForm.ui LangPropPageForm.ui \
  DocPropDlg.cpp    DocPropLangDlg.cpp

METASOURCES = AUTO
