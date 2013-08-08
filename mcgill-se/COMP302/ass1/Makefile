SOURCES= \
		 q1.scm \
		 q2.scm \
		 q3.scm \
		 q4.scm \
		 q5.scm \
		 q6.scm \
		 q7.scm \
		 q8.scm \
		 q9.scm \
		 q10.scm

all: all-procedures.scm all-tests

all-procedures.scm: $(SOURCES)
	cat $^ | grep -v '(load' > $@

all-tests: $(SOURCES:.scm=.test)
	echo '> (load "all-procedures.scm")' > $@
	cat $^ | grep -v '(load' >> $@

clean:
	rm -f all-procedures.scm all-tests
