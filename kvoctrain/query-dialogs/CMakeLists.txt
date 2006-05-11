kde4_header()




########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/../../../libkdeedu/kdeeducore -I$(srcdir)/.. \
#	-I../common-dialogs -I$(srcdir)/../kvt-core -I$(top_builddir)/kvoctrain/kvoctrain/share \
#	$(all_includes)
#
#noinst_LTLIBRARIES = libquerydlg.la
#
#libquerydlg_la_SOURCES = QueryDlgBase.cpp MCQueryDlgForm.ui \
#	RandomQueryDlgForm.ui SimpleQueryDlgForm.ui MCQueryDlg.cpp RandomQueryDlg.cpp \
#	SimpleQueryDlg.cpp VerbQueryDlgForm.ui AdjQueryDlgForm.ui ArtQueryDlgForm.ui \
#	VerbQueryDlg.cpp AdjQueryDlg.cpp ArtQueryDlg.cpp
#
#METASOURCES = AUTO
#
