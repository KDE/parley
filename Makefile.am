SUBDIRS = kvoctrain 

EXTRA_DIST = AUTHORS ChangeLog README README.develop \
             TODO kvoctrain.kdevprj bug-report

AUTOMAKE_OPTIONS = foreign 

install-data-local:
	@echo ""
	@echo ""
	@echo ""
	@echo "*************** Important *************************"
	@echo ""
	@echo " This package does not contain any documentation."
	@echo ""
	@echo " Download the doc-package according to your language"
	@echo " code (you really should do it)."
	@echo ""
	@echo "   e.g.: - kvoctrain-0.7.0.doc-en.tar.bz2  (english)"
	@echo "         - kvoctrain-0.7.0.doc-de.tar.bz2  (german)"
	@echo ""
	@echo " Homepage is now on Sourceforge:"
	@echo "     http://kvoctrain.sourceforge.net"
	@echo ""
	@echo " Please report bugs at"
	@echo "     http://sourceforge.net/bugs/?group_id=9265"
	@echo ""
	@echo " Ewald Arnold <ewald@ewald-arnold.de> whishes you"
	@echo " a happy learning."
	@echo "****************************************************"
	@echo ""

bug-rep:
	./bug-report

clean_distdir:
	-rm -rf $(distdir)

release: clean_distdir $(DISTFILES) 
	mkdir -p $(distdir)
	-chmod 777 $(distdir)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --foreign Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir -p $(distdir)/$$subdir \
	    || exit 1; \
	    chmod 777 $(distdir)/$$subdir; \
	    (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir=../$(distdir) distdir=../$(distdir)/$$subdir distdir) \
	      || exit 1; \
	  fi; \
	done
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) choIf $(distdir).tar.bz2 $(distdir)
	let "_arclen=`ls -l -d -G $(distdir).tar.bz2 | cut -b24-33`/1000+1";\
	sed $(PACKAGE)/$(PACKAGE).lsm.tpl  >$(PACKAGE)-$(VERSION).lsm -e s/=VERSION=/$(VERSION)/g  -e "s,=DATE=,`date +"%Y %B %d."`,g" -e "s,=ARCLEN=,$$_arclen,g"
	cp $(PACKAGE)-$(VERSION).lsm $(PACKAGE).lsm
	mv $(distdir).tar.bz2 ..
	mv $(PACKAGE)-$(VERSION).lsm ..
	-rm -rf $(distdir)
	@echo ""
	@echo "#########################################"
	@echo "#  ?? aboutdlg.cpp: update date-string  #"
	@echo "#                                       #"
	@echo "#  ?? make messages                     #"
	@echo "#  ?? make merge                        #"
	@echo "#                                       #"
	@echo "#########################################"


lsm : $(PACKAGE)/$(PACKAGE).lsm.tpl $(distdir).tar.bz2
	let "_arclen=`ls -l -d -G $(distdir).tar.bz2 | cut -b23-32`/1000+1";\
	sed $(PACKAGE)/$(PACKAGE).lsm.tpl  >$(PACKAGE)-$(VERSION).lsm -e s/=VERSION=/$(VERSION)/g  -e "s,=DATE=,`date +"%Y %B %d."`,g" -e "s,=ARCLEN=,$$_arclen,g"
	cp $(PACKAGE)-$(VERSION).lsm $(PACKAGE).lsm


clean-metasources: clean
	  -rm config.cache
	  cd kvoctrain && $(MAKE) clean-metasources 
	  @echo "#####################################################"
	  @echo "# Re-run configure to detect your version of QT/KDE #"
	  @echo "# and create the necessary files                    #"
	  @echo "# Do not forget to set QTDIR and KDEDIR correctly   #"
	  @echo "#####################################################"

refresh-dlg:
	find . -name *.dlg  -exec touch {} \;


