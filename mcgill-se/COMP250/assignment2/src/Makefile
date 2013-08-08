all: fibit.png fibrec.png fibsquare.png expmod.png fast.png

%.png: %.data
	echo "set terminal png color" > .gnuplot_cmds
	echo "set output '$@'" >> .gnuplot_cmds
	echo "set xlabel 'n'" >> .gnuplot_cmds
	echo "set ylabel 'Running Time (ms)'" >> .gnuplot_cmds
	echo "plot '$<' title '$(<:.data=)'" >> .gnuplot_cmds
	gnuplot < .gnuplot_cmds
	rm .gnuplot_cmds

%.data: Main.class
	touch $@
	java Main $(@:.data=) > $@

%.class: %.java
	javac $<
