SUBDIRS = kvoctrain 

EXTRA_DIST = AUTHORS ChangeLog README README.develop \
             TODO kvoctrain.kdevprj bug-report

AUTOMAKE_OPTIONS = foreign 

install-data-local:

bug-rep:
	./bug-report

clean_distdir:
	-rm -rf $(distdir)


lsm : $(PACKAGE)/$(PACKAGE).lsm.tpl $(distdir).tar.bz2
	let "_arclen=`ls -l -d -G $(distdir).tar.bz2 | cut -b23-32`/1000+1";\
	sed $(PACKAGE)/$(PACKAGE).lsm.tpl  >$(PACKAGE)-$(VERSION).lsm -e s/=VERSION=/$(VERSION)/g  -e "s,=DATE=,`date +"%Y %B %d."`,g" -e "s,=ARCLEN=,$$_arclen,g"
	cp $(PACKAGE)-$(VERSION).lsm $(PACKAGE).lsm


