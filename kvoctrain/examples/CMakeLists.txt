kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kvoctrain/examples FILES sample-de.kvtml sample-en.kvtml )

kde4_footer()



#original Makefile.am contents follow:

#
#examples_DATA = sample-de.kvtml sample-en.kvtml
#examplesdir = $(kde_datadir)/kvoctrain/examples
#
#EXTRA_DIST = sample-template-ascii.kvtml
#
