XMLPROC=xsltproc --nonet --xinclude

all: index.html

index.html: assignment-4.xml html.xsl
	$(XMLPROC) --output $@ html.xsl $<
