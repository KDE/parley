INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain

noinst_LIBRARIES = libkvtxml.a

libkvtxml_a_METASOURCES =

libkvtxml_a_SOURCES = $(libkvtxml_a_METASOURCES) \
 XmlReader.cpp XmlTokenizer.cpp XmlElement.cpp XmlWriter.cpp

EXTRA_DIST = \
  XmlReader.h XmlTokenizer.h XmlWriter.h XmlElement.h  koxml_config.h

CLEANFILES += *.rpo *.html *~

doc:

messages:

clean-metasources:

release: 
