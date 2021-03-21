#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char ** argv)
{
	printf("Hello World\n");
	int pid = getpid();
	char cmdline[64];
	sprintf(cmdline, "cat /proc/%ld/maps", pid);
	system(cmdline);
	sleep(10);
	return 0;
}
