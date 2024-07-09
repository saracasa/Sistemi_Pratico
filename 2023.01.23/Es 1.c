//CSUNIBO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if( argc < 2 ) {
		perror( "Bad usage! u have to specify pipe name\n" );
		return -1;
	}

	FILE *mypipe;
	size_t buflen = 0;
	char *buf = NULL;
	size_t len;

	// crea una named pipe (fifo) al path indicato come primo e unico argomento
	// mkfifo(<pathname>, <permission>)
	char *myname = argv[1];

	if( mkfifo(myname, 0666) < 0 ){
		perror( "Cannot create a pipe\n" );
	}
	while (1) { 
		//apra la named pipe in lettura
		mypipe = fopen( myname, "r" );

		while ( len == getline(&buf, &buflen, mypipe ) > 0 ) {
	    		printf( "Received: %s", buf );
			int exit = ! strcmp(buf, "FINE\n");
	    		if( exit ) {	//compare buf con "FINE"
				printf("fine!\n");
				unlink(myname);
				return 0;
			}
		}

		fclose(mypipe);

	}
	return 0;
}

