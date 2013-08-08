MYSQL_INC=-I/opt/mysql/include/mysql

static:
	gcc -Wall -g -o stripchars.o stripchars.c $(MYSQL_INC)

so:
	gcc -Wall -g -fno-implicit-templates -fno-exceptions -fno-rtti -shared -o stripchars.so stripchars.c $(MYSQL_INC)

install: so
	cp -f stripchars.so /usr/local/lib/libstripchars.so
	/sbin/ldconfig
