index.html: assignment-1.xml html.xsl
	xsltproc --nonet --output index.html html.xsl assignment-1.xml

index.fo: assignment-1.xml pdf.xsl
	xsltproc --nonet --output index.fo pdf.xsl assignment-1.xml

index.pdf: index.fo
	fop -fo index.fo -pdf index.pdf
