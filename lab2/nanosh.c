/* 
 CS 3100 Lab 2 - by Nathan Borup
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void cmdExit(int argc, char **argv) {
	exit(0);
}

// getParameters returns the argc, the number of words found in cmd
//  while populating argv with pointers to each word
int getParameters(char *cmd, char **argv) {
	char *token;
	int argc = 0;
	token = strtok(cmd, " \t\n");
	while (token != NULL) {
		argv[argc] = token;
		argc++;
		token = strtok(NULL, " \t\n");
	}
	argv[argc] = NULL;		// set last + 1 argument to NULL
	return argc;
}

int main(int argc, char **argv) {

	char cmd[1024];
	char *rc;
	int myArgc = 0;
	char *myArgv[1000];

	while (1) {
		//char pwd[1024];
		//getcwd(pwd, sizeof(pwd));
		printf("nanosh: ");
		fflush(stdout);
		rc = fgets(cmd, sizeof(cmd), stdin);
		if (rc == NULL) {
			exit(0);
		}
		myArgc = getParameters(cmd, myArgv);

		// if no words typed: restart loop
		if (myArgc == 0) {			
			continue;
		}

		// if the first word is "exit", terminate the program
		if ((strcmp(myArgv[0], "exit") == 0) && (myArgc == 1)){
			cmdExit(myArgc, myArgv);
			continue;
		}
		else if ((strcmp(myArgv[0], "exit") == 0) && (myArgc > 1)) {
			errno = EINVAL;
			perror("Too many arguments");
			continue;
		}

		// add if statements here for the other internal commands
		// PWD - Show current working directory
		if ((strcmp(myArgv[0], "pwd") == 0) && (myArgc == 1)) {
			char pwd[1024];
			getcwd(pwd,sizeof(pwd));
			printf("%s\n",pwd);
			continue;
		}
		else if ((strcmp(myArgv[0], "pwd") == 0) && (myArgc > 1)) {
			errno = EINVAL;
			perror("Too many arguments");
			continue;	
		}

		// CD - Change working directory to $HOME
		if ((strcmp(myArgv[0], "cd") == 0) && (myArgc == 1)) {
			chdir(getenv("HOME"));
			//printf("%s\n",getenv("HOME"));
			continue;
		}
		else if ((strcmp(myArgv[0], "cd") == 0) && (myArgc == 2)) {
			if(chdir(myArgv[1])){
				errno = ENOENT;
				perror("Invalid Directory");		
			}
			else {
				char pwd[1024];
				getcwd(pwd,sizeof(pwd));
				//printf("%s\n",pwd);
			}
			continue;
		}
		else if ((strcmp(myArgv[0], "cd") == 0) && (myArgc > 2)) {
			errno = EINVAL;
			perror("Too many arguments");
			continue;
		}
		else {
			pid_t pid;
			int *returnVal = 0;
			pid = fork();
			if (pid == 0) {
				//child process
				if (execvp(myArgv[0], myArgv)) {
					errno = ENOENT;
					perror("Invalid Command");
					exit(1);
				}
			}
			if (pid > 0) {
				//parent process
				waitpid(pid,returnVal,0);
			}
		}
		
		//   and a default action that calls a function to fork()
		//   and exec() while the parent issues waitpid()
	}

	return 0;
} 
