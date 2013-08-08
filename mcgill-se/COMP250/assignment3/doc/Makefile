XMLPROC=xsltproc --nonet --xinclude

all: index.html images

tex_files = \
	q1a-q.tex	\
	q1a-a.tex	\
	q1b-q.tex	\
	q1c-q.tex	\
	q1c-a.tex

graphics = $(tex_files:.tex=.png)

images : $(graphics)

index.html: assignment-3.xml html.xsl
	$(XMLPROC) --output $@ html.xsl $<

assignment-2.xml: $(png_files) $(graphs)

%.png: %.tex
	texgd_src=`cat $<` texgd_outfile=$@ texgd
