test: q1.out q2.out
	@grep FAILURE $+ || echo "No errors found."

clean:
	rm -f *.out *.exe

%.exe: %.c
	gcc -Wall -g $< -o $@

q2.out: q2.exe
	exec ./$< > $@

q1.out: test-q1.py q1.exe
	exec ./$< > $@
