#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	pid_t p = getpid();
	printf("%d\n",p);
	return 0;
}
