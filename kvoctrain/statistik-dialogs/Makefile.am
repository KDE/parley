INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libstatdlg.la

libstatdlg_a_DLG_sources = \
  StatistikPage.dlg \
  GenStatPage.dlg

libstatdlg_a_QOBJ_sources =   \
  $(libstatdlg_a_DLG_sources:.dlg=.cpp) \
  $(libstatdlg_a_DLG_sources:.dlg=Data.cpp) \
  StatistikDlg.cpp 

libstatdlg_la_SOURCES = $(libstatdlg_a_QOBJ_sources)
                        
EXTRA_DIST = \
  $(libstatdlg_a_QOBJ_sources:.cpp=.h) \
  $(libstatdlg_a_DLG_sources)

# CLEANFILES +=  *~ # This breaks badly with automake 1.5

#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO

