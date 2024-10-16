CC=musl-gcc-x86_32
CFLAGS=-Wall -D_GNU_SOURCE
all:	tcpconnectcheck
tcpconnectcheck: tcpconnectcheck.o ttcp.o
rpm:	tcpconnectcheck
	strip tcpconnectcheck
	bar -c --license=GPLv3 --version 1.0 --release 1 --name tcpconnectcheck --prefix=/usr/bin --fgroup=root --fuser=root tcpconnectcheck-1.0-1.rpm tcpconnectcheck
clean:
	rm -f *.o tcpconnectcheck
