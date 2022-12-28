#include <stdio.h>


typedef struct Promotor {
	int pid;
	int fd[2];
	char name[20];
	int valid;
} Promotor;


