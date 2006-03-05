kde4_header()

add_subdirectory( kvoctrain ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_create_doxygen_docs(  )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = kvoctrain
#
#EXTRA_DIST = AUTHORS
#
#AUTOMAKE_OPTIONS = foreign
#
#install-data-local:
#
#clean_distdir:
#	-rm -rf $(distdir)
#
#
#lsm : $(PACKAGE)/$(PACKAGE).lsm.tpl $(distdir).tar.bz2
#	let "_arclen=`ls -l -d -G $(distdir).tar.bz2 | cut -b23-32`/1000+1";\
#	sed $(PACKAGE)/$(PACKAGE).lsm.tpl  >$(PACKAGE)-$(VERSION).lsm -e s/=VERSION=/$(VERSION)/g  -e "s,=DATE=,`date +"%Y %B %d."`,g" -e "s,=ARCLEN=,$$_arclen,g"
#	cp $(PACKAGE)-$(VERSION).lsm $(PACKAGE).lsm
#
#
#
#include ../admin/Doxyfile.am
