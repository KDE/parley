INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libkvoctraincore.la

METASOURCES = AUTO

libkvoctraincore_la_SOURCES = \
 langset.cpp \
 LineList.cpp \
 kvoctrainexpr.cpp \
 kvoctraindoc.cpp \
 kvd_rh_kvtml.cpp \
 kvd_rb_kvtml1.cpp kvd_rb_kvtml2.cpp \
 kvd_wkvtml.cpp \
 kvd_csv.cpp kvd_lex.cpp kvd_vcb.cpp \
 GrammerManager.cpp UsageManager.cpp \
 QueryManager.cpp 

# CLEANFILES += *~ # This breaks badly with automake 1.5

SUBDIRS = kvt-xml

libkvoctraincore_la_LIBADD= kvt-xml/libkvtxml.la

