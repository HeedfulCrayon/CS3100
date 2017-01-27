/*
	Nathan Borup
	CS 3100
	Lab 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>


// usage() prints a standard usage statement
void usage(char *path) {
    fprintf(stderr, "Usage: %s -n NAME\n", path);
}

int main(int argc, char *argv[])
{
	char *name;

	if (argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}
	
	// read the input parameter and exit on error
	int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "n:")) != -1) 
	{
		switch (c) 
		{
			case 'n':
				name = optarg;
				break;
			default:
				usage(argv[0]);
				exit(1);
		}
	}
	
	// output on STDOUT "Welcome to Lab 1, written by USERNAME"
	printf("Welcome to Lab 1, written by %s\n", name);
	// output on STDOUT "Hostname: <hostname>"
	char hostname[1024];
	gethostname(hostname,sizeof(hostname));
	printf("Hostame: %s\n", hostname);
	// output on STDOUT "PID and PPID: nnnnn/nnnnn"
	printf("PID and PPID: %i/%i\n", getpid(), getppid());
	// output on STDOUT "Program: <pathToThisProgram>"
	char cwd[1024];	
	getcwd(cwd,sizeof(cwd));
	printf("Program: %s\n",argv[0]);
	return 0;
}

