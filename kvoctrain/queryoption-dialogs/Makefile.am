# please provide kvoctrain/* AND kvoctrain/kvoctrain/*
# this enables me to create a personal tar ball

INCLUDES = -I$(top_srcdir)/kvoctrain/kvoctrain/common-dialogs \
           -I$(top_builddir)/kvoctrain/common-dialogs \
           -I$(top_srcdir)/kvoctrain/kvoctrain \
           -I$(top_srcdir)/kvoctrain \
           -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core \
           -I$(top_srcdir)/kvoctrain/kvt-core \
           $(all_includes)

noinst_LTLIBRARIES = libqueryoptiondlg.la

libqueryoptiondlg_la_SOURCES = \
 GroupOptPage.cpp    QueryOptPage.cpp    ThreshOptPage.cpp    BlockOptPage.cpp \
 GroupOptPageForm.ui QueryOptPageForm.ui ThreshOptPageForm.ui BlockOptPageForm.ui \
 QueryOptionsDlg.cpp

METASOURCES = AUTO

