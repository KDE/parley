INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libkvoctraincore.a

METASOURCES = AUTO

libkvoctraincore_a_SOURCES = \
 langset.cpp \
 LineList.cpp \
 kvoctrainexpr.cpp \
 kvd_rh_kvtml.cpp \
 kvd_rb_kvtml1.cpp kvd_rb_kvtml2.cpp \
 kvd_wkvtml.cpp \
 kvd_csv.cpp kvd_lex.cpp kvd_vcb.cpp \
 GrammerManager.cpp UsageManager.cpp \
 QueryManager.cpp 

SUBDIRS = kvt-xml

libkvoctraincore_a_LIBADD= kvt-xml/libkvtxml.a

