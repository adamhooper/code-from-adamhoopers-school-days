XMLPROC=xsltproc --nonet --xinclude

source_files = ../src/q1.c ../src/q2.c ../src/q1.out ../src/q2.out ../src/Makefile ../src/test-q1.py

all: index.html index.pdf

index.html: assignment-1.xml html.xsl $(source_files)
	$(XMLPROC) --output $@ html.xsl $<

index.fo: assignment-1.xml pdf.xsl $(source_files)
	$(XMLPROC) --output $@ pdf.xsl $<

index.pdf: index.fo
	fop -fo $< -pdf $@
