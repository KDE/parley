KDE_LANG = en
KDE_DOCS = AUTO
KDE_MANS = AUTO

check-local:
	$(MEINPROC) --check index.docbook

convert-local:
	$(MEINPROC) $(srcdir)/index.docbook
                	
# needs pngcrush which reduces files size about 20%
# brute-crush takes _very_ long time
crush:
	mkdir -p crushed
	for i in *.png; do \
	  pngcrush $$i crushed/$$i;\
	done
	mv *.png /tmp
	mv crushed/*.png .
	rm -r crushed

brute-crush:
	mkdir -p crushed
	for i in *.png; do \
	  pngcrush -brute $$i crushed/$$i;\
	done
	mv *.png /tmp
	mv crushed/*.png .
	rm -r crushed
