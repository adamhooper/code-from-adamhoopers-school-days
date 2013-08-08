XSLTPROC=xsltproc --nonet --xinclude
FOP=fop

source_files = ../src/*.java

all: index.html index.pdf

index.html: assignment-5.xml html.xsl $(source_files)
	$(XSLTPROC) --output $@ html.xsl $<

index.pdf: assignment-5.xml fo.xsl $(source_files)
	$(XSLTPROC) --output t.fo fo.xsl $<
	$(FOP) -fo t.fo -pdf $@
	rm t.fo
