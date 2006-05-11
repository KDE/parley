kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kvoctrain/icons FILES  )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



#original Makefile.am contents follow:

#kvoctrainiconsdir = $(kde_datadir)/kvoctrain/icons
#
#kvoctrainicons_ICON = AUTO
#
#KDE_ICON = kvoctrain
#
