 #include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <mqueue.h>
#include <stdarg.h>
#include <time.h>

#include <assert.h>

#include "logger_task.h"

int main()
{
	FILE *fptr;
	char str[20];

	fptr = fopen("test.txt", "a");
	LOG_TO_FILE(fptr, "This is a test\n");
	fclose(fptr);

	fptr = fopen("test.txt", "r+");
	fgets(str, 20, fptr);
	fclose(fptr);

	int status = strcmp(str, "This is a test\n");
	assert(status == 0);

	return 0;
}
