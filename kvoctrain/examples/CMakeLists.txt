kde4_header()




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
