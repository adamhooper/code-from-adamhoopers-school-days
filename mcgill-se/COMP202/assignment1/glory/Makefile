all: GloryApplet.class

clean:
	rm -f *.class

%.class: %.java
	gcj -C -Wall $< --encoding=UTF-8
