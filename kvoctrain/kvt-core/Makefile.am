INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libkvoctraincore.a

libkvoctraincore_a_DEPENDENCIES = \
     ./kvt-xml/libkvtxml.a \
     $(libkvoctraincore_a_METASOURCES) $(libkvoctraincore_a_METASOURCES:.cpp=.o)
     
libkvoctraincore_a_SOURCES = \
 $(libkvoctraincore_a_QOBJ_SOURCES)  \
 $(libkvoctraincore_a_QOBJ_SOURCES:.cpp=.moc.cpp) \
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

libkvoctraincore_a_HEADER = \
  $(libkvoctraincore_a_QOBJ_SOURCES:.cpp=.h) \
  kvoctrainexpr.h \
  kv_resource.h \
  langset.h \
  QueryManager.h GrammerManager.h UsageManager.h \
  eadebug.h compat_2x.h

EXTRA_DIST = $(APPS_FILES) $(libkvoctraincore_a_HEADER)

libkvoctraincore_a_DLG_SOURCES = 

libkvoctraincore_a_QOBJ_SOURCES =   \
   kvoctraindoc.cpp \
   $(libkvoctraincore_a_DLG_SOURCES:.dlg=.cpp) \
   $(libkvoctraincore_a_DLG_SOURCES:.dlg=Data.cpp) 

libkvoctraincore_a_LIBADD=`echo $(libkvoctraincore_a_SOURCES:.cpp=.o)` \
        ./kvt-xml/XmlReader.o \
        ./kvt-xml/XmlTokenizer.o \
        ./kvt-xml/XmlElement.o \
        ./kvt-xml/XmlWriter.o


EXTRA_DIST = \
   $(libkvoctraincore_a_QOBJ_SOURCES:.cpp=.h) \
   $(libkvoctraincore_a_DLG_SOURCES)

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

CLEANFILES += *.rpo *.i *~

doc:

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(libkvoctraincore_a_METASOURCES)

i18n_files=  kvt-xml/*.cpp \
             *.i           \
             $(libkvoctraincore_a_SOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kvoctrainexpr.h >kvoctrainexpr.i
	$(XGETTEXT) -C -ki18n -ki18n_noop -x $(includedir)/kde.pot `ls $(i18n_files)` && mv messages.po ../po/kvoctrain.pot
	-rm *.i

kvd_rb_kvtml2.o: kvd_rb_kvtml2.cpp
	$(CXXCOMPILE) -O1 -Wall -c $<

kvoctraindoc.moc.cpp: $(srcdir)/kvoctraindoc.h
	$(MOC) $< -o kvoctraindoc.moc.cpp

clean-metasources:
	-rm -f $(libkvoctraincore_a_METASOURCES) *.moc.*

release: 
