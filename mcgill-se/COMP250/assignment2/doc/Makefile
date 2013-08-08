XMLPROC=xsltproc --nonet --xinclude

graphs = ../src/fibrec.png	\
	 ../src/fibit.png	\
	 ../src/fibsquare.png	\
	 ../src/expmod.png	\
	 ../src/fast.png

tex_files = \
	fibsquare-formula.tex	\
	fibsquare-proof.tex	\
	fibmatrix-formula.tex	\
	fibmatrix-proof.tex

graphics = $(graphs) $(tex_files:.tex=.png)

index.html: assignment-2.xml html.xsl
	$(XMLPROC) --output $@ html.xsl $<

assignment-2.xml: $(png_files) $(graphs)

%.png: %.tex
	texgd_src=`cat $<` texgd_outfile=$@ texgd
