INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = liboptiondlg.la

liboptiondlg_a_DLG_sources = \
  GenOptPage.dlg \
  LangOptPage.dlg \
  ViewOptPage.dlg \
  PasteOptPage.dlg
  
liboptiondlg_a_QOBJ_sources =   \
  $(liboptiondlg_a_DLG_sources:.dlg=.cpp) \
  $(liboptiondlg_a_DLG_sources:.dlg=Data.cpp) \
  GeneralOptionsDlg.cpp 


liboptiondlg_la_SOURCES = $(liboptiondlg_a_QOBJ_sources)
                        
EXTRA_DIST = \
  $(liboptiondlg_a_QOBJ_sources:.cpp=.h) \
  $(liboptiondlg_a_DLG_sources)

# CLEANFILES +=  *~ # This breaks badly with automake 1.5

#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO

