CC=gcc
CFLAGS="-Wall"

debug:clean
	$(CC) $(CFLAGS) -g -o treesdl2project main.c
stable:clean
	$(CC) $(CFLAGS) -o treesdl2project main.c
clean:
	rm -vfr *~ treesdl2project
