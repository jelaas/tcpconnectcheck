#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <poll.h>
#include <arpa/inet.h>

#include "ttcp.h"

struct {
	int verbose;
	int timeout;
	int wait;
} conf;

int main(int argc, char **argv)
{
	int fd;
	int rc;
	struct sockaddr_in addr;

	while(argc > 1 && argv[1][0] == '-') {
		if(!strcmp(argv[1], "-h")) {
		help:
			printf("tcpconnectcheck [OPTIONS] IP PORT\n"
			       " -t N            wait a maximum of N seconds until connection can be established\n"
			       " -w              wait until connection can be established\n"
			       " -h              this help\n"
			       " -v              verbose\n"
			       "\n"
			       "Check reachability or wait for connection.\n"
			       "\n"
				);
			exit(0);
		}
		if(!strcmp(argv[1], "-v")) {
			conf.verbose = 1;			
		}
		if(!strcmp(argv[1], "-w")) {
			conf.wait = 1;			
		}
		if(!strcmp(argv[1], "-t")) {
			conf.timeout = atoi(argv[2]);
			argv++;
			argc--;
		}
		argv++;
		argc--;
	}

	if(argc < 3) goto help;

        memset(&addr, 0, sizeof(addr));
	inet_aton(argv[1], &addr.sin_addr);
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_family = AF_INET;
	
	fd = socket( AF_INET, SOCK_STREAM, 0 );

	do {
		rc = tconnect(fd, (const struct sockaddr *) &addr, sizeof(addr), 1000);
		if(conf.verbose) fprintf(stderr, "rc = %d\n", rc);
		if(rc == 0) {
			if(conf.verbose) fprintf(stderr, "connected\n");
			exit(0);
		}
		if(rc == -1) sleep(1);
		if(conf.timeout) conf.timeout--;
	} while(conf.timeout || conf.wait);
	
	if(conf.verbose) fprintf(stderr, "connection failed\n");
	
	exit(1);
}
